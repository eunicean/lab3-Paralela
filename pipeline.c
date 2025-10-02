#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000

int main(int argc, char** argv) {
    int rank, size;
    int *data = NULL;
    int *buffer;
    int stop = -1;
    MPI_Request request;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk = 1000;
    if (argc > 1) {
        chunk = atoi(argv[1]);
    }

    buffer = malloc(chunk * sizeof(int));

    if (rank == 0) {
        data = malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) data[i] = i;

        start = MPI_Wtime();

        for (int i = 0; i < N; i += chunk) {
            MPI_Send(&data[i], chunk, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }

        // enviar señal de stop
        MPI_Isend(&stop, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);

        end = MPI_Wtime();
        printf("Chunk size: %d, Tiempo total: %f segundos\n", chunk, end - start);

        free(data);
    } else {
        while (1) {
            MPI_Recv(buffer, chunk, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (buffer[0] == -1) break;
            printf("Proceso 1 procesó chunk con inicio %d\n", buffer[0]);
        }
    }

    free(buffer);
    MPI_Finalize();
    return 0;
}
