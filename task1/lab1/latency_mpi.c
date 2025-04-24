#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank;
    const int tag = 0;
    const int N = 1000000; // Количество повторов для средней задержки
    char msg = 'x';
    double t_start, t_end, latency_sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < N; ++i) {
        if (rank == 0) {
            t_start = MPI_Wtime();
            MPI_Send(&msg, 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
            MPI_Recv(&msg, 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            t_end = MPI_Wtime();
            latency_sum += (t_end - t_start);
        } else if (rank == 1) {
            MPI_Recv(&msg, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&msg, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        double avg_latency = latency_sum / N / 2; // Делим на 2 (туда-обратно)
        printf("Average latency: %e seconds\n", avg_latency);
    }

    MPI_Finalize();
    return 0;
}
