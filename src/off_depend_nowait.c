#include <stdio.h>
#include <omp.h>
#define SIZE 5

int main(int argc, char* argv[]) {  /* off_depend_nowait.c */
    int i, a[SIZE], b[SIZE] = {0}, c[SIZE] = {0};
    for (i = 0; i < SIZE; i++) a[i] = i;
    printf("--- Antes ---\n");
    for (i = 0; i < SIZE; i++)
        printf("a[%d]=%d b[%d]=%d c[%d]=%d\n", i, a[i], i, b[i], i, c[i]);

    #pragma omp parallel
    {
        // Região target para b = 2a (com nowait)
        #pragma omp target map(to: a) map(tofrom: b) nowait depend(out: b)
        #pragma omp parallel for
        for (i = 0; i < SIZE; i++)
            b[i] = a[i] * 2;
        // Região target para c = b + 5, esperando b
        #pragma omp target map(to: b) map(tofrom: c) depend(in: b)
        #pragma omp parallel for
        for (i = 0; i < SIZE; i++)
            c[i] = b[i] + 5;
    }
    printf("--- Depois ---\n");
    for (i = 0; i < SIZE; i++)
        printf("a[%d]=%d b[%d]=%d c[%d]=%d\n", i, a[i], i, b[i], i, c[i]);
    return 0;
}
