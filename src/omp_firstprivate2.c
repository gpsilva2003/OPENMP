#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) { /* omp_firstprivate.c  */
int a=1000, n = 5;
    omp_set_num_threads(3);
    #pragma omp parallel firstprivate(a)
    {
    int tid = omp_get_thread_num();
    printf("Thread %d recebe o valor inicial de a = %d \n", tid,a);
    	#pragma omp for  
    	for (int i = 0; i < n; i++) {
        	a += i;
    	} 
    printf("Thread %d tem o valor final de a = %d \n", tid,a);
    }
    printf("Thread master tem um valor de a = %d  \n", a);
    return(0);
}
