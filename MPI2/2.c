#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int i,j,sum=0;
    float x[world_size];
    int a[world_size][world_size], b[world_size];
    for(i=0;i<world_size;i++)
	for(j=0;j<world_size;j++)
		a[i][j] = -1;
    for(i=0;i<world_size;i++)
    {
	b[i] = rand()%10;
    }
    for(i=0;i<world_size;i++)
    {
	for(j=0;j<=i;j++)
	{
		a[i][j] = rand()%10;
	}
    }
    if(world_rank == 0)
    {
	x[world_rank] = b[world_rank]/(a[world_rank][world_rank]*1.0);
	printf("x[%d] = %f\n",world_rank,x[world_rank]);
        MPI_Send(&x[world_rank], 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
    }
    else if(world_rank > 0 && world_rank < (world_size-1))
    {
	for(j=0;j<world_rank;j++)
	{
		MPI_Recv(&x[j], 1, MPI_FLOAT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Send(&x[j], 1, MPI_FLOAT, world_rank + 1, 0, MPI_COMM_WORLD);
		sum = sum + a[world_rank][j]*x[j];
	}
	x[world_rank] = (b[world_rank] - sum)/(a[world_rank][world_rank]*1.0);
	printf("x[%d] = %f\n",world_rank,x[world_rank]);
	MPI_Send(&x[world_rank], 1, MPI_FLOAT, world_rank+1, 0, MPI_COMM_WORLD);
		
    }
    else
    {
	for(j=0;j<world_rank;j++)
	{
		MPI_Recv(&x[j], 1, MPI_FLOAT, world_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		sum = sum + a[world_rank][j]*x[j];
	}
	x[world_rank] = (b[world_rank] - sum)/(a[world_rank][world_rank]*1.0);
	printf("x[%d] = %f\n",world_rank,x[world_rank]);
    }
   MPI_Finalize();
}

