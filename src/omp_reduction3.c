#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) { /* omp_reduction.c  */
int a = 10, b = 3;
    omp_set_num_threads(4);
    #pragma omp parallel default(none) reduction(+:a) firstprivate(b)
    {
     int tid = omp_get_thread_num();
     a = tid + b;
    printf("Valor de 'a' na thread: %d é igual %d\n", tid, a);
    }
    printf("Valor de 'a' depois da região paralela: %d\n",a);
    return(0);
}
