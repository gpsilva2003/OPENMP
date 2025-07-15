#include <stdio.h>
#include <omp.h>

// Função recursiva para calcular o número de Fibonacci
int fibo(int n) {
    int i, j;
    
    if (n < 2) return n;
    
    #pragma omp task final (n < 10) shared(i)
    i = fibo(n-1);
    
    #pragma omp task final (n < 10) shared(j)
    j = fibo(n-2);
    
    #pragma omp taskwait
    return i + j;
}

int main() {
    int n = 40, resultado; // Valor para calcular fibonacci
    
    printf("Calculando fibonacci(%d) com OpenMP tasks\n", n);
    
    double start_time = omp_get_wtime();
    
    #pragma omp parallel shared(resultado) num_threads(6)
    {
        #pragma omp single
        {
    	    printf("Número de threads disponíveis: %d\n\n", omp_get_num_threads());
            printf("Thread %d iniciando a computação principal\n", omp_get_thread_num());
            
            #pragma omp task if (n > 20) 
            resultado = fibo(n);
        }
    }
    
    double end_time = omp_get_wtime();
    printf("Resultado para %d é %d\n", n, resultado);
    printf("\nTempo de execução: %f segundos\n", end_time - start_time);
    
    return 0;
}
