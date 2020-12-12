#include <stdio.h>
#include <omp.h>

void teste(int first, int last)
{
    #pragma omp for schedule(static) ordered
    for (int i = first; i <= last; ++i) {
        // Faz alguma coisa aqui
        if (i % 2)
        {
            #pragma omp ordered
            printf("teste() iteração %d\n", i);
        }
    }
}

void teste2(int iter)
{
    #pragma omp ordered
    printf("teste2() iteração %d\n", iter);
}

int main(int argc, char *argv[]) { /* omp_ordered.c  */
// Faz alguma coisa aqui
int i;
#pragma omp parallel
 {
    teste(1, 8);
    #pragma omp for ordered
    for (i = 0 ; i < 5 ; i++)
        teste2(i);
 }
    return(0);
}
