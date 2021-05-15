#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
#define SIZE 100
/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];
/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int safety_algorithm(int *a)
{
    int work[NUMBER_OF_RESOURCES];
    for(int i=0; i<NUMBER_OF_RESOURCES; i++)
        work[i] = available[i]-a[i+1];
 
    int finish[NUMBER_OF_CUSTOMERS];
    for(int i=0; i<NUMBER_OF_CUSTOMERS; ++i)
        finish[i]=0;

    int all_satisfy;
    int i;
    for(i=0; i<NUMBER_OF_CUSTOMERS; ++i)
    {
        all_satisfy=0;//refresh

        if(finish[i]==0){
            for(int j=0; j<NUMBER_OF_RESOURCES; ++j)
            {   
                if(i==a[0])
                {
                    if(need[i][j]-a[j+1]<=work[j])
                        all_satisfy++;
                }
                else if(need[i][j]<=work[j])
                    all_satisfy++;
            }
            if(all_satisfy==NUMBER_OF_RESOURCES)
            {   //this customer can be finished
                for(int j=0; j<NUMBER_OF_RESOURCES; ++j)
                    work[j] += allocation[i][j];
                finish[i]=1;
                i=-1;// back to the start
            }  
        }
    }

    all_satisfy = 0;
    for(i=0; i<NUMBER_OF_CUSTOMERS; ++i)
        if(finish[i]==1)
            all_satisfy++;
    if(all_satisfy==NUMBER_OF_CUSTOMERS)
        return 0; //safe
    else
        return -1; //unsafe
}


/* request resource*/
int request(int *a)
{
    
    int consumer_id = a[0];
    
    if(consumer_id>=NUMBER_OF_CUSTOMERS)
    {   
        printf("Request Error: ID Exceed\n");
        return -1;
    }
    // basic check
    for(int i=1;i<=NUMBER_OF_RESOURCES;++i)
    {
        if(a[i]>need[consumer_id][i-1])
        {
            printf("Request Error: Request Exceed Need\n");
            return -1;
        }
        if(a[i]>available[i-1])
        {
            printf("Request Error: Request Exceed Available\n");
            return -1;
        }
    }
    /* safety algorithm*/
    int is_safe = safety_algorithm(a);
    if(is_safe==-1)
    {
        printf("Request Makes Unsafe State!!!\n");
        return 1;
    }
    else
    {
        printf("Request is satisfied.\n");
    }
    // after the request, the state should be safe.
    for(int i=1;i<=NUMBER_OF_RESOURCES;++i)
    {
        available[i-1] -= a[i];
        need[consumer_id][i-1] -= a[i];
        allocation[consumer_id][i-1] += a[i];
    }
    return 0;
}


/* release resource*/
int release(int *a)
{
    int consumer_id = a[0];
    if(consumer_id>=NUMBER_OF_CUSTOMERS)
    {   
        printf("Request Error: ID Exceed\n");
        return -1;
    }
    // basic check
    for(int i=1;i<=NUMBER_OF_RESOURCES;++i)
    {
        if(a[i]>allocation[consumer_id][i-1])
        {
            printf("Release Error: Release Exceed Allocation\n");
            return -1;
        }
    }
    // no exceed
    for(int i=1;i<=NUMBER_OF_RESOURCES; ++i)
    {
        allocation[consumer_id][i-1] -= a[i];
    }
    return 0;
}


/* show all the states*/
void state_show()
{
    int i;
    int j;
    printf("-----------------Available Resource---------------\n");
    for(i=0;i<NUMBER_OF_RESOURCES;++i)
        printf("resource[%d]=%d ", i, available[i]);
    printf("\n\n");

    printf("---------------------Maximum----------------------\n");
    for(i=0;i<NUMBER_OF_CUSTOMERS;++i)
    {
        printf("custom[%d]   ", i);
        for(j=0;j<NUMBER_OF_RESOURCES;++j)
            printf("resource[%d]=%d ", j, maximum[i][j]);
        printf("\n");
    }
    printf("\n");

    printf("---------------------Allocation--------------------\n");
    for(i=0;i<NUMBER_OF_CUSTOMERS;++i)
    {
        printf("custom[%d]   ", i);
        for(j=0;j<NUMBER_OF_RESOURCES;++j)
            printf("resource[%d]=%d ", j, allocation[i][j]);
        printf("\n");
    }
    printf("\n");

    printf("-----------------------Need-----------------------\n");
    for(i=0;i<NUMBER_OF_CUSTOMERS;++i)
    {
        printf("custom[%d]   ", i);
        for(j=0;j<NUMBER_OF_RESOURCES;++j)
            printf("resource[%d]=%d ", j, need[i][j]);
        printf("\n");
    }
    printf("\n");
    printf("-----------------------End-----------------------\n");


}


int main()
{
    // initialize array MAXIMUM with txt file
    FILE *in;
    in = fopen("max_need.txt", "r");
    char customer[SIZE];
    char *temp;
    for(int i=0;i<NUMBER_OF_CUSTOMERS;++i)
    {
        fgets(customer,SIZE,in);
        temp = strdup(customer);
        for(int j=0;j<NUMBER_OF_RESOURCES;j++)
            maximum[i][j]=need[i][j]=atoi(strsep(&temp, ",")), allocation[i][j]=0; 
    }
    fclose(in);

    // initialize array **available**  with txt file
    in = fopen("available_resource.txt", "r");
    char ava[SIZE];
    fgets(ava, SIZE, in);
    temp = strdup(ava);
    for(int i=0;i<NUMBER_OF_RESOURCES;i++)
        available[i]=atoi(strsep(&temp, ","));
    fclose(in);

    
    int input_instruction[NUMBER_OF_RESOURCES+1];
    int request_value;
    int release_value;
    int tmp;


    char input_line[SIZE];
    int running = 1;
    while(running)
    {
        printf("banker>");
        fgets(input_line, SIZE, stdin);
      
        if(strcmp(input_line, "end\n")==0) 
        {
            running = 0;
            printf("---------------Banker's Algorithm Finished-------------\n");
            continue;
        }
        
        if(input_line[0]=='R')
        {
            if(input_line[1]=='Q')
            {
                //decode instruction
                temp = strdup(input_line);
                strsep(&temp," ");
                for(int i=0; i<=NUMBER_OF_RESOURCES;++i)
                    input_instruction[i] = atoi(strsep(&temp, " "));
                request_value = request(input_instruction);
                if(request_value!=0)
                    printf("Request Failed. Please Follow Rules.\n");
                continue;
            }
            else if(input_line[1]=='L')
            {
                temp = strdup(input_line);
                strsep(&temp," ");
                for(int i=0; i<=NUMBER_OF_RESOURCES;++i)
                    input_instruction[i] = atoi(strsep(&temp, " "));
                release_value = release(input_instruction);
                if(release_value!=0)
                    printf("Release Failed. Please Follow Rules.\n");
                continue;
            }
        }
        else if(input_line[0]=='*')
                state_show();

    }

    
}