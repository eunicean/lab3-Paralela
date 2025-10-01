#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, size;
    int number, result = 0;
    MPI_Request request;
    MPI_Status status;
    int flag = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            printf("Este programa requiere exactamente 2 procesos.\n");
        }
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        number = 1234;
        MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Irecv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

        // Mientras esperamos, hacemos operaciones
        for (int i = 0; i < 1000000; i++) {
            result += i % 5;
            MPI_Test(&request, &flag, &status);
            if (flag) break;
        }

        MPI_Wait(&request, &status);
        printf("Proceso 1 recibió %d y calculó resultado=%d\n", number, result);
    }

    MPI_Finalize();
    return 0;
}