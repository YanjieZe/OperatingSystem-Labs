/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the work queue
task task_queue[QUEUE_SIZE];
task worktodo;

// the current size
int queue_current_size;

// the thread queue
pthread_t thread_queue[NUMBER_OF_THREADS];

// the mutex lock for queue
pthread_mutex_t queue_mutex;

// work signal for threads;
int thread_working[NUMBER_OF_THREADS];

// sem for thread
sem_t thread_sem;

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{   if(queue_current_size < QUEUE_SIZE)
    {
        pthread_mutex_lock(&queue_mutex);
        task_queue[queue_current_size] = t;
        queue_current_size++;
        pthread_mutex_unlock(&queue_mutex);
        return 0;
    }
    else
    {
        printf("Enqueue Failure.\n");
        return 1;
    }
    return 0;
}


// remove a task from the queue
task dequeue() 
{

    if(queue_current_size!=0)
    {  
        pthread_mutex_lock(&queue_mutex);
        worktodo = task_queue[0];
        queue_current_size--;

        for(int i=0; i<queue_current_size; i++)
            task_queue[i] = task_queue[i+1];
        task_queue[queue_current_size].function = NULL;
        task_queue[queue_current_size].data = NULL;
       
        pthread_mutex_unlock(&queue_mutex);
    }
    else
    {
        worktodo.data = NULL;
        worktodo.function = NULL;
        printf("Dequeue Error: Empty Queue.\n");
    }
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
    int *idx = (int*)param;
    int thread_idx = *idx;
    worktodo = dequeue();
    printf("Execute with thread %u .\n", thread_idx);
    execute(worktodo.function, worktodo.data);
    
    //finish task
    thread_working[thread_idx]=0;
    sem_post(&thread_sem);
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    worktodo.function = somefunction;
    worktodo.data = p;

    int not_success = enqueue(worktodo);
    int work_thread_idx = 0;
    if(!not_success)
    {
        sem_wait(&thread_sem);
        while(1)
        {
            if(thread_working[work_thread_idx]==0)
            {
                thread_working[work_thread_idx]=1;
                break;
            }
            else
            {
                work_thread_idx = (work_thread_idx+1)%(NUMBER_OF_THREADS);
            }
        } 
    }
    else
    {
        printf("Pool Submit Failure.\n");
        return 1;
    }
    pthread_create(&thread_queue[work_thread_idx], NULL, &worker, &work_thread_idx);

    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    queue_current_size = 0;
    sem_init(&thread_sem,0, NUMBER_OF_THREADS);
    pthread_mutex_init(&queue_mutex, NULL);
    for(int i=0 ; i<NUMBER_OF_THREADS; i++)
        thread_working[i]=0;// state of not working
}
 
// shutdown the thread pool
void pool_shutdown(void)
{
    for(int i=0; i<NUMBER_OF_THREADS; ++i)
        pthread_join(thread_queue[i], NULL);
    pthread_mutex_destroy(&queue_mutex);
    sem_destroy(&thread_sem);
}
