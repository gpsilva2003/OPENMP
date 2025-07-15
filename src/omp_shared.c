#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]) { /* omp_shared.c  */
int i, n = 7, a [n];
    omp_set_num_threads(n);
    for (i = 0; i < n; i++)
         a[i] = i+1;
    #pragma omp parallel for default(none) shared(a,n) 
    for (i = 0; i < n; i++) {
         a[i] += i;
    } /*-- Fim do parallel for --*/
    printf("No programa principal depois do parallel for:\n");
    for (i = 0; i < n; i++)
         printf("a[%d] = %d\n",i,a[i]);
    return(0);
}
