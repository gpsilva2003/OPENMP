#include <stdio.h>

void trab1() {
     printf("Trab1.\n");
}
void trab2() {
    printf("Trab2.\n");
}
    
int main(int argc, char *argv[]) { /* omp_single.c  */
#pragma omp parallel
{
    #pragma omp single
    printf("Começando Trab1.\n");
    trab1();
    #pragma omp single
    printf("Terminando Trab1.\n");
    #pragma omp single
    printf("Trab1 terminado e começando Trab2.\n");
    trab2();
}
    return(0);
}
