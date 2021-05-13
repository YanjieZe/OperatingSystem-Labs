#include "buffer.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

/* the buffer */
buffer_item buffer[BUFFER_SIZE];
int free_slot[BUFFER_SIZE];

/* mutex and semaphore */
pthread_mutex_t buffer_mutex;
sem_t full;
sem_t empty;

/* insert item into buffer
return 0 if successful, otherwise
return -1 indicating an error condition */
int insert_item(buffer_item item) {
    
    int insert_success=0;
    for(int i=0; i<BUFFER_SIZE; ++i)
    {
        if(free_slot[i]==1)
        {
            insert_success = 1;
            buffer[i] = item;
            free_slot[i] = 0;
            break;
        }
    }
   
    if(insert_success==1)
        return 0;
    else    
        return 1;
}

/* remove an object from buffer
placing it in item
return 0 if successful, otherwise
return -1 indicating an error condition */
int remove_item(buffer_item *item) {
  
    int remove_success=0;
    for(int i=0; i<BUFFER_SIZE; ++i)
    {
        if(free_slot[i]==0)
        {
            remove_success = 1;
            (*item) = buffer[i];
            free_slot[i] = 1;
            break;
        }
    }
    
    if(remove_success)
        return 0;
    else
        return 1;
}

void *producer(void *param) {
    buffer_item item;
    while (1) {
    /* sleep for a random period of time */
    int sleep_a_while = rand()%3;
    sleep(sleep_a_while);

    /* generate a random number */
    sem_wait(&empty);
    pthread_mutex_lock(&buffer_mutex);
    item = rand()%100;
    if (insert_item(item))
        printf("Producer Insert Failure.\n");
    else
        printf("Producer produced %d.\n",item);
    pthread_mutex_unlock(&buffer_mutex);
    sem_post(&full);
    }
}

void *consumer(void *param) 
{ 
    buffer_item item;
    while (1) {
    /* sleep for a random period of time */
    int sleep_a_while = rand()%3;
    sleep(sleep_a_while);
    sem_wait(&full);
    pthread_mutex_lock(&buffer_mutex);
    if (remove_item(&item))
    printf("Consumer Remove Failure.\n");
    else
    printf("Consumer consumed %d.\n",item);
    pthread_mutex_unlock(&buffer_mutex);
    sem_post(&empty);
    }
}

int main(int argc, char*argv[])
{
    if(argc!=4)
    {
        printf("You should input:\n");
        printf("1.sleep time.\n");
        printf("2.num of producers.\n");
        printf("3.num of consumers.\n");
        return 1;
    }

    /* 1. Get command line arguments argv[1],argv[2],argv[3] */
    int sleep_time = atoi(argv[1]);
    int num_producer = atoi(argv[2]);
    int num_consumer = atoi(argv[3]);
   
    /* 2. Initialize buffer */
    pthread_mutex_init(&buffer_mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);  
    for(int i=0; i<BUFFER_SIZE; i++)
        free_slot[i]=1; // set all buffers to free
    
    /* 3. Create producer thread(s) */
    pthread_t producer_thread[num_producer];
    for(int i=0; i<num_producer; i++)
    {
        pthread_create(&producer_thread[i], NULL, producer, NULL);
    }

    /* 4. Create consumer thread(s) */
    pthread_t consumer_thread[num_consumer];
    for(int i=0; i<num_consumer; i++)
    {
        pthread_create(&consumer_thread[i], NULL, consumer, NULL);
    }
   
    /* 5. Sleep */
    printf("*Main* begin to sleep.\n");
    sleep(sleep_time);
    printf("*Main* wake and begin to terminate.\n");

    /* 6. Exit */
    for(int i=0; i<num_producer; i++)
    {
        pthread_cancel(producer_thread[i]);
    }
    for(int i=0; i<num_consumer; i++)
    {
        pthread_cancel(consumer_thread[i]);
    }
    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&buffer_mutex);
    printf("Cancel all. Finish.\n");
    return 0;
}