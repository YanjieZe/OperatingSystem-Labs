#include<stdio.h>
#include<stdlib.h>
#define PAGE_NUM 256
#define PAGE_SIZE 256
#define FRAME_NUM 256
#define FRAME_SIZE 256
#define TLB_ENTRY_NUM 16
#define PAGE_TABLE_SIZE 256

FILE* addresses;
FILE* backing_store;
FILE* result_file;

typedef struct PAGE_TABLE_ITEM
{
    int valid;
    int frame_id;
} page_table_item;

typedef struct TLB_ITEM
{
    int used_time;
    int frame_id;
    int page_id;
} tlb_item;

typedef struct MEMORY_ITEM
{
    int used_time;
    char data[FRAME_SIZE];
} memory_item;


page_table_item page_table[PAGE_TABLE_SIZE];
tlb_item TLB[TLB_ENTRY_NUM];
memory_item memory[FRAME_NUM];


/* initialize page table */
void page_table_init()
{
    for(int i=0; i<PAGE_TABLE_SIZE; i++)
    {
        page_table[i].valid = 0;// set to not valid
        page_table[i].frame_id = -1; // set to None
    }
}

/* initialize TLB */
void tlb_init()
{
    for(int i=0; i<TLB_ENTRY_NUM; i++)
    {
        TLB[i].used_time = -1; 
        TLB[i].frame_id = -1;
        TLB[i].page_id = -1;
    }
}

/* initialize memory*/
void memory_init()
{
    for(int i=0; i<FRAME_NUM; i++)
    {
        memory[i].used_time = -1;
    }
}

/* initialize page table, TLB, memory */
void initialize()
{
    page_table_init();
    tlb_init();
    memory_init();
    printf("Initialize Finish.\n");
}


/* get the page num, given address*/
int get_page(int address)
{
    address = address>>8;
    return address;
}


/*get the offset, given address*/
int get_offset(int address)
{
    return address - (get_page(address)<<8);
}


/* LRU replacement for TLB*/
void TLB_LRU_Replacement(int page_id, int frame_id, int time)
{
    int min_time=time;
    int min_idx = 0;
    // find the least recently used 
    for(int i=0; i<TLB_ENTRY_NUM; i++)
    {
        if(TLB[i].used_time<min_time)
        {
            min_time = TLB[i].used_time;
            min_idx = i;
        }
    }
    TLB[min_idx].frame_id = frame_id;
    TLB[min_idx].page_id = page_id;
    TLB[min_idx].used_time = time;
}

/* LRU Replacement for Memory
    Update new data
    return the frame id */
int memory_LRU_Replacement(int page_id, int time)
{
    int min_time=time;
    int min_idx = 0;
    // find the least recently used 
    for(int i=0; i<FRAME_NUM; i++)
    {
        if(memory[i].used_time<min_time)
        {
            min_time = memory[i].used_time;
            min_idx = i;
        }
    }
    memory[min_idx].used_time = time;

    //find the old page id, and set invalid
    for(int i=0; i<PAGE_TABLE_SIZE; i++)
    {
        if(page_table[i].frame_id==min_idx)
        {
            page_table[i].valid = -1;
        }
    }

    // seek data
    fseek(backing_store, page_id*PAGE_SIZE, SEEK_SET);
    fread(memory[min_idx].data, sizeof(char), FRAME_SIZE, backing_store);

    return min_idx;
}

int main(int argc, char*argv[])
{

    addresses = fopen(argv[1], "r");
    backing_store = fopen("BACKING_STORE.bin", "rb");
    result_file = fopen("result.txt", "w");

    // init
    initialize();

    // addrees parse arg
    int address;
    int page_id;
    int frame_id;
    int offset;

    // count arg
    int count=0;
    int tlb_hit = 0;
    int page_fault = 0;
    int time = 0;

    fscanf(addresses, "%u", &address);
    while(!feof(addresses))
    {
        count ++; 
        time ++;

        //decode address
        page_id = get_page(address);
        offset = get_offset(address);
        
        
        // search in TLB
        int tlb_find=0;
        for(int i=0; i<TLB_ENTRY_NUM; i++)
        {
            if(page_id==TLB[i].page_id)
            {
                tlb_hit ++;
                tlb_find = 1;
                frame_id = TLB[i].frame_id;
                memory[frame_id].used_time = time;
                TLB[i].used_time = time;
                break;
            }
        }

        // not find in TLB ,search in page table
        int page_find=0;
        if(!tlb_find)
        {   
            // valid = 1, find page
            if(page_table[page_id].valid==1)
            {
                page_find = 1;
                frame_id = page_table[page_id].frame_id;
                memory[frame_id].used_time = time;
                TLB_LRU_Replacement(page_id, frame_id, time); // Update TLB
                
            }
            else // valid = -1, not find, page fault , do demand paging
            {
                page_fault++;
               
                // demand paging
                frame_id = memory_LRU_Replacement(page_id, time);

                //update page table
                page_table[page_id].frame_id = frame_id;
                page_table[page_id].valid = 1;// set valid
                
                //update TLB
                TLB_LRU_Replacement(page_id, frame_id, time);                       
            }
        }
        // calculate physical address and get data
        int physical_address = frame_id*FRAME_SIZE + offset;
        int data = memory[frame_id].data[offset];

        // output the result
        fprintf(result_file, "Virtual address: %d Physical address: %d Value: %d\n", address, physical_address, data);
        fscanf(addresses, "%u", &address);  
    }

    // finish   
    fclose(addresses);
    fclose(backing_store);
    fclose(result_file);
    
    printf("Execution Finish.\n");
    printf("-----------------------------\n");

    //statistics
    double page_fault_rate = page_fault/(double)count;
    double tlb_hit_rate = tlb_hit/(double)count;
    printf("Frame Num = %d:\n", FRAME_NUM);
    printf("TLB Hit Rate: %f\n", tlb_hit_rate);
    printf("Page Fault Rate: %f\n", page_fault_rate);
    
}
