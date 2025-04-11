#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/types.h>

typedef struct node node;
struct node {
    int data;
    node * next;
};
void process(node * p)
{
     /* o trabalho é feito aqui */
}
void increment_list_items(node * head) {
     #pragma omp parallel
     {
         #pragma omp single
         {
             node * p = head;
             while (p) {
                  #pragma omp task
                  // p é firstprivate por padrão
                  process(p);
                  p = p->next;
             }
         }
     }
}

int main() {
    // Criação de uma lista encadeada simples para teste
    node *head = malloc(sizeof(node));
    node *second = malloc(sizeof(node));
    node *third = malloc(sizeof(node));

    head->data = 1;
    head->next = second;

    second->data = 2;
    second->next = third;

    third->data = 3;
    third->next = NULL;

    printf("Iniciando processamento da lista...\n");
    increment_list_items(head);

    // Libera a memória alocada
    free(third);
    free(second);
    free(head);

    return 0;
}