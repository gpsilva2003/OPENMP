#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

float 	leia_dado( ) {
float*	tmp;
float 	return_val;
	#pragma omp single copyprivate(tmp) /* uma thread aloca espaço para a variável */
	{  
		tmp = (float*) malloc(sizeof(float));
	}  								/* o endereço armazenado no ponteiro é copiado  */
	#pragma omp master /* a thread master faz a leitura do valor */
	{ 
		printf("Digite um valor: ");
		fflush(stdout);
		scanf("%f", tmp);
	}
	#pragma omp barrier   /* todas as threads esperam a leitura */
	return_val = *tmp;   /* todas as threads recebem o valor lido */
	#pragma omp barrier   /* todas as threads esperam antes da liberação do espaço */
	#pragma omp single   /* apenas uma thread libera o espaço alocado */
	{
		free(tmp);
	}  
	return return_val;
}

int main(int argc, char *argv[]) { /* omp_copyprivate.c  */
   #pragma omp parallel
   {
	   float a = leia_dado( );
	   printf("Valor lido %.3f \n", a);
   }
}