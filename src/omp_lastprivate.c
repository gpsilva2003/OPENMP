#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
 
int main(int argc, char *argv[]) { /* omp_lastprivate.c  */
int i;
int x = 33;

    #pragma omp parallel for lastprivate(x)
    for(i = 0; i <= 10; i++){
        x = i;
        printf("Número da thread: %d    x: %d\n",omp_get_thread_num(),x);
    }
    printf("x é %d\n", x);
    return(0);
}