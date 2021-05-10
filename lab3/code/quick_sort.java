import java.util.concurrent.*;

public class SortTask extends RecursiveAction
{
    static final int SIZE = 5000;
    static final int THRESHOLD = 500;

    private int begin;
    private int end;
    private int[] array;

    public SortTask(int begin, int end, int[] array) 
    {
        this.begin = begin;
        this.end = end;
        this.array = array;
    }

    protected void compute() 
    {
        if (end - begin < 4*THRESHOLD) {
            
            this.quick_sort_pivot(begin, end);
            return;
        }
        else{
            // divide stage 
            int mid = (end+begin)/2;
            SortTask leftTask = new SortTask(begin, mid, array);
            SortTask rightTask = new SortTask(mid + 1, end, array);

            leftTask.fork();
            rightTask.fork();
            // leftTask.quick_sort_pivot(begin, mid);
            // rightTask.quick_sort_pivot(mid+1, end);

            leftTask.join();
            rightTask.join();
            this.merge(begin, mid, end);
            return;
        }
    }
    
    


    public void quick_sort_pivot(int low, int high)
    {   
        if (end - begin < THRESHOLD) {
            
            this.selection_sort(begin, end);
            return;
        }
        int i=low;
        int j=high;
        int pivot = array[low];
        if(low>=high)
            return;

        while(low<high)
        {
            while(low<high && pivot<=array[high])
                high--;
            if(pivot > array[high])
            {
                int tmp;
                tmp = array[low];
                array[low] = array[high];
                array[high] =tmp;
                ++low;
            }
            while (low < high && pivot >= array[low])
            {
            ++low; 
            }
            if (pivot < array[low])
            {
                int tmp;
                tmp = array[low];
                array[low] = array[high];
                array[high] =tmp;
                --high;
            }
        }
        quick_sort_pivot(i, low-1); 
        quick_sort_pivot(low+1, j);
        //     return i;
    }

    private void selection_sort(int low, int high)
    {
        
        
        for(int i=low;i<high;i++)
        {
            int min = array[i];
            for(int j=i+1;j<=high;j++)
                {
                    if(array[j]<min)
                    {
                        min = array[j];
                        array[j] = array[i];
                        array[i] = min;
                    }
                }
        }
    }

    private void merge(int left, int mid, int right)
    {
        int current1=left;
        int current2=mid+1;
        int copy_id =left;
        int[] array_copy = new int[SIZE];

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
            for(int i=left; i<=right;i++)
                array[i]=array_copy[i];
        }
        else if(current2<right)
        {
                for(int i=current2;i<=right;i++)
                {
                    array_copy[copy_id]=array[i];
                    copy_id++;
                }

                for(int i=left; i<=right;i++)
                    array[i]=array_copy[i];
        }   
            
    }




	public static void main(String[] args) 
    {
		ForkJoinPool pool = new ForkJoinPool();
		int[] array = new int[SIZE];

		// create SIZE random integers between 0 and 9
		java.util.Random rand = new java.util.Random();

		System.out.println("Init array is: ");

		for (int i = 0; i < SIZE; i++) {
			array[i] = rand.nextInt(10);
            
            System.out.print(array[i]);
            System.out.print(' ');
		}		
		// use fork-join parallelism to sum the array
		SortTask task = new SortTask(0, SIZE-1, array);

		pool.invoke(task);
        System.out.println(' ');

		System.out.println("Sorted array is: ");
        for (int i = 0; i < SIZE; i++) {
            System.out.print(array[i]);
            System.out.print(' ');
		}
	}
}
