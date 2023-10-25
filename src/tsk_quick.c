#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// Função para trocar dois elementos em um array
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Função para dividir o array em relação ao pivô
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Função principal do QuickSort
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi;

        #pragma omp task shared(pi)
        pi = partition(arr, low, high);

        #pragma omp taskwait

        #pragma omp task
        quickSort(arr, low, pi - 1);

        #pragma omp task
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    int arr[] = {12, 11, 13, 5, 6, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Array original: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    #pragma omp parallel
    {
        #pragma omp single
        quickSort(arr, 0, n - 1);
    }

    printf("Array ordenado: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}

