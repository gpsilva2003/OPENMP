// Exemplo em OpenMP com o algoritmo de Dijkstra para encontrar 
// o caminho mais curto em um grafo bidrecional. 
// Cada tarefa cuida de um conjuto de vértices.  
// Uso:  dijkstra 

#include <stdio.h>
#include <omp.h>

#define LARGEINT (1<<30)-1  // "Infinito"
#define NV 6

// variáveis gloabais // compratilhadas por todas as tarefas por padrão 

int dvv[NV][NV],    // Distância entre vértices vizinhos
    dmin[NV],       // Distâncias mínimas atá então encontradas
    naopronto[NV],  // vertices não visitados
    numt,           // número de threads
    tam,            // número de vértices tratato por cada tarefa 
    dmt,            // mínimo atual de todas as tarefas 
    vm;             // vértice oonde o mínimo foi encontrado

    void init( )
    {
        int i, j;
        for (i = 0; i < NV; i++)
            for (j = 0; j < NV; j++)
                dvv[i][j] = (i == j) ? 0 : LARGEINT;
    
        dvv[0][1] = dvv[1][0] = 40;
        dvv[0][2] = dvv[2][0] = 15;
        dvv[1][2] = dvv[2][1] = 20;
        dvv[1][3] = dvv[3][1] = 10;
        dvv[1][4] = dvv[4][1] = 25;
        dvv[2][3] = dvv[3][2] = 100;
        dvv[1][5] = dvv[5][1] = 6;
        dvv[4][5] = dvv[5][4] = 8;
    
        for (i = 1; i < NV; i++) {
            naopronto[i] = 1;
            dmin[i] = dvv[0][i];
        }
    }

// versão com OpenMP tasks
void busca() {
    #pragma omp parallel num_threads(4)
    {
        #pragma omp single
        {
            numt = omp_get_num_threads();    
            printf("Total de threads: %d \n",numt);  
            for (int step = 0; step < NV; step++) {
                dmt = LARGEINT;
                vm = 0;

                // Task: encontrar o menor caminho
                for (int i = 1; i < NV; i++) {
                    #pragma omp task firstprivate(i)
                    {
                        if (naopronto[i]) {
                            #pragma omp critical
                            {
                                if (dmin[i] < dmt) {
                                    dmt = dmin[i];
                                    vm = i;
                                }
                            }
                        }
                    }
                }

                #pragma omp taskwait // espera todos os candidatos a mv
                naopronto[vm] = 0;

                // Task: atualizar caminhos com base em mv
                for (int i = 1; i < NV; i++) {
                    #pragma omp task firstprivate(i)
                    {
                        if (naopronto[i]) {
                            int newdist = dmin[vm] + dvv[vm][i];
                            if (newdist < dmin[i]) {
                                #pragma omp critical
                                dmin[i] = newdist;
                            }
                        }
                    }
                }

                #pragma omp taskwait // espera todas atualizações
            }
        }
    }
}
int main(int argc, char **argv) {    // tsk_dijkstra.c
    int i;
    init();
    busca();  
   
    printf("Distancias minimas: ");
    for (i = 1; i < NV; i++)
        printf("%d ",dmin[i]);
    printf("\n ");
}
