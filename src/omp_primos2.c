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
double t_inicio, t_fim;
long int i,j, primo, n, total=0;

    if (argc < 2) {
        printf("Valor inválido! Entre com o valor do maior inteiro\n");
       	return 0;
    } else {
        n = strtol(argv[1], (char **) NULL, 10);
    }
    t_inicio = omp_get_wtime();
#pragma omp parallel for reduction(+:total) private(j,primo) num_threads(4)
    for (i = 3; i <= n; i += 2) { 	
//	for (j = 3, primo=1; j < (long int)(sqrt(i) + 1); j+=2) 
	for (j = 3, primo=1; j*j < i; j+=2) 
	     if (i%j == 0) {
	     	primo = 0;
	        break;
             }
	total+=primo;
    }
		
    total += 1;    /* Acrescenta o dois, que também é primo */
    t_fim = omp_get_wtime();
    printf("Quant. de primos entre 1 e %ld: %ld \n", n, total);
    printf("Tempo de execução: %f \n", t_fim-t_inicio);
    return(0);
}
