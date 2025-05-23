#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf(">>> Início do programa principal (Thread %d)\n", omp_get_thread_num());

    // Inicia uma região paralela. OpenMP criará um time de threads.
    // num_threads(4) é opcional, mas ajuda a visualizar o paralelismo com um número fixo de threads.
    #pragma omp parallel num_threads(4)
    {
        // Este bloco 'single' garante que apenas uma thread (geralmente a thread master)
        // execute o código dentro dele, incluindo a criação das tasks.
        // O 'nowait' é crucial aqui: ele permite que as outras threads do time OpenMP
        // comecem a executar as tasks assim que elas são criadas, sem esperar que a
        // thread que está no bloco 'single' termine todo o seu bloco.
        #pragma omp single nowait
        {
            printf("  > Thread %d dentro do bloco single, começando a criar tarefas...\n", omp_get_thread_num());

            // Loop para criar múltiplas tasks
            for (int i = 0; i < 5; ++i) {
                // Cria uma task. O código dentro desta task pode ser executado por
                // qualquer thread no time OpenMP, a qualquer momento após sua criação.
                #pragma omp task
                {
                    // O código da task. Imprime qual tarefa é e qual thread a está executando.
                    // A ordem de execução destas linhas é não determinística.
                    printf("    - Tarefa %d executada pela thread %d.\n", i, omp_get_thread_num());
                }
            }

            printf("  > Thread %d dentro do bloco single, terminou de criar tarefas.\n", omp_get_thread_num());

            // Esta diretiva 'taskwait' faz com que a thread que a executa (a thread single)
            // espere até que TODAS as tasks filhas que ela criou (dentro deste bloco single)
            // tenham sido concluídas.
            #pragma omp taskwait

            printf("  > Thread %d dentro do bloco single, todas as tarefas foram concluídas.\n", omp_get_thread_num());

        } // Fim do bloco single. Se 'nowait' não fosse usado, haveria uma barreira implícita aqui.

        // As threads que não entraram no bloco single ou que terminaram suas tasks
        // podem esperar aqui até que a thread single termine o taskwait e saia do bloco single.
        // (Embora com 'nowait' no single e 'taskwait' explícito, o comportamento é mais claro).

    } // Fim da região paralela. Há uma barreira implícita aqui; todas as threads esperam
      // que as outras terminem a região paralela antes de continuar.

    printf("<<< Fim do programa principal (Thread %d)\n", omp_get_thread_num());

    return 0;
}
