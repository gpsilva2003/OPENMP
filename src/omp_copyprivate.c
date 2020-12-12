
#include <stdio.h>
#include <stdlib.h>
float 	read_next( ) {
float*	tmp;
float 	return_val;

#pragma omp single copyprivate(tmp)
	{  /* uma thread aloca espaço para a variável */
		tmp = (float*) malloc(sizeof(float));
	}  /* o endereço armazenado no ponteiro é copiado  */
#pragma omp master
	{ /* a thread master faz a leitura do valor */
		scanf("%f", tmp);
	}
#pragma omp barrier
     /* todas as threads recebem o valor lido */
	return_val = *tmp;
#pragma omp barrier
#pragma omp single
	{
		free(tmp);
	}  /* apenas uma thread libera o espaço alocado */
	return return_val;
}

int main(int argc, char *argv[]) { /* omp_copyprivate.c  */
   float a;
   #pragma omp parallel
   {
	   a = read_next( );
	   printf("Valor lido %.3f \n", a);
   }
}