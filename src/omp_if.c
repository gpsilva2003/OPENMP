#include <stdio.h>  
#include <omp.h>

void teste(int valor)  {  
#pragma omp parallel if (valor)  
    if (omp_in_parallel()) {  
        #pragma omp single  
        printf("Valor = %d, paralelizada com %d threads\n", valor, omp_get_num_threads());  
    }  
    else {  
        printf("Valor = %d, serializada\n", valor);  
    }  
}

int main(int argc, char *argv[]) {  /* omp_if.c  */
    omp_set_num_threads(2);  
    teste(0);  
    teste(2);
    return(0);
} 

