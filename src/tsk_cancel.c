#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[]) {  /* tsk_cancel.c */
    int found = 0;
    #pragma omp parallel shared(found)
    {
        #pragma omp for
        for (int i = 0; i < 100; i++) {
            if (i == 42) {
                #pragma omp atomic write
                found = 1;
                #pragma omp cancel for
            }
            #pragma omp cancellation point for
        }
    }
    printf("Encontrado: %d\n", found);
    return 0;
}
