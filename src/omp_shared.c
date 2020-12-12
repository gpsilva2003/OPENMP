#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) { /* omp_shared.c  */
int i, n = 7;
int a[n];

    (void) omp_set_num_threads(4);
    for (i = 0; i < n; i++)
        a[i] = i+1;
    #pragma omp parallel for shared(a)
    for (i = 0; i < n; i++) {
        a[i] += i;
    } /*-- Fim do parallel for --*/
    printf("No programa principal depois do parallel for:\n");
    for (i = 0; i < n; i++)
        printf("a[%d] = %d\n",i,a[i]);
    return(0);
}
