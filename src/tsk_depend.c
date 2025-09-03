#include <omp.h>

int main(int argc, char* argv[]) { /* tsk_depend.c */
    int x = 0;
    #pragma omp parallel
        #pragma omp single
        {
            // Tarefa T1: lê e escreve x
            #pragma omp task depend(inout: x)
                x += 10;  // leitura e escrita em x

            // Tarefa T2: apenas lê x
            #pragma omp task depend(in: x)
                int y = x + 1;  // leitura de x 

            // Tarefa T3: também lê x
            #pragma omp task depend(in: x)
                int z = x * 2;  // leitura de x

            // Tarefa T4: lê e modifica x novamente
            #pragma omp task depend(inout: x)
                x = x * 2;  // leitura e escrita em x
            #pragma omp taskwait
        }
    return 0;
}
