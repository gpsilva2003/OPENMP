#include <stdio.h>
#include <omp.h>

int main(int argc, const char * argv[]){
omp_lock_t chave_simples;	
    omp_init_lock(&chave_simples);

    #pragma omp parallel num_threads(4)
    {

        while (!omp_test_lock(&chave_simples)){
            printf("=== Tid  %d: chave ocupada\n", omp_get_thread_num());
        }
        printf("+++ Tid %d: Consegui a chave\n", omp_get_thread_num());
        printf("--- Tid %d: Liberando a chave\n", omp_get_thread_num());
        omp_unset_lock(&chave_simples);    
        
    }

    omp_destroy_lock(&chave_simples);
}
