#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int message = 0;

    MPI_Init(&argc, &argv);                         // Инициализация MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);           // Получаем ранг текущего процесса
    MPI_Comm_size(MPI_COMM_WORLD, &size);           // Получаем количество процессов

    if (rank == 0) {
        message = 0;
        printf("Start: rank %d, message = %d\n", rank, message);
        MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);  // Отправка следующему
        MPI_Recv(&message, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Получение от последнего
        printf("End:   rank %d, final message = %d\n", rank, message);
    } else {
        MPI_Recv(&message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Получение от предыдущего
        message += 1;
        printf("rank %d received message = %d\n", rank, message);
        int next = (rank + 1) % size;
        MPI_Send(&message, 1, MPI_INT, next, 0, MPI_COMM_WORLD); // Отправка следующему
    }

    MPI_Finalize();
    return 0;
}
