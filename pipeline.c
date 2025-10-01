#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define CHUNK 1000
#define N 10000  // tamaño total del arreglo

int main(int argc, char** argv) {
    int rank, size;
    int data[N];
    int buffer[CHUNK];
    int stop = -1;
    MPI_Request request;

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
        // llenar datos
        for (int i = 0; i < N; i++) data[i] = i;

        for (int i = 0; i < N; i += CHUNK) {
            MPI_Send(&data[i], CHUNK, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }

        // enviar stop
        MPI_Isend(&stop, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
    } else {
        while (1) {
            MPI_Recv(buffer, CHUNK, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (buffer[0] == -1) break; // señal de stop
            // procesar
            printf("Proceso 1 procesó chunk con inicio %d\n", buffer[0]);
        }
    }

    MPI_Finalize();
    return 0;
}