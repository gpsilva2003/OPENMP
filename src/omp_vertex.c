#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define NEDGES 10
#define NVERTEX 10

typedef struct {
	int vertex1, vertex2;
} Edge;

int main(int argc, char** argv)
{
	Edge edge[NEDGES];
	int degree[NVERTEX];
	for(int i = 0; i < NVERTEX; i++) degree[i] = 0;

	srand(time(NULL));
	for(int i = 0; i < NEDGES; i++){
		edge[i].vertex1 = rand() % NVERTEX;
		edge[i].vertex2 = rand() % NVERTEX;
	}
	
	puts("Imprimindo o grafo");
	for(int i = 0; i < NEDGES; i++) printf("Aresta %d, vértice1 = %d, vértice2 = %d\n", i, edge[i].vertex1, edge[i].vertex2);

	#pragma omp parallel for
	for (int j=0; j < NEDGES; j++){
		#pragma omp atomic
			degree[edge[j].vertex1]++;  
		#pragma omp atomic
		        degree[edge[j].vertex2]++; 
	}

	puts("Imprimindo os graus dos vértices");
	for(int i = 0; i < NVERTEX; i++) printf("Vértice %d, grau %d\n", i, degree[i]);

	return 0;
		
}

