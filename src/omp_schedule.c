#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) { /* omp_schedule.c  */
int n = 20;
    
    printf("Cláusula static SEM o parâmetro tamanho \n");
#pragma omp parallel for schedule (static) num_threads(4)
    for (int i = 0; i < n; ++i) {
	    printf("tid =%d iteracao = %d \n", omp_get_thread_num(),i);
    }

    printf("Cláusula static COM o parâmetro tamanho \n");
#pragma omp parallel for schedule (static,3) num_threads(4)
    for (int i = 0; i < n; ++i) {
	    printf("tid =%d iteracao = %d \n", omp_get_thread_num(),i);
    }

    return 0;
}
