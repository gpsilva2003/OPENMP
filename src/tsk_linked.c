#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// Estrutura de um nó da lista encadeada
struct Node {
    int data;
    struct Node* next;
};

// Função para buscar um valor na lista encadeada
struct Node* searchValue(struct Node* head, int target) {
struct Node* result;
    if (head == NULL) return NULL;
    #pragma omp task // Divide a busca em tarefas independentes

    if (head->data == target) {
        printf("Valor %d encontrado na thread %d\n", target, omp_get_thread_num());
	result = head;
//        return head;
    }
    else{ // Continua a busca no próximo nó
    #pragma omp task
    result = searchValue(head->next, target);
    }

    return result;
}

int main() {
    struct Node* head = (struct Node*)malloc(sizeof(struct Node));
    head->data = 1;
    head->next = NULL;

    struct Node* current = head;

    // Preenche a lista com alguns valores
    for (int i = 2; i <= 10; i++) {
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = i;
        newNode->next = NULL;
        current->next = newNode;
        current = newNode;
    }

    int target = 5;

    #pragma omp parallel
    {
        #pragma omp single
        {
            struct Node* result = searchValue(head, target);
            if (result == NULL) 
                printf("Valor %d não encontrado.\n", target);
        }
    }

    return 0;
}

