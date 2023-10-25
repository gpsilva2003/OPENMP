#include <stdio.h>
#include <omp.h>
#define TAM 10
int main(int argc, char *argv[]) { /* omp_questao12  */

int i, x=1, n=TAM;
int y[TAM];
#pragma omp parallel for firstprivate(x) ordered num_threads(4)
	for(i = 0; i < n; i++){
        #pragma omp ordered
		{
		y[i] = x + i;
		x = i;
		}
	}
	for(int j = 0; j < n; j++)
		printf("%d ", y[j]); 
	printf("\n " ); 
	printf("x = %d ", x); 
	printf("i = %d ", i); 
    return 0;
}
