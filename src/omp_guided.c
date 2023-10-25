#include <stdio.h>
#include <omp.h>

void saxpy(int n, float a, float * restrict x, float * restrict y)
{
    #pragma omp parallel for
    for (int i = 0; i < n; ++i)
        y[i] = a*x[i] + y[i];
}
    
int main(int argc, char *argv[]) { /* omp_schedule  */
int n = 40;
    
    printf("Guided SEM chuncksize \n");
#pragma omp parallel for schedule (guided) num_threads(4)
    for (int i = 0; i < n; ++i) {
	    printf("tid =%d iteracao = %d \n", omp_get_thread_num(),i);
    }

    printf("Guided COM chuncksize \n");

#pragma omp parallel for schedule (guided,3) num_threads(4)
    for (int i = 0; i < n; ++i) {
	    printf("tid =%d iteracao = %d \n", omp_get_thread_num(),i);
    }

    return 0;
}
