#include <stdio.h>
#include <stdlib.h>
#define TRUE  1
#define FALSE 0
#include <omp.h>

int main(int argc, char *argv[]) { /* omp_firstprivate.c  */
int *a;
int n = 2; 
int nthreads, vlen, indice, offset = 4, i, tid;

    omp_set_num_threads(3);
    indice = offset;

/* Prepara os parâmetros para a computação e aloca memória */
#pragma omp parallel firstprivate(indice) shared(a,n,nthreads)
{
    tid = omp_get_thread_num();
    if (tid == 0) {
    	nthreads = omp_get_num_threads();
    	vlen = indice + n*nthreads;
    	a = (int *) malloc(vlen*sizeof(int));
    }
} /*-- Final de região paralela --*/
    for (i = 0; i < vlen; i++) 
        a[i] = -i-1;
/* Cada thread acessa o vetor com a variável indice */  
   printf("Comprimento do segmento por thread é %d\n", n);
   printf("O offset do vetor a é %d\n",indice);
#pragma omp parallel default(none) firstprivate(indice) private(i,tid) shared(n,a)
{
    tid = omp_get_thread_num();
    indice += n*tid;
    for (i = indice; i < indice+n; i++)
         a[i] = tid + 1;
} /*-- Final da região paralela --*/
   printf("Depois da região paralela:\n");
   for (i = 0; i < vlen; i++)
        printf("a[%d] = %d\n",i,a[i]);
   free(a);
   return(0);
}