#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define TAMANHO 50000000  // Ajuste conforme desejar
#define CORTE    100000   // Tamanho mínimo para paralelizar
#define PRINT 100        // Tamanho impressão

// Função para imprimir o array
void imprimir_array(int arr[]) {
    for (int i = 0; i < PRINT; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Verifica se o array está ordenado em ordem crescente
int validar_array_ordenado(int arr[], int tamanho) {
    for (int i = 1; i < tamanho; i++) {
        if (arr[i - 1] > arr[i]) {
            return 1; // erro
        }
    }
    return 0; // ok
}

// Função de particionamento (pivô simples)
int particionar(int arr[], int baixo, int alto) {
    int pivô = arr[alto];
    int i = baixo - 1;
    for (int j = baixo; j < alto; j++) {
        if (arr[j] <= pivô) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[alto];
    arr[alto] = temp;
    return i + 1;
}

// Quicksort paralelo com OpenMP tasks
void quicksort_paralelo(int arr[], int baixo, int alto) {
    if (baixo < alto) {
        int pi = particionar(arr, baixo, alto);

        #pragma omp task default(none) shared(arr) firstprivate(baixo, pi)
        quicksort_paralelo(arr, baixo, pi - 1);

        #pragma omp task default(none) shared(arr) firstprivate(alto, pi)
        quicksort_paralelo(arr, pi + 1, alto);

        // Sincronização local: espera tarefas recursivas terminarem
        #pragma omp taskwait
    }
}

int main() {
    srand(time(NULL));  // Seed para rand

    int tamanho = TAMANHO;
    int *arr = (int *)malloc(tamanho * sizeof(int));

    // Preenche array com valores aleatórios
    for (int i = 0; i < tamanho; i++) {
        arr[i] = rand() % tamanho;
    }

    printf("Array original (%d):\n", tamanho);
    imprimir_array(arr);
    double inicio = omp_get_wtime();

    // Região paralela: inicia o quicksort com uma task inicial
    #pragma omp parallel default(none) shared(arr, tamanho)
        #pragma omp single nowait
        quicksort_paralelo(arr, 0, tamanho - 1);

    double fim = omp_get_wtime();
    printf("\n Tempo de execução %2.5f segundos \n", fim-inicio);
    printf("\nArray ordenado (%d):\n", tamanho);
    imprimir_array(arr);

    if (validar_array_ordenado(arr, tamanho)) {
        fprintf(stderr, "ERRO ao ordenar\n");
    } else {
        printf("Array ordenado corretamente.\n");
    }

    free(arr);
    return 0;
}
