#include <stdio.h>
#include <omp.h>
#include <stdlib.h> // Para malloc e free

// Função auxiliar para imprimir parte de um array
void print_array_snippet(const char *name, double *arr, int n_total, int snippet_size) {
    printf("%s (primeiros %d elementos): {", name, snippet_size);
    for (int i = 0; i < snippet_size && i < n_total; i++) {
        printf(" %.2f%s", arr[i], (i == snippet_size - 1 || i == n_total - 1) ? "" : ",");
    }
    printf(" }\n");
}

int main() {
    const int N = 20; // Tamanho reduzido para facilitar a visualização
    const int PRINT_SNIPPET = 5;
    double *host_source = (double*)malloc(N * sizeof(double));
    double *host_target = (double*)malloc(N * sizeof(double)); // Para receber resultados e ser modificado

    // 1. Iniciando os dados no Hospedeiro
    for (int i = 0; i < N; i++) {
        host_source[i] = i * 1.0;
        host_target[i] = -1.0; // Valor inicial para ver a mudança
    }
     printf("Dados inciados no hospedeiro.\n");
    // 2. TARGET ENTER DATA: Mapear dados para o dispositivo
    #pragma omp target enter data map(to: host_source[0:N]) map(alloc: host_target[0:N])
    printf("Dados mapeados/alocados no dispositivo.\n");
    // 3. PRIMEIRA REGIÃO TARGET: Computação no dispositivo
    #pragma omp target
        for (int i = 0; i < N; i++) 
            host_target[i] = host_source[i] * 2.0;
    printf("host_target modificado no dispositivo.\n");
    // 4. TARGET UPDATE FROM: Atualizar dados do dispositivo para o hospedeiro
    #pragma omp target update from(host_target[0:N])
    print_array_snippet("Hospedeiro: host_target (após update from)", host_target, N, PRINT_SNIPPET);
    // 5. Modificar dados no HOST e TARGET UPDATE TO: Atualizar do host para o dispositivo
    host_source[0] = 100.0;
    print_array_snippet("Hospedeiro: host_source (após modificação local)", host_source, N, PRINT_SNIPPET);
    #pragma omp target update to(host_source[0:1])
    printf("host_source[0] atualizado do hospedeiro para o dispositivo.\n");
    // 6. SEGUNDA REGIÃO TARGET: Nova computação no dispositivo
    #pragma omp target
        for (int i = 0; i < N; i++) 
            host_target[i] = host_source[i] + 5.0; // Nova operação
    printf("Segunda computação modificando host_target no dispositivo.\n");
    // 7. TARGET EXIT DATA: Transferir dados finais e liberar memória do dispositivo
    #pragma omp target exit data map(from: host_target[0:N]) map(delete: host_source[0:N])
    printf("Dados finais de host_target transferidos para o hospedeiro, memória liberada no dispositivo.\n");
    // 8. Verificação Final no Hospedeiro
    printf("Verificação final dos dados no Hospedeiro:\n");
    // Deve ser o valor modificado em host_source[0]
    print_array_snippet("Hospedeiro: host_source (final)", host_source, N, PRINT_SNIPPET);
    // Deve refletir a segunda computação
    print_array_snippet("Hospedeiro: host_target (final)", host_target, N, PRINT_SNIPPET); 
    // Verificando alguns valores esperados para host_target
    printf("Verificação de host_target[0]: %.2f (Esperado: 100.0 + 5.0 = 105.0)\n", host_target[0]);
    if (N > 1) 
        printf("Verificação de host_target[1]: %.2f (Esperado: 1.0 + 5.0 = 6.0)\n", host_target[1]);
    // Liberando memória do host
    free(host_source);
    free(host_target);
    return 0;
}