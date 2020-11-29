#include "mpi.h"
#include <math.h>

int main(int argc, char *argv[])
{
    int done = 0;
    int n; 
    int myid; 
    int numprocs; 
    int k;
    double sum_global, pi, sum, factor;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    while (!done)
    {
        if (myid == 0)
        {
            printf("Enter the value of n: (0 quits) ");
            scanf("%d", &n);
        }
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (n == 0)
            break;

        factor = 1.0;
        sum = 0.0;
        for (k = myid; k < n; k += numprocs)
        {
            if (k%2 == 0) {
                factor = 1.0;
            } else {
                factor = -1.0;
            }
            sum += factor/(2*k+1);
        }

        MPI_Reduce(&sum, &sum_global, 1, MPI_DOUBLE, MPI_SUM, 0,
                   MPI_COMM_WORLD);

        if (myid == 0) {
            pi = 4*sum_global;
            printf("pi is approximately %.16f\n", pi);
        }
    }
    MPI_Finalize();
    return 0;
}
