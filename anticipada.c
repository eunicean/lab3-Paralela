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
    
    int message_size = 1; 
    if (argc > 1) {
        message_size = atoi(argv[1]);
    }

    int *buffer = malloc(message_size * sizeof(int));

    if (rank == 0) {
        // llenar el buffer con datos
        for (int i = 0; i < message_size; i++) {
            buffer[i] = 1234 + i;
        }
        MPI_Send(buffer, message_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Irecv(buffer, message_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);

        for (int i = 0; i < 1000000; i++) {
            result += i % 5;
            MPI_Test(&request, &flag, &status);
            if (flag) break;
        }

        MPI_Wait(&request, &status);
        printf("Proceso 1 recibió %d enteros, primero=%d, resultado=%d\n",
               message_size, buffer[0], result);
    }

    free(buffer);
    MPI_Finalize();
    return 0;
}
