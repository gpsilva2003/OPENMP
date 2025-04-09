#include <stdio.h>
#include <omp.h>
    
int main(int argc, char *argv[]) { /* omp_for.c  */
    #pragma omp parallel for num_threads(4)
    for (int i = 0; i < 18; ++i) {
	printf("Iteração %2d executada pela thread %d \n", i, omp_get_thread_num());
    }
    return(0);
}
