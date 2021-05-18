#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 100

typedef struct Process
{
    long start;
    long end;
    long compacity;
    int id;
    struct Process *next;
    // when id=-1, this means "unused".
} process;


int first_fit(process **memory_head, process* new_process)
{
    process *temp_process=malloc(sizeof(process));
    temp_process = (*memory_head);

    if(temp_process->id==-1)// 第一次分配
    {
        if(temp_process->compacity>=new_process->compacity)//满足分配情况
        {
            new_process->start = 0;
            new_process->end = new_process->compacity - 1;
            temp_process->compacity -= new_process->compacity;
            temp_process->start = new_process->end + 1;

            if(temp_process->compacity==0)//用完unused block
            {
                new_process->next=NULL;
                free(temp_process);
            }
            else
                new_process->next = temp_process;

            *memory_head = new_process;
            return 1;//第一次分配，分配成功
        }
        else
            return 0;//分配失败
    }

    process *prev_process=malloc(sizeof(process));

    while (temp_process->next!=NULL)
    {
        prev_process = temp_process;
        temp_process = temp_process->next;
        if(temp_process->id!=-1)
            continue;

        // find an unused hole
        if(temp_process->compacity<new_process->compacity)
            continue;
        
        //allocation
        temp_process->compacity -= new_process->compacity;
        new_process->start = temp_process->start;
        new_process->end = new_process->start + new_process->compacity - 1;
        prev_process->next = new_process;
        temp_process->start = new_process->end + 1;
        if(temp_process->compacity==0)
        {
            new_process->next=temp_process->next;
            free(temp_process);
        }
        else
        {
            new_process->next = temp_process;
        }
        
        return 1;// 分配成功
    }
    
    return 0;
}


int best_fit(process **memory_head, process* new_process)
{
    process *temp_process=malloc(sizeof(process));
    temp_process = (*memory_head);

    if(temp_process->id==-1)// 第一次分配
    {
        if(temp_process->compacity>=new_process->compacity)//满足分配情况
        {
            new_process->start = 0;
            new_process->end = new_process->compacity - 1;
            temp_process->compacity -= new_process->compacity;
            temp_process->start = new_process->end + 1;

            if(temp_process->compacity==0)//用完unused block
            {
                new_process->next=NULL;
                free(temp_process);
            }
            else
                new_process->next = temp_process;

            *memory_head = new_process;
            return 1;//第一次分配，分配成功
        }
        else
            return 0;//分配失败
    }

    process *prev_process=malloc(sizeof(process));
    int minimal_cp = 0;
    while (temp_process->next!=NULL)
    {
        prev_process = temp_process;
        temp_process = temp_process->next;
        if(temp_process->id!=-1)
            continue;

        // find an unused hole
        if(temp_process->compacity<new_process->compacity)
            continue;
        
        if(minimal_cp==0)// first find
            minimal_cp=temp_process->compacity;

        if(temp_process->compacity<minimal_cp)
            minimal_cp = temp_process->compacity;   
    }

    if(minimal_cp==0)
        return 0;
    
    temp_process = (*memory_head);
    while (temp_process->next!=NULL)
    {
        prev_process = temp_process;
        temp_process = temp_process->next;
        if(temp_process->id!=-1)
            continue;

        // find an unused hole
        if(temp_process->compacity<new_process->compacity)
            continue;

        if(temp_process->compacity==minimal_cp)
        {
        //allocation
        temp_process->compacity -= new_process->compacity;
        new_process->start = temp_process->start;
        new_process->end = new_process->start + new_process->compacity - 1;
        prev_process->next = new_process;
        temp_process->start = new_process->end + 1;
        if(temp_process->compacity==0)
        {
            new_process->next=temp_process->next;
            free(temp_process);
        }
        else
        {
            new_process->next = temp_process;
        }
        
        return 1;// 分配成功
        }
    }
    return 0;
}


int worst_fit(process **memory_head, process* new_process)
{
    process *temp_process=malloc(sizeof(process));
    temp_process = (*memory_head);

    if(temp_process->id==-1)// 第一次分配
    {
        if(temp_process->compacity>=new_process->compacity)//满足分配情况
        {
            new_process->start = 0;
            new_process->end = new_process->compacity - 1;
            temp_process->compacity -= new_process->compacity;
            temp_process->start = new_process->end + 1;

            if(temp_process->compacity==0)//用完unused block
            {
                new_process->next=NULL;
                free(temp_process);
            }
            else
                new_process->next = temp_process;

            *memory_head = new_process;
            return 1;//第一次分配，分配成功
        }
        else
            return 0;//分配失败
    }

    process *prev_process=malloc(sizeof(process));
    int max_cp = 0;
    while (temp_process->next!=NULL)
    {
        prev_process = temp_process;
        temp_process = temp_process->next;
        if(temp_process->id!=-1)
            continue;

        // find an unused hole
        if(temp_process->compacity<new_process->compacity)
            continue;
        
        if(max_cp==0)// first find
            max_cp=temp_process->compacity;

        if(temp_process->compacity>max_cp)
            max_cp = temp_process->compacity;   
    }

    if(max_cp==0)
        return 0;
    
    temp_process = (*memory_head);
    while (temp_process->next!=NULL)
    {
        prev_process = temp_process;
        temp_process = temp_process->next;
        if(temp_process->id!=-1)
            continue;

        // find an unused hole
        if(temp_process->compacity<new_process->compacity)
            continue;

        if(temp_process->compacity==max_cp)
        {
        //allocation
        temp_process->compacity -= new_process->compacity;
        new_process->start = temp_process->start;
        new_process->end = new_process->start + new_process->compacity - 1;
        prev_process->next = new_process;
        temp_process->start = new_process->end + 1;
        if(temp_process->compacity==0)
        {
            new_process->next=temp_process->next;
            free(temp_process);
        }
        else
        {
            new_process->next = temp_process;
        }
        
        return 1;// 分配成功
        }
    }
    return 0;
}


int release_process(process **memory_head, int process_id)
{
    process *temp_process=malloc(sizeof(process));
    process *prev_process=malloc(sizeof(process));
    temp_process = (*memory_head);
    prev_process = NULL;

    while(temp_process!=NULL)
    {
        if(temp_process->id==process_id)
            temp_process->id = -1;
        temp_process = temp_process->next;     
    }

    //compress unused block
    temp_process = (*memory_head);
    
    while(temp_process!=NULL)
    {
        if(temp_process->id==-1)
        {
            
            if(prev_process!=NULL&&prev_process->id==-1)
            {
                prev_process->compacity += temp_process->compacity;
                prev_process->end = temp_process->end;
                prev_process->next = temp_process->next;
                free(temp_process);
                temp_process = prev_process->next;
            }
            else if(prev_process==NULL&&temp_process->next==NULL)
            {
                temp_process->id=-1;
                return 1;
            }
            else
            {
                prev_process = temp_process;
                temp_process = temp_process->next;
            }
        }   
        else
        {
            prev_process = temp_process;
            temp_process = temp_process->next;
        }

    }
    return 1;
}


void show_process_state(process *single_process)
{
    int id = single_process->id;
    int cp = single_process->compacity;
    if(id==-1)
        if(cp==1)
            printf("Addresses [%ld] Unused\n", single_process->start);
        else
            printf("Addresses [%ld:%ld] Unused\n", single_process->start, single_process->end);
    else
        if(cp==1)
            printf("Addresses [%ld] Process P%d\n", single_process->start, single_process->id);
        else
            printf("Addresses [%ld:%ld] Process P%d\n", single_process->start, single_process->end, single_process->id);
}


void show_state(process **memory_head)
{
    process *temp_process=malloc(sizeof(process));
    temp_process = (*memory_head);
    show_process_state(temp_process);

    while (temp_process->next!=NULL)
    {
        temp_process = temp_process->next;
        show_process_state(temp_process);
    }
}



int main(int argc, char* argv[])
{
    // Memory Initialize
    long max_space;
    max_space = atoi(argv[1]);
    process *memory=malloc(sizeof(process));
    memory->start = 0;
    memory->end =  max_space-1;
    memory->compacity = max_space;//compacity = end - start + 1
    memory->next = NULL;
    memory->id = -1;
    
    process **memory_head=malloc(sizeof(process*));
    *memory_head=memory;
   

    int running=1;
    char instruction[SIZE];
    char *temp;

    while (running)
    {
        printf("allocator>");
        fgets(instruction, SIZE, stdin); //会读入换行符

        // "X": exit
        if(instruction[0]=='X')
        {
            running = 0;
            continue;
        }

        // "STAT": print states
        if(instruction[0]=='S'&&instruction[1]=='T'&&instruction[2]=='A'&&instruction[3]=='T')
            show_state(memory_head);

        // "RQ": request
        if(instruction[0]=='R'&&instruction[1]=='Q')
        {
            temp = strdup(instruction);
            strsep(&temp, " ");
            char *process_name=strsep(&temp, " ");
            process_name++; // skip "P"

            int process_id = atoi(process_name);
            long process_size = atoi(strsep(&temp, " "));
            char *insert_mode = strsep(&temp, " ");

            process *new_process=malloc(sizeof(process));
            new_process->id = process_id;
            new_process->compacity = process_size;
            
            // execute algorithm
            int allocation_success=0;
            if(insert_mode[0]=='F')
                allocation_success = first_fit(memory_head, new_process);
            else if (insert_mode[0]=='B')
                allocation_success = best_fit(memory_head, new_process);
            else if (insert_mode[0]=='W')
                allocation_success = worst_fit(memory_head, new_process);
            else
            {
                printf("Error: No such mode.\n");
                continue;
            }
            if(allocation_success)
                printf("Allocate P%d Success.\n", new_process->id);
            else
                printf("Allocate P%d Fail!\n", new_process->id);
        }

        if(instruction[0]=='R'&&instruction[1]=='L')
        {   
            temp = strdup(instruction);
            strsep(&temp, " ");
            char *process_name=strsep(&temp, " ");
            process_name++; // skip "P"
            int process_id = atoi(process_name);

            int release_success = release_process(memory_head, process_id);

            if(release_success)
                printf("Release P%d success.\n", process_id);
            else
                printf("Release P%d fail.\n", process_id);
        }
    }   
}
