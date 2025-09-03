#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {  /* off_target_data.c */
    const int N = 10;
    double *host_source = (double*)malloc(N * sizeof(double));
    double *host_target = (double*)malloc(N * sizeof(double));

    // Inicializa os dados
    for (int i = 0; i < N; i++) {
        host_source[i] = i * 1.0;
        host_target[i] = -1.0;
    }
    
    // Mapeia e aloca dados no dispositivo
    #pragma omp target enter data map(to: host_source[0:N]) map(alloc: host_target[0:N])

    // Primeira computação no dispositivo
    #pragma omp target
    for (int i = 0; i < N; i++) 
        host_target[i] = host_source[i] * 2.0;

    // Atualiza resultado para o hospedeiro
    #pragma omp target update from(host_target[0:N])
    printf("Resultado após primeira computação:\n");
    for (int i = 0; i < N; i++) 
        printf("host_target[%d] = %.1f\n", i, host_target[i]);

    // Modifica o host_source e atualiza o dispositivo
    host_source[0] = 100.0;
    #pragma omp target update to(host_source[0:1])

    // Segunda computação no dispositivo
    #pragma omp target
    for (int i = 0; i < N; i++) 
        host_target[i] = host_source[i] + 5.0;

    // Transferência final e liberação
    #pragma omp target exit data map(from: host_target[0:N]) map(delete: host_source[0:N])
    printf("\nResultado após segunda computação:\n");
    for (int i = 0; i < N; i++) 
        printf("host_target[%d] = %.1f\n", i, host_target[i]);

    // Liberando memória
    free(host_source);
    free(host_target);
    return 0;
}
