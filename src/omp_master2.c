#include <stdio.h>
#include <omp.h>
#include <unistd.h>
void trab1() {
     printf("Trab1 executado pela thread %d\n", omp_get_thread_num());
     sleep(1);
}
void trab2() {
     printf("Trab2 executado pela thread %d\n", omp_get_thread_num());
}
int main(int argc, char *argv[]) { /* omp_master2.c  */
    #pragma omp parallel num_threads(8)
    {
        #pragma omp master // Executado apenas pela thread 0 
        {
    	    printf("Começando Trab1.\n");
    	    trab1();
    	    printf("Terminando Trab1.\n");
        }
        #pragma omp single // Executado por quaisquer threads em qualquer ordem
        {
    	    printf("Começando Trab2.\n");
    	    trab2();
    	    printf("Terminando Trab2.\n");
        }
    }
    return(0);
}
