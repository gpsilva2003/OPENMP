#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define SOMA_INICIAL 0

int main(int argc, char *argv[]) { /* omp_reduction.c  */
int i, n = 25;
int soma, a[n];
int ref = SOMA_INICIAL + ((n-1)*n)/2;

    omp_set_num_threads(4);
    for (i = 0; i < n; i++)
        a[i] = i;
    soma = SOMA_INICIAL;
    printf("Valor da soma antes da região paralela: %d \n",soma);
    #pragma omp parallel for default(none) shared(n,a) reduction(+:soma)
    for (i = 0; i < n; i++)
        soma += a[i];
/*-- Fim da redução paralela --*/
    printf("Valor da soma depois da região paralela: %d\n",soma);
    printf("Verificação do resultado: soma = %d (deveria ser %d)\n",soma,ref);
    return(0);
}
