/*
 * Problema 3 - Caso 3 do Teorema Mestre
 * Recorrencia: T(n) = 2T(n/2) + Theta(n^2)   (a = 2, b = 2, f(n) = n^2)
 * Algoritmo: conta, no trecho v[ini..fim), os pares (i, j), i < j, cuja soma
 * e multipla de 3. A contagem do trecho inteiro e feita por dois lacos
 * aninhados (Theta(n^2)); depois o algoritmo recursa nas duas metades,
 * acumulando o trabalho (o resultado devolvido e a soma das contagens de
 * todos os niveis, usada apenas para impedir que o compilador elimine o
 * calculo).
 * Uso: ./p3 <n_min> <n_max> <repeticoes>   (n dobra a cada experimento)
 * Saida: acrescenta linhas em resultados.csv
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static unsigned long long operacoes;  /* pares examinados */

static long long conta(const int *v, int ini, int fim) {
    int n = fim - ini;
    if (n <= 1) return 0;                          /* caso base */
    long long cnt = 0;
    for (int i = ini; i < fim; i++)                /* trabalho f(n) = Theta(n^2) */
        for (int j = i + 1; j < fim; j++) {
            if ((v[i] + v[j]) % 3 == 0) cnt++;
            operacoes++;
        }
    int m = ini + n / 2;
    return cnt + conta(v, ini, m) + conta(v, m, fim);  /* 2 chamadas de n/2 */
}

static double agora(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "uso: %s n_min n_max reps\n", argv[0]); return 1; }
    int nmin = atoi(argv[1]), nmax = atoi(argv[2]), reps = atoi(argv[3]);
    FILE *f = fopen("resultados.csv", "a");
    if (!f) { perror("resultados.csv"); return 1; }
    srand(42);
    volatile long long sorvedouro = 0;
    for (int n = nmin; n <= nmax; n *= 2) {
        int *v = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) v[i] = rand();
        for (int r = 1; r <= reps; r++) {
            operacoes = 0;
            double t0 = agora();
            sorvedouro += conta(v, 0, n);
            double t1 = agora();
            fprintf(f, "3,%d,%d,%.9f,%llu\n", n, r, t1 - t0, operacoes);
        }
        free(v);
    }
    fclose(f);
    return 0;
}
