#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define M 100
#define K 100
#define a 1.0
#define L 1.0
#define T 1.0

double phi(double x) {
    return (x * x) / 2;
}

double psi(double t) {
    return (t * t) / 2;
}

double f(double t, double x) {
    return 0;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double h = L / M;
    double tau = T / K;
    double CFL = a * tau / h;
    if (rank == 0 && CFL > 1.0) {
        printf("Ошибка: нарушено условие устойчивости CFL = %.4f > 1.0\n", CFL);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int base = M / size;
    int extra = M % size;
    int local_M = base + (rank < extra ? 1 : 0);
    int offset = rank * base + (rank < extra ? rank : extra);
    int stride = local_M + 2;

    double *u = malloc(sizeof(double) * (K + 1) * stride);
    if (!u) {
        printf("Ошибка выделения памяти на ранге %d\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Инициализация t=0
    for (int m = 1; m <= local_M; m++) {
        int global_m = offset + m - 1;
        double x = global_m * h;
        u[0 * stride + m] = phi(x);
    }
    u[0 * stride + 0] = 0.0;
    u[0 * stride + local_M + 1] = 0.0;
    
    const char* tau_env = getenv("TAU");
    if (tau_env) tau = atof(tau_env);

    double t_start = MPI_Wtime();

    // Основной цикл
    for (int k = 0; k < K; k++) {
        double *cur = &u[k * stride];
        double *next = &u[(k + 1) * stride];
        double t = k * tau;

        // Обмен границ
        if (rank > 0) {
            MPI_Sendrecv(&cur[1], 1, MPI_DOUBLE, rank - 1, 0,
                         &cur[0], 1, MPI_DOUBLE, rank - 1, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            cur[0] = psi(t);
        }

        if (rank < size - 1) {
            MPI_Sendrecv(&cur[local_M], 1, MPI_DOUBLE, rank + 1, 0,
                         &cur[local_M + 1], 1, MPI_DOUBLE, rank + 1, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            cur[local_M + 1] = 0.0;  // фиктивная граница
        }

        // Расчёт следующего слоя
        for (int m = 1; m <= local_M; m++) {
            int global_m = offset + m - 1;
            double x = global_m * h;
            next[m] = cur[m]
                    - CFL * (cur[m] - cur[m - 1])
                    + tau * f(t, x);
        }

        // Левый край — физическое граничное условие
        if (rank == 0)
            next[1] = psi((k + 1) * tau);

        // ⚠ УБРАНО: никакой обнулённой правой границы!
    }

    // Сбор данных
    int *recvcounts = NULL, *displs = NULL;
    if (rank == 0) {
        recvcounts = malloc(size * sizeof(int));
        displs = malloc(size * sizeof(int));
    }

    int *local_counts = malloc(sizeof(int));
    *local_counts = local_M;

    MPI_Gather(local_counts, 1, MPI_INT,
               recvcounts, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        displs[0] = 0;
        for (int i = 1; i < size; i++)
            displs[i] = displs[i - 1] + recvcounts[i - 1];
    }

    double *full_data = NULL;
    if (rank == 0)
        full_data = malloc(sizeof(double) * (K + 1) * M);

    double *sendbuf = malloc(local_M * sizeof(double));
    for (int k = 0; k <= K; k++) {
        double *row = &u[k * stride];
        for (int m = 0; m < local_M; m++)
            sendbuf[m] = row[m + 1];

        MPI_Gatherv(sendbuf, local_M, MPI_DOUBLE,
                    full_data ? &full_data[k * M] : NULL,
                    recvcounts, displs, MPI_DOUBLE,
                    0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        FILE *f = fopen("solution_paralel.txt", "w");
        for (int k = 0; k <= K; k++) {
            for (int m = 0; m < M; m++)
                fprintf(f, "%.6f ", full_data[k * M + m]);
            fprintf(f, "\n");
        }
        fclose(f);
        printf("✅ Решение сохранено в solution_paralel.txt\n");
        free(full_data);
        free(recvcounts);
        free(displs);
    }

    free(sendbuf);
    free(u);
    free(local_counts);
    double t_end = MPI_Wtime();
    if (rank == 0) {
        FILE *f = fopen("timing_log.txt", "a");
        fprintf(f, "np=%d tau=%.6f time=%.6f\n", size, tau, t_end - t_start);
        fclose(f);
    }
    MPI_Finalize();
    return 0;
}
