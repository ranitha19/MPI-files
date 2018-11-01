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
    {
	b[i] = rand()%10;
    }
    for(i=0;i<world_size;i++)
    {
	for(j=0;j<i;j++)
	{
		a[i][j] = rand()%10;
	}
    }
    if(world_rank == 0)
    {
	x[0] = (float)b[0]/a[0][0];
	printf("x[%d] = %f ",world_rank,x[world_rank]);
        MPI_Send(&x[0], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else if(world_rank > 0 && world_rank < world_size-1)
    {
	for(j=0;j<world_size;j++)
	{
		int r1 = world_rank - 1;
		int r2 = world_rank + 1;
		MPI_Recv(&x[j], 1, MPI_INT, r1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Send(&x[j], 1, MPI_INT, r2, 0, MPI_COMM_WORLD);
		sum = sum + a[world_rank][j];
	}
	x[world_rank] = (float)(b[world_rank] - sum)/a[world_rank][world_rank];
	printf("x[%d] = %f ",world_rank,x[world_rank]);
	int r = world_rank+1;
	MPI_Send(&x[world_rank], 1, MPI_INT, r, 0, MPI_COMM_WORLD);
		
    }
   MPI_Finalize();
}

