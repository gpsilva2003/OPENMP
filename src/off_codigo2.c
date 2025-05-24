
#include <stdio.h>
#include <omp.h>
#define SIZE 5
int main() {    /* off_codigo2 */
    int i, a[SIZE], b[SIZE]={0}, c[SIZE]={0};
    // Valor inicial do vetor 'a' no hospedeiro
    for (i = 0; i < SIZE; i++) 
        a[i] = i;
    printf("--- Antes das regiões target ---\n");
    for (i = 0; i < SIZE; i++)
        printf("a[%d] = %d, b[%d] = %d, c[%d] = %d\n", i, a[i], i, b[i], i, c[i]);
    printf("\n");
    // Primeira região target: Calcula b = f(a)
    #pragma omp target map(to: a) map(tofrom: b)
        #pragma omp parallel for
        for (i = 0; i < SIZE; i++) 
            b[i] = a[i] * 2;
    // Segunda região target: Calcula c = g(b), dependendo dos resultados de b
    #pragma omp target map(to: b) map(tofrom: c) depend(inout: b)
        #pragma omp parallel for
        for (i = 0; i < SIZE; i++) 
            c[i] = b[i] + 5;
    printf("--- Após as regiões target ---\n");
    for (i = 0; i < SIZE; i++) {
        printf("a[%d] = %d, b[%d] = %d, c[%d] = %d\n", i, a[i], i, b[i], i, c[i]);
    }
    return 0;
}