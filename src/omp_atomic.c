#include <stdio.h>
#include <omp.h>
#define ARESTAS 10
#define VERTICES 7

int main(int argc, char *argv[]) { /* omp_atomic.c  */
int i,j;
int num_arestas = ARESTAS, grau[VERTICES]={0,0,0,0,0,0,0};
typedef struct {
   int vertice1, vertice2;
   } tipo_aresta;

tipo_aresta aresta[ARESTAS];
 
    aresta[0].vertice1 = 0;
    aresta[0].vertice2 = 1;
    aresta[1].vertice1 = 0;
    aresta[1].vertice2 = 2;
    aresta[2].vertice1 = 1;
    aresta[2].vertice2 = 2;
    aresta[3].vertice1 = 1;
    aresta[3].vertice2 = 3;
    aresta[4].vertice1 = 2;
    aresta[4].vertice2 = 3;
    aresta[5].vertice1 = 2;
    aresta[5].vertice2 = 5;
    aresta[6].vertice1 = 2;
    aresta[6].vertice2 = 4;
    aresta[7].vertice1 = 3;
    aresta[7].vertice2 = 5;
    aresta[8].vertice1 = 3;
    aresta[8].vertice2 = 4;
    aresta[9].vertice1 = 4;
    aresta[9].vertice2 = 6;
   
    omp_set_num_threads(4);

    #pragma omp parallel for
    for (j = 0; j< num_arestas; j++){
        #pragma omp atomic
        grau[aresta[j].vertice1]++;  
        #pragma omp atomic
        grau[aresta[j].vertice2]++; 
    }
    for (i = 0; i < VERTICES; i++)
   	    printf("Grau do vértice %d = %d \n",i, grau[i]);
   	return(0);
}
