#include <stdio.h>
#include <omp.h>

int main() {  /* omp_for */
    int i, n = 10;
    #pragma omp parallel for num_threads(3)
    for (i = 0; i < n; i++) {
        int thread_id = omp_get_thread_num();
        printf("Thread %d executou a iteração %d\n", thread_id, i);
    }
    return 0;
}
