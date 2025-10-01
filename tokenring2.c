#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    int token = 0;
    int src, dst;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    src = (rank - 1 + size) % size;
    dst = (rank + 1) % size;

    if (rank == 0) {
        token = 100;
    }

    for (int i = 0; i < size; i++) {
        MPI_Sendrecv(&token, 1, MPI_INT, dst, 0,
                     &token, 1, MPI_INT, src, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Proceso %d recibió token %d\n", rank, token);
        token++;
    }

    MPI_Finalize();
    return 0;
}
