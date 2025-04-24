#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int rank, size, N;
    double local_sum = 0.0, total_sum = 0.0;

    MPI_Init(&argc, &argv);                 // Инициализация MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // Получение ранга процесса
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Получение общего числа процессов

    if (rank == 0) {
        if (argc < 2) {
            printf("Usage: %s N\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        N = atoi(argv[1]);
    }

    // Рассылка N всем процессам
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Каждый процесс считает свою часть суммы
    for (int i = rank + 1; i <= N; i += size) {
        local_sum += 1.0 / i;
    }

    // Сбор всех локальных сумм в total_sum на процессе 0
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total sum of 1/n for n=1..%d is %.10f\n", N, total_sum);
    }

    MPI_Finalize(); // Завершение MPI
    return 0;
}
