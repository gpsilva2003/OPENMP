#include <stdio.h>
#include <omp.h>

void task_body(int ref) {
    // A tarefa imprimirá o valor de 'ref' recebido no momento da criação
    printf("Thread %d executando tarefa: valor de ref = %d (endereço %p)\n",
           omp_get_thread_num(), ref, (void*)&ref);
}

void gen_task(int x) {
    // Essa tarefa é órfã (está fora da região paralela)
    // Como 'x' é argumento por referência, e não há 'default', ele é tratado como firstprivate
    #pragma omp task
    task_body(x);
}

void teste(int y, int z) {
    #pragma omp parallel private(y)
    {
        // Cada thread tem sua própria cópia de y
        y = z + 10;

        // Mostrando o endereço e valor antes da criação da tarefa
        printf("Thread %d: y = %d, endereço de y = %p\n",
               omp_get_thread_num(), y, (void*)&y);

        #pragma omp single
        {
            gen_task(y); // y é private (diferente por thread), mas vira firstprivate na tarefa
            gen_task(z); // z é compartilhado, mas também vira firstprivate na tarefa
        }

        y++; // Modifica y local da thread (sem efeito sobre a tarefa já criada)
    }
}


int main() {
    int a = 100, b = 200;

    teste(a, b);

    return 0;
}


