#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// Estrutura de um nó da lista encadeada
struct Node {
    int data;
    struct Node* next;
};

// Função para criar um novo nó (ajuda a organizar o código)
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        perror("Falha ao alocar memória para o nó");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

int main() {
    struct Node* head = NULL;
    struct Node* current = NULL;

    // Preenche a lista com alguns valores (de 1 a 10)
    // Criando o primeiro nó
    head = createNode(1);
    current = head;
    // Adicionando os nós restantes
    for (int i = 2; i <= 10; i++) {
        current->next = createNode(i);
        current = current->next;
    }

    int total_sum = 0; // Variável para armazenar a soma total. Deve ser compartilhada.
    // Inicia a região paralela OpenMP
    #pragma omp parallel
    {
       // Esta diretiva garante que apenas uma thread (o master) entre neste bloco
       // #pragma omp single nowait // nowait permite que outras threads iniciem tarefas enquanto este bloco executa
	#pragma omp master
        {
            struct Node* temp = head;
            while (temp != NULL) {
                // Cria uma task para processar cada nó
                // firstprivate(temp): Cada task receberá uma cópia do ponteiro 'temp'
                // no momento em que a task é criada, garantindo que a task acesse o nó correto.
                // shared(total_sum): 'total_sum' é explicitamente marcada como compartilhada entre as tasks.
                //#pragma omp task firstprivate(temp) shared(total_sum)
                #pragma omp task shared(total_sum)
                {
                    // Adiciona o valor do nó (acessado através do ponteiro 'temp' private da task)
                    // à soma total. O #pragma omp atomic garante que a operação de adição
                    // em 'total_sum' seja segura em threads concorrentes.
                    #pragma omp atomic
                    total_sum += temp->data;
                    // Opcional: linha para depuração para ver qual thread processou qual valor
                    // printf("Task processando nó com valor %d na thread %d\n", temp->data, omp_get_thread_num());
                }
                temp = temp->next; // Avança para o próximo nó no thread que cria as tasks
            }
            // Espera que todas as tasks criadas dentro deste bloco 'single' terminem
            #pragma omp taskwait
        }
    } // Fim da região paralela. Neste ponto, todas as tasks foram concluídas e total_sum tem o resultado final.
    // Imprime a soma total
    printf("Soma total dos valores na lista encadeada: %d\n", total_sum);
    // Libera a memória alocada para a lista encadeada
    current = head;
    while (current != NULL) {
        struct Node* next = current->next;
        free(current);
        current = next;
    }
    return 0;
}
