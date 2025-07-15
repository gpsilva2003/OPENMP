#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define SEED_DEFAULT 100
#define TAM_DEFAULT 20000000

int main(int argc, char* argv[]) {
    int tam, i,nthreads, tid;
    unsigned long long sum;
    int *vec1, *vec2;
    int seed;
    double inicio,fim;

    if(argc == 2) {
        tam = atoi(argv[1]);
        seed = SEED_DEFAULT;
    } else if(argc >= 3) {
        tam = atoi(argv[1]);
        seed = atoi(argv[2]);
    } else {
        tam = TAM_DEFAULT;
        seed = SEED_DEFAULT;
    }
    srand(seed);
    vec1 = (int*) malloc(tam*sizeof(int));
    vec2 = (int*) malloc(tam*sizeof(int));
    omp_set_num_threads(8);
    inicio = omp_get_wtime();
    #pragma omp parallel for schedule(static) shared(vec1,vec2) 
    for(i = 0;i < tam; i++) {
        vec1[i] = rand()%10000; //valores pequenos para a soma nao escalar muito
        vec2[i] = rand()%10000;
    }
    sum = 0;   
    #pragma omp parallel for reduction(+:sum) schedule(static) shared(vec1,vec2)
    for(i = 0; i < tam; i++) 
        sum += vec1[i] * vec2[i];    
    fim = omp_get_wtime();
    printf("O resultado da operacao eh: %lld\n", sum);
	printf("Tempo: %lf segundos\n\n", fim-inicio);
    free(vec1);
    free(vec2);
    return 0;
}

