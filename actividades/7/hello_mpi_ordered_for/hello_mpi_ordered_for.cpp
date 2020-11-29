#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[])
{

    int my_id, num_processes, message;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    for (int i = 0, j = 1; i < num_processes, j < num_processes; ++i, ++j)
    {
        if (my_id == 0)
        {
            MPI_Ssend(&message, 1, MPI_INT, i+1 /*dest*/, i, MPI_COMM_WORLD);
        }
        else
        {
            if (my_id == i)
            {
                MPI_Recv(&message, 1, MPI_INT, i /*source*/, i, MPI_COMM_WORLD, &status);
                MPI_Ssend(&message, 1, MPI_INT, i+1 /*dest*/, i, MPI_COMM_WORLD);
            }
        }
    }

    cout << "Hello. I am process: " << my_id << ". Total processes: " << num_processes << endl;

    MPI_Finalize();
    return 0;
}