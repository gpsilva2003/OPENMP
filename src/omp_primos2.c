#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int primo (long int n) {
	long int i;
       
	for (i = 3; i < (long int)(sqrt(n) + 1); i+=2) 
	     if (n%i == 0) 
	         return 0;
	return 1;
}

int main(int argc, char *argv[]) { /* primos_seq.c  */
double t_inicio, t_fim[8];
long int i, n, total=0;

    if (argc < 2) {
        printf("Valor inválido! Entre com o valor do maior inteiro\n");
       	return 0;
    } else {
        n = strtol(argv[1], (char **) NULL, 10);
    }
    t_inicio = omp_get_wtime();
//#pragma omp parallel for reduction(+:total) schedule(dynamic,100000) num_threads(4) 
#pragma omp parallel num_threads(8)
{
	int tid = omp_get_thread_num();
# pragma omp for reduction(+:total) schedule(static) 
    for (i = 3; i <= n; i += 2) 	{
         if(primo(i) == 1) total++;
    t_fim[tid] = omp_get_wtime();
    }
		
    total += 1;    /* Acrescenta o dois, que também é primo */
    printf("Quant. de primos entre 1 e %ld: %ld \n", n, total);
    printf("Tempo de execução: %3.10f Thread = %d\n", t_fim[tid]-t_inicio, tid);
}
    return(0);
}

