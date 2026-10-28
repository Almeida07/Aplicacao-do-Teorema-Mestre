/*
 * Problema 2 - Caso 2 do Teorema Mestre
 * Recorrencia: T(n) = 3T(n/3) + Theta(n)   (a = 3, b = 3, f(n) = n)
 * Algoritmo: merge sort em 3 partes. O vetor e dividido em tres tercos,
 * cada terco e ordenado recursivamente e a intercalacao dos tres trechos
 * ordenados custa Theta(n).
 * Uso: ./p2 <n_min> <n_max> <repeticoes>   (n dobra a cada experimento)
 * Saida: acrescenta linhas em resultados.csv
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static unsigned long long operacoes; /* comparacoes + copias */

/* Intercala v[ini..m1), v[m1..m2), v[m2..fim) (ja ordenados) usando aux. */
static void intercala3(int *v, int *aux, int ini, int m1, int m2, int fim) {
    int i = ini, j = m1, k = m2, p = ini;
    while (i < m1 || j < m2 || k < fim) {
        int escolhe = -1;                 /* 0, 1 ou 2: trecho do menor elemento */
        if (i < m1) escolhe = 0;
        if (j < m2 && (escolhe < 0 || (operacoes++, v[j] < v[i]))) escolhe = 1;
        if (k < fim) {
            if (escolhe < 0) escolhe = 2;
            else { operacoes++; if (v[k] < v[escolhe == 0 ? i : j]) escolhe = 2; }
        }
        aux[p++] = (escolhe == 0) ? v[i++] : (escolhe == 1) ? v[j++] : v[k++];
        operacoes++;                      /* copia */
    }
    for (int x = ini; x < fim; x++) { v[x] = aux[x]; operacoes++; }
}

/* Ordena v[ini..fim) dividindo em 3 partes. */
static void mergesort3(int *v, int *aux, int ini, int fim) {
    int n = fim - ini;
    if (n <= 1) return;                   /* caso base */
    int m1 = ini + n / 3, m2 = ini + 2 * (n / 3);
    if (m2 == m1) m2 = m1 + 1;            /* n = 2: partes de tamanho 1 e 1 */
    mergesort3(v, aux, ini, m1);
    mergesort3(v, aux, m1, m2);
    mergesort3(v, aux, m2, fim);
    intercala3(v, aux, ini, m1, m2, fim);
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
        int *v = malloc(n * sizeof(int)), *aux = malloc(n * sizeof(int));
        for (int r = 1; r <= reps; r++) {
            for (int i = 0; i < n; i++) v[i] = rand();
            operacoes = 0;
            double t0 = agora();
            mergesort3(v, aux, 0, n);
            double t1 = agora();
            for (int i = 1; i < n; i++)   /* verificacao de corretude */
                if (v[i - 1] > v[i]) { fprintf(stderr, "ERRO: nao ordenado\n"); return 2; }
            fprintf(f, "2,%d,%d,%.9f,%llu\n", n, r, t1 - t0, operacoes);
        }
        free(v); free(aux);
    }
    fclose(f);
    return 0;
}
