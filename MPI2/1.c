#include <stdio.h>
#include <stdlib.h>
#include<mpi.h> 

struct bucket 
{
    int count;
    int* value;
};
 
int compareIntegers(const void* first, const void* second)
{
    int x = *((int*)first), y =  *((int*)second);
    if (x == y)
    {
        return 0;
    }
    else if (x < y)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}
 
void bucketSort(int array[],int n)
{
    struct bucket buckets[3];
    int i, j, k;
    for (i = 0; i < 3; i++)
    {
        buckets[i].count = 0;
        buckets[i].value = (int*)malloc(sizeof(int) * n);
    }
    
    for (i = 0; i < n; i++)
    {
        if (array[i] < 0)
        {
            buckets[0].value[buckets[0].count++] = array[i];
        }
        else if (array[i] > 10)
        {
            buckets[2].value[buckets[2].count++] = array[i];
        }
        else
        {
            buckets[1].value[buckets[1].count++] = array[i];
        }
    }
    for (k = 0, i = 0; i < 3; i++)
    {
        qsort(buckets[i].value, buckets[i].count, sizeof(int), &compareIntegers);
        for (j = 0; j < buckets[i].count; j++)
        {
            array[k + j] = buckets[i].value[j];
        }
        k += buckets[i].count;
        free(buckets[i].value);
    }
}
 
int main(int argc,char *arg[]) {
    int size,rank,no,i=12,j,k,n;
    n=i;
    int array[20] = {-3, 45, 78, 90, -90, -88, 34, 56, 76, 89, -65, 23 };
    MPI_Init(NULL,NULL);
    MPI_Status status; 
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank==0){
	    printf("Numbers\n");
	    for (j = 0; j<i; j++)
	    {
		printf("%d ", array[j]);
	    }
	    printf("\n");
	    MPI_Send(&array,1,MPI_INT,1,1,MPI_COMM_WORLD);
    }
    else{
	MPI_Recv(&array,1,MPI_INT,0,1,MPI_COMM_WORLD,&status);
    	bucketSort(array, n); 
    	printf("\n Sorted Numbers\n");
    	for (k = 0; k<i; k++)
        	printf("%d ", array[k]);   
    }
    printf("\n");
    MPI_Finalize();
return 0;
}
