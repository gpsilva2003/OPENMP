#include <stdio.h>
#include <omp.h>
void trab1(int tid) {
     printf("Executando trab1 na thread %d \n", tid);
}
void trab2(int tid) {
    printf("Executando trab2 na thread %d \n", tid);
} 
int main(int argc, char *argv[]) { /* omp_single.c  */
#pragma omp parallel num_threads(4)
{
     int tid =  omp_get_thread_num();
    /* Trecho executado por uma única thread, aleatoriamente */
    #pragma omp single
    trab1(tid);
    /* Trecho executado por todas as threads, sempre depois de trab1 */
    trab2(tid);
}
    return(0);
}
