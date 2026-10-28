/*
 * Problema 1 - Caso 1 do Teorema Mestre
 * Recorrencia: T(n) = 4T(n/2) + Theta(n)   (a = 4, b = 2, f(n) = n)
 * Algoritmo: multiplicacao recursiva de dois polinomios com n coeficientes
 * (n potencia de 2). Cada polinomio e dividido em duas metades
 *   A = A0 + A1*x^(n/2),  B = B0 + B1*x^(n/2)
 * e calculam-se os 4 subprodutos A0B0, A0B1, A1B0, A1B1 (4 chamadas de
 * tamanho n/2). A combinacao dos resultados custa Theta(n).
 * Uso: ./p1 <n_min> <n_max> <repeticoes>   (n dobra a cada experimento)
 * Saida: acrescenta linhas em resultados.csv
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned long long u64;
static u64 operacoes; /* contador de operacoes elementares (mult. e somas) */

/* Multiplica a[0..n-1] por b[0..n-1]; escreve em c[0..2n-2] (c ja zerado). */
static void multiplica(const u64 *a, const u64 *b, int n, u64 *c) {
    if (n == 1) {                       /* caso base: 1 multiplicacao */
        c[0] = a[0] * b[0];
        operacoes++;
        return;
    }
    int h = n / 2;
    u64 *t = calloc(2 * h - 1, sizeof(u64));   /* produto parcial */
    /* os 4 subprodutos, cada um de tamanho n/2 */
    const u64 *ap[2] = { a, a + h }, *bp[2] = { b, b + h };
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            memset(t, 0, (2 * h - 1) * sizeof(u64));
            multiplica(ap[i], bp[j], h, t);
            /* combinacao: soma t em c a partir da posicao (i+j)*h -> Theta(n) */
            for (int k = 0; k < 2 * h - 1; k++) {
                c[(i + j) * h + k] += t[k];
                operacoes++;
            }
        }
    free(t);
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
    for (int n = nmin; n <= nmax; n *= 2) {
        u64 *a = malloc(n * sizeof(u64)), *b = malloc(n * sizeof(u64));
        u64 *c = calloc(2 * n - 1, sizeof(u64));
        for (int i = 0; i < n; i++) { a[i] = rand() % 10; b[i] = rand() % 10; }
        for (int r = 1; r <= reps; r++) {
            memset(c, 0, (2 * n - 1) * sizeof(u64));
            operacoes = 0;
            double t0 = agora();
            multiplica(a, b, n, c);
            double t1 = agora();
            fprintf(f, "1,%d,%d,%.9f,%llu\n", n, r, t1 - t0, operacoes);
        }
        free(a); free(b); free(c);
    }
    fclose(f);
    return 0;
}
