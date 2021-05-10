#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *array;
int *array_copy;

typedef struct
{
    /* data */
    int left;
    int right;
    int mid;
} range;


void quick_sort(int *a, int low, int high)
{
    int i = low;
    int j = high;
    int pivot = a[low];
    if (low >= high) 
    {
        return ;
    }
    
    while (low < high) 
    {
        while (low < high && pivot <= a[high])
        {
            --high; 
        }
        if (pivot > a[high])
        {
            int tmp;
            tmp = a[low];
            a[low] = a[high];
            a[high] =tmp;
            ++low;
        }
        while (low < high && pivot >= a[low])
        {
            ++low; 
        }
        if (pivot < a[low])
        {
            int tmp;
            tmp = a[low];
            a[low] = a[high];
            a[high] =tmp;
            --high;
        }
    }
    quick_sort(a, i, low-1); 
    quick_sort(a, low+1, j);
}

void *sort(void *r)
{
    int left = ((range*)r)->left;
    int right = ((range*)r)->right;
    quick_sort(array, left, right); 
}

void *merge(void *r)
{   
    int left = ((range*)r)->left;
    int right = ((range*)r)->right;
    int mid = ((range*)r)->mid;

    int current1=left;
    int current2=mid+1;
    int copy_id = 0;

    while(current1<=mid && current2<=right)
    {
        if(array[current1]<=array[current2])
            {
                array_copy[copy_id]=array[current1];
                current1++;
                copy_id++;
            }
        else
        {
            array_copy[copy_id]=array[current2];
            current2++;
            copy_id++;
        }
    }
    if(copy_id==right)
    {
        for(int i=left;i<=right;i++)
            array[i]=array_copy[i];
    }
    else if(current1<mid)
    {
        for(int i=current1;i<=mid;i++)
            {
                array_copy[copy_id]=array[i];
                copy_id++;
            }
        for(int i=0; i<=right;i++)
            array[i]=array_copy[i];
    }
    else if(current2<right)
    {
        for(int i=current2;i<=right;i++)
            {
                array_copy[copy_id]=array[i];
                copy_id++;
            }

        for(int i=0; i<=right;i++)
            array[i]=array_copy[i];
    }  
}

int main()
{
    int n;
    scanf("%d", &n);
    printf("Your input: %d, then please input array. \n", n);
    array = (int*)malloc(sizeof(int)*n);
    array_copy = (int*)malloc(sizeof(int)*n);

    for(int i=0;i<n;++i)
    {
        scanf("%d", &array[i]);        
    }

    pthread_t tid1, tid2, tid3;
    pthread_attr_t attr1, attr2, attr3;
    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);
    pthread_attr_init(&attr3);

    range *r[3];
    for(int i=0;i<3;++i)
        r[i] = (range*)malloc(sizeof(range));

    int mid = (n-1)/2;
    if(n>1){
    r[0]->left = 0;
    r[0]->right = mid;

    r[1]->left = mid+1;
    r[1]->right = n-1;
    r[2]->left = 0;
    r[2]->right = n-1;
    r[2]->mid = mid;
    }
    if(n==1)
    {
        printf("Sort Result: %d\n", array[0]);
        return 0;
    }
    pthread_create(&tid1, &attr1, sort, r[0]);//sort
    pthread_create(&tid2, &attr2, sort, r[1]); //sort
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_create(&tid3, &attr3, merge, r[2]);//merge
    pthread_join(tid3, NULL);

    
    printf("Sort Result: ");
    for(int i=0;i<n;++i)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
    // free space
    for(int i=0;i<3;++i)
        free(r[i]);
    free(array_copy);
    free(array);
}