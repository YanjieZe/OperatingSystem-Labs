#include "schedulers.h"
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

void add(char *name, int priority, int burst, struct node **head){
    Task *t_new = (Task*)malloc(sizeof(Task*));
    t_new->name = name;
    t_new->priority = priority;
    t_new->burst = burst;
    insert(head, t_new);
}

    /* FCFS*/
void schedule(struct node **head){

    struct node *current_node, *front_node, *back_node;
    current_node = *head;
    front_node = NULL;
    //printf("%s\n", current_node->task->name);

    while(current_node!=NULL && current_node->next!=NULL)
    {
        back_node = current_node->next;

        current_node->next = front_node;
        //printf("%s\n", current_node->task->name);

        front_node = current_node;
        current_node = back_node;     
    }
    // process the end
    if(current_node!=NULL && current_node->next==NULL)
        current_node->next = front_node;
    *head = current_node;

    Task *tmp_task;
    while(current_node!=NULL)
    {
        tmp_task = current_node->task;
        run(tmp_task, tmp_task->burst);
        current_node = current_node->next;
    }
}
