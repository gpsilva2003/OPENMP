#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/types.h>

typedef struct node node;
struct node {
    int data;
    node *next;
};

// Simula o processamento de um nó
void process(node *p) {
    /* O trabalho é feito aqui */
    printf("Processando nó com valor: %d  Thread %d\n", p->data, omp_get_thread_num());
}

// Renomeada para process_list_items
void processa_itens_lista(node *head) {
    #pragma omp parallel
        #pragma omp single
        {
            node *p = head;
            while (p) {
                #pragma omp task // Cria várias tarefas para processar os nós
                process(p);     // p é firstprivate por padrão
                p = p->next;
            }
        }
}

int main() {
    // Criação de uma lista encadeada simples para teste
    node *head = malloc(sizeof(node));
    node *second = malloc(sizeof(node));
    node *third = malloc(sizeof(node));
    // Atribui valores iniciais aos nós
    head->data = 1;   
    head->next = second;
    second->data = 2;
    second->next = third;
    third->data = 3;
    third->next = NULL;

    printf("Iniciando processamento da lista...\n");
    processa_itens_lista(head); // Chamada da função para processar a lista
    printf("Processamento concluído.\n");
    // Libera a memória alocada
    free(third);
    free(second);
    free(head);
    return 0;
}