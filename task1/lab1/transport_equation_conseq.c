#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define X 1.0
#define T 1.0
#define M 100   // шагов по x
#define K 100   // шагов по t

double phi(double x) {
    return (x * x) / 2;  // начальное условие
}

double psi(double t) {
    return (t * t) / 2;  // граничное условие
}

double f(double t, double x) {
    return 0;  // правая часть
}

int main() {
    double h = X / M;
    double tau = T / K;
    double a = 1.0;

    double CFL = a * tau / h;
    if (CFL > 1.0) {
        printf("Ошибка: нарушено условие устойчивости CFL = %.2f > 1\n", CFL);
        return 1;
    }

    double u[K+1][M+1];

    // начальные условия: t = 0
    for (int m = 0; m <= M; m++) {
        double x = m * h;
        u[0][m] = phi(x);
    }

    // граничные условия: x = 0
    for (int k = 1; k <= K; k++) {
        double t = k * tau;
        u[k][0] = psi(t);
    }

    // основной цикл по времени и пространству (схема "левый уголок")
    for (int k = 0; k < K; k++) {
        double t = k * tau;
        for (int m = 1; m <= M; m++) {
            double x = m * h;
            u[k+1][m] = u[k][m]
                - CFL * (u[k][m] - u[k][m-1])
                + tau * f(t, x);
        }
    }

    // запись результата в файл
    FILE *out = fopen("solution_conseq.txt", "w");
    for (int k = 0; k <= K; k++) {
        for (int m = 0; m <= M; m++) {
            fprintf(out, "%.5f ", u[k][m]);
        }
        fprintf(out, "\n");
    }
    fclose(out);

    printf("Решение сохранено в файл solution_conseq.txt\n");
    return 0;
}
