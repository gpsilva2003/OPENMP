#include <stdio.h>
#include <omp.h>
#include <unistd.h>

void trab1() {
     printf("Trab1. executado pela thread %d \n", omp_get_thread_num());
     sleep(1);  /* Espera um segundo*/
}

void trab2() {
    printf("Trab2. executado pela thread %d \n", omp_get_thread_num());
}
    
int main(int argc, char *argv[]) { /* omp_single.c  */
#pragma omp parallel num_threads(4)   // Região paralela 
{
    #pragma omp single    // Apenas uma thread executa a primeira tarefa
    {
    printf("Começando Trab1.\n");
    trab1();
    printf("Terminando Trab1.\n");
    }
    #pragma omp single   // Essa segunda tarefa só será executada após a primeira
    {
    printf("Começando Trab2.\n");
    trab2();
    printf("Terminando Trab2.\n");

}
return(0);
}
