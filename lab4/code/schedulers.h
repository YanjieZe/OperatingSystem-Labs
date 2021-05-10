#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

#include "list.h"

// add a task to the list 
void add(char *name, int priority, int burst, struct node **head);

// invoke the scheduler
void schedule(struct node **head);
