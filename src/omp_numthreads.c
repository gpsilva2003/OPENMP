#include <stdio.h>  
#include <omp.h>  

int main(int argc, char *argv[]) {  /* omp_numthreads.c  */
    // Verifica se o código está sendo executado fora de uma região paralela
    printf("Fora da região paralela = %d\n", omp_in_parallel());
    // Obtém e imprime o número de processadores disponíveis no sistema
    printf("Número de processadores = %d\n", omp_get_num_procs());
    // Inicia uma região paralela com 3 threads
    #pragma omp parallel num_threads(3)  
    {  
        // Obtém o ID da thread atual
        int tid = omp_get_thread_num(); 
        // Imprime uma mensagem da thread atual
        printf("Olá da thread %d\n", tid);
        // Verifica se o código está sendo executado dentro de uma região paralela
        printf("Dentro da região paralela = %d\n", omp_in_parallel()); 
    }
    return(0);
}