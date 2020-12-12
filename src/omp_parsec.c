#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) { /* omp_parsec.c */
int conta_secao = 0;
#pragma omp parallel sections
{
    #pragma omp section
	{
 		conta_secao++;
 		/* deve imprimir o número um ou dois */ 
 		printf( "Ordem de execução da sessão %d\n", conta_secao);
 	}
    #pragma omp section
 	{
 		conta_secao++;
		/* deve imprimir o número um ou dois */ 
 		printf( "Ordem de execução da sessão %d\n", conta_secao );
 	}
}
    return(0);
}
