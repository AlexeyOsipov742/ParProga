#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int id;
    int num_threads;
    double a, b;
    double epsilon;
    double result;
} ThreadData;

double f(double x) {
    return sin(1.0 / (x * x));
}

void* integrate(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double a_local = data->a + (data->b - data->a) * data->id / data->num_threads;
    double b_local = data->a + (data->b - data->a) * (data->id + 1) / data->num_threads;
    double h = sqrt(12.0 * data->epsilon / (b_local - a_local));
    int n = (int)((b_local - a_local) / h);
    h = (b_local - a_local) / n;

    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double x1 = a_local + i * h;
        double x2 = x1 + h;
        sum += (f(x1) + f(x2)) * h / 2.0;
    }

    data->result = sum;
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_threads> <epsilon>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    double epsilon = atof(argv[2]);
    double a = 0.01, b = 1.0;

    pthread_t threads[num_threads];
    ThreadData data[num_threads];

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_threads; i++) {
        data[i] = (ThreadData){.id = i, .num_threads = num_threads, .a = a, .b = b, .epsilon = epsilon, .result = 0};
        pthread_create(&threads[i], NULL, integrate, &data[i]);
    }

    double total = 0.0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total += data[i].result;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Result: %.10f\n", total);
    printf("np=%d eps=%.1e time=%.6f\n", num_threads, epsilon, time);
    return 0;
}

