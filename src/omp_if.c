#include <stdio.h>  
#include <omp.h>

void teste(int valor)  {  
#pragma omp parallel if (valor)  /* Executa em paralelo se valor for verdadeiro */
    if (omp_in_parallel()) {  
        #pragma omp single  
        printf("Valor = %d, paralelizada com %d threads\n", valor, omp_get_num_threads());  
    }  
    else {  
        printf("Valor = %d, serializada\n", valor);  
    }  
}

int main(int argc, char *argv[]) {  /* omp_if.c  */
    omp_set_num_threads(4);  
    teste(0);     /* faz chamada com valor falso */
    teste(2);     /* faz chamada com valor verdadeiro */
    return(0);
} 

