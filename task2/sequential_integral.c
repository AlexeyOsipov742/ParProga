
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

double f(double x) {
    return cos(1.0 / (x * x));
}

int main(int argc, char** argv) {
    int N = 1e8;
    double a = 0.01, b = 1.0;
    double h = (b - a) / N;
    double sum = 0.0;

    clock_t start = clock();
    for (int i = 0; i < N; i++) {
        double x = a + i * h + h / 2;
        sum += f(x);
    }
    clock_t end = clock();

    double integral = sum * h;
    printf("Интеграл ≈ %.10f\n", integral);
    printf("⏱ Время: %.6f сек\n", (double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}

