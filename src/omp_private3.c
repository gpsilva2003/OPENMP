#include <stdio.h>
#include <omp.h>
int main( ) {
int valor_original = 10;
#pragma omp parallel num_threads(2) firstprivate(valor_original)
    	{
        	int id_thread = omp_get_thread_num();
        	printf("Thread %d: Valor inicial de valor_original = %d\n", id_thread, valor_original);
        	valor_original = valor_original + id_thread +1;
        	printf("Thread %d: Valor modificado de valor_original = %d\n", id_thread, valor_original);
    	}
	printf("Thread master:  valor final de valor_original = %d  \n", valor_original);
    	return(0);
}

