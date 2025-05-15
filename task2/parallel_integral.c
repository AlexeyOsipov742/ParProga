#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

double f(double x) {
    return cos(1.0 / (x * x));
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 0.01;
    double b = 1.0;
    double h = 1e-5;  // значение по умолчанию

    // Переопределение шага через переменную окружения
    const char* h_env = getenv("H_STEP");
    if (h_env) {
        double parsed = atof(h_env);
        if (parsed > 0 && parsed < (b - a)) {
            h = parsed;
        }
    }

    long long N = (long long)((b - a) / h);
    double local_sum = 0.0;

    double t_start = MPI_Wtime();

    // Распределение по кругу
    for (long long i = rank; i < N; i += size) {
        double x = a + i * h + h / 2.0;
        local_sum += f(x);
    }

    double total_sum = 0.0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double t_end = MPI_Wtime();

    if (rank == 0) {
        double integral = total_sum * h;
        printf("Интеграл ≈ %.10f\n", integral);
        printf("np = %d, h = %.8f, время = %.6f сек\n", size, h, t_end - t_start);

        FILE *log = fopen("timing_log.txt", "a");
        if (log) {
            fprintf(log, "np=%d h=%.8f time=%.6f\n", size, h, t_end - t_start);
            fclose(log);
        }
    }

    MPI_Finalize();
    return 0;
}

