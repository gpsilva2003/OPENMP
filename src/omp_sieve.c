#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>
#include <string.h>

#define N 1000000000 // 1 bilhao

int main(int argc, char** argv)
{
	unsigned long total = 0, n,i,j;
	double start, end;

	if (argc < 2) {
	    printf("Sem parâmetors, assumindo n = %ld \n", N);
            n = N;
      	 } else {
            n = strtol(argv[1], (char **) NULL, 10);
         }
	// vetor com N entradas do tipo bool para economizar memória
	bool *vector = malloc(n * sizeof(bool)); 
	// atribuindo true a todas as entradas do vetor
	// de inicio consideramos todos os numeros como primos
	memset(vector, true, n);
	// inicio da medicao
	start = omp_get_wtime();
        int limite = (int) sqrt(n);
//        #pragma omp parallel for schedule(dynamic) // o melhor escalonamento é dynamic,1                                                
	for (i = 2; i <= limite; i++) {            // vericacao somente ate raiz de N ja basta
            if (!vector[i]) continue;              // não pode usar collapse por conta do if
            #pragma omp parallel for               // a paralelização aqui é mais eficiente  
            for(j = i * i; j < n; j += i)             // loop sobre multiplos de i
                vector[j] = false;	              // marcando como nao primo
                                                      // Não é necessário atomic write                     
        }
        // efetuando contagem apos o crivo
        #pragma omp parallel for reduction(+:total)   // basta o escalonamento static com redução
        for (i = 0; i < n; i++)
            if (vector[i]) 
               total++;                

	// fim da medicao
	end = omp_get_wtime();

	// impressao do tempo gasto e total de primos
	// subtraimos 2 do total, pois zero e um nao sao considerados primos
	printf("Total de primos: %lu | Tempo total do algoritmo: %.3f segundos\n", total - 2
, end - start);
	printf("Total de threads: %d\n", omp_get_max_threads());

	return 0;
}
