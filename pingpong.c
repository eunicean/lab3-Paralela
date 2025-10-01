#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    const int N = 10000000;
    int msg = 11100000;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        start = MPI_Wtime();
        for (int i = 0; i < N; i++) {
            MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        end = MPI_Wtime();
        printf("Tiempo total: %f segundos\n", end - start);
    } else {
        for (int i = 0; i < N; i++) {
            MPI_Recv(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}