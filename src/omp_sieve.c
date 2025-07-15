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
	    printf("Sem parâmetors, assumindo n = %d \n", N);
            n = N;
      	 } else {
            n = strtol(argv[1], (char **) NULL, 10);
         }
	// vetor com N entradas do tipo bool para economizar memória
	bool *vector = malloc(n * sizeof(bool)); 
	// inicio da medicao
	start = omp_get_wtime();
	// atribuindo true a todas as entradas do vetor
	// de inicio consideramos todos os numeros como primos
	memset(vector, true, n);

        int limite = (int) sqrt(n);
//        #pragma omp parallel for schedule(dynamic) // o melhor escalonamento é dynamic,1                                                
	for (i = 3; i <= limite; i+=2) {            // vericacao somente ate raiz de N ja basta
            if (!vector[i]) continue;              // não pode usar collapse por conta do if
            #pragma omp parallel for               // a paralelização aqui é mais eficiente  
            for(j = i * i; j < n; j += 2*i)             // loop sobre multiplos de i
                vector[j] = false;	              // marcando como nao primo
                                                      // Não é necessário atomic write                     
        }
        // efetuando contagem apos o crivo
        #pragma omp parallel for reduction(+:total)   // basta o escalonamento static com redução
		for (i = 3; i <= n; i+=2)
            if (vector[i]) 
               total++;                

	// fim da medicao
	end = omp_get_wtime();

	// impressao do tempo gasto e total de primos
	// somamos um no total, pois dois também é primo
	printf("Total de primos: %lu | Tempo total do algoritmo: %.3f segundos\n", total+1, end - start);
	printf("Total de threads: %d\n", omp_get_max_threads());

	return 0;
}
