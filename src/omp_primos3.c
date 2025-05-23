#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int primo (long int n) {
int result = 1;
    #pragma omp for 
	for (long int i = 3; i < (long int)(sqrt(n) + 1); i+=2){
	     if (n%i == 0) {
            result = 0;
            #pragma omp cancel for
         }
        #pragma omp cancellation point for
    }	         
	return result;
}

int main(int argc, char *argv[]) { /* primos_seq.c  */
double t_inicio, t_fim;
long int  n, total=1; /* Acrescenta o dois, que também é primo */

    if (argc < 2) {
        printf("Valor inválido! Entre com o valor do maior inteiro\n");
       	return 0;
    } else {
        n = strtol(argv[1], (char **) NULL, 10);
    }
    t_inicio = omp_get_wtime();
    int result=1;
//#pragma omp parallel for reduction(+:total) schedule(dynamic,100000) num_threads(4) 
#pragma omp parallel num_threads(8) private(t_fim) 
{
    #pragma omp for reduction(+:total) schedule(guided, 10000) firstprivate(result)
    for (long int i = 3; i <= n; i += 2) {
       // int result = 1;
        for (long int j = 3; j < (long int)(sqrt(i) + 1); j+=2)
            if (i%j == 0) {
                result = 0;
                break;
            } 
        if (result == 1) total++;
        else result = 1;
       //  if(primo(i) == 1) total++;
    }
    t_fim = omp_get_wtime();
    printf("Quant. de primos entre 1 e %ld: %ld \n", n, total);
    printf("Tempo de execução: %3.10f Thread = %d\n", t_fim-t_inicio, omp_get_thread_num());
}
    return(0);
}

