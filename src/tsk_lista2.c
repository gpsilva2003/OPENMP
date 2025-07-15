#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef struct node node;
struct node {
    int data;
    node *next;
};

// Função para criar um novo nó
node* create_node(int data) {
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(1);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// Função para adicionar um nó ao final da lista
void append(node** head, int data) {
    node* new_node = create_node(data);
    
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    
    node* last = *head;
    while (last->next != NULL) {
        last = last->next;
    }
    
    last->next = new_node;
}

// Função para imprimir a lista
void print_list(node* head) {
    node* current = head;
    printf("Lista: ");
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

// Função para liberar a memória da lista
void free_list(node* head) {
    node* current = head;
    node* next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

// Função que processa cada nó da lista
// Neste caso, incrementa o valor do nó em 1 e mostra qual thread realizou a operação
void process(node* p) {
    int thread_id = omp_get_thread_num();
    int old_value = p->data;
    
    // Simulando algum trabalho computacional
    for (int i = 0; i < 10000000; i++);
    
    // Incrementando o valor do nó
    p->data += 1;
    
    printf("Thread %d processou nó: %d -> %d\n", thread_id, old_value, p->data);
}

// Função principal que percorre a lista e cria tarefas para processar cada nó
void increment_list_items(node* head) {
    #pragma omp parallel
    {
        #pragma omp single
        {
            printf("Iniciando processamento paralelo com %d threads\n", omp_get_num_threads());
            
            node* p = head;
            while (p) {
                #pragma omp task firstprivate(p) // Garantindo que 'p' seja privado para cada tarefa
                {
                    process(p);
                }
                p = p->next;
            }
            
            printf("Todas as tarefas foram criadas\n");
        }
        // Barreira implícita aqui - todas as threads aguardam a conclusão de todas as tarefas
    }
    
    printf("Processamento paralelo concluído\n");
}

int main() {
    // Criando uma lista encadeada para teste
    node* head = NULL;
    
    // Adicionando 10 elementos à lista
    for (int i = 0; i < 10; i++) {
        append(&head, i * 10);
    }
    
    printf("Lista original:\n");
    print_list(head);
    
    // Definindo o número de threads (opcional)
    omp_set_num_threads(4);
    
    // Processando a lista em paralelo
    increment_list_items(head);
    
    printf("\nLista após processamento:\n");
    print_list(head);
    
    // Liberando a memória
    free_list(head);
    
    return 0;
}
