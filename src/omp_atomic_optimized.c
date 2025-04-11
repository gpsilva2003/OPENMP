#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define ARESTAS 10
#define VERTICES 7

int main(int argc, char *argv[]) {
    int i, j;
    int num_arestas = ARESTAS, grau[VERTICES] = {0};
    typedef struct {
        int vertice1, vertice2;
    } tipo_aresta;

    tipo_aresta aresta[ARESTAS];

    // Inicialização das arestas
    aresta[0].vertice1 = 0; aresta[0].vertice2 = 1;
    aresta[1].vertice1 = 0; aresta[1].vertice2 = 2;
    aresta[2].vertice1 = 1; aresta[2].vertice2 = 2;
    aresta[3].vertice1 = 1; aresta[3].vertice2 = 3;
    aresta[4].vertice1 = 2; aresta[4].vertice2 = 3;
    aresta[5].vertice1 = 2; aresta[5].vertice2 = 5;
    aresta[6].vertice1 = 2; aresta[6].vertice2 = 4;
    aresta[7].vertice1 = 3; aresta[7].vertice2 = 5;
    aresta[8].vertice1 = 3; aresta[8].vertice2 = 4;
    aresta[9].vertice1 = 4; aresta[9].vertice2 = 6;

    omp_set_num_threads(4);

    // Determina o número de threads dinamicamente
    int num_threads = omp_get_max_threads();

    // Aloca matriz local dinamicamente
    int **local_grau = malloc(num_threads * sizeof(int *));
    for (i = 0; i < num_threads; i++) {
        local_grau[i] = calloc(VERTICES, sizeof(int));
    }

    // Etapa paralela: cada thread acumula em sua matriz local
    #pragma omp parallel private(j)
    {
        int tid = omp_get_thread_num();
        #pragma omp for
        for (j = 0; j < num_arestas; j++) {
            local_grau[tid][aresta[j].vertice1]++;
            local_grau[tid][aresta[j].vertice2]++;
        }
    //}

    // Etapa de redução paralela: combina os resultados das matrizes locais
    #pragma omp for private(j) collapse(2)
    for (i = 0; i < VERTICES; i++) 
        for (j = 0; j < num_threads; j++) {
            grau[i] += local_grau[j][i];
        }
    }

    // Libera a memória alocada para a matriz local
    for (i = 0; i < num_threads; i++) {
        free(local_grau[i]);
    }
    free(local_grau);

    // Imprime os graus dos vértices
    for (i = 0; i < VERTICES; i++) {
        printf("Grau do vértice %d = %d \n", i, grau[i]);
    }

    return 0;
}