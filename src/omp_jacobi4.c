#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define COLUMNS    3000
#define ROWS       3000
#define MAX_ERROR 0.001

double *restrict  Anew;
double *restrict A;

void iniciar();

int main(int argc, char *argv[]) { /* omp_jacobi.c  */
int i, j;
int max_iterations=1000;
int iteration=1;
double dt=100;
double *tmp;

    Anew = (double*) malloc((ROWS+2)*(COLUMNS+2)*sizeof(double));
    A = (double*) malloc((ROWS+2)*(COLUMNS+2)*sizeof(double));
    iniciar();
    double t_inicio = omp_get_wtime();
    while (dt > MAX_ERROR && iteration <= max_iterations ) {
        dt = 0.0;
#pragma omp parallel for default(none) private(j) shared(A, Anew) schedule(static) reduction(max:dt) 
        for (i = 1; i <= ROWS; i++) {
             for (j = 1; j <= COLUMNS; j++) {
                  Anew[i*(ROWS+2)+j] = 0.25 * (A[(i+1)*(ROWS+2)+j] + A[(i-1)*(ROWS+2)+j] + A[i*(ROWS+2)+j+1] + A[i*(ROWS+2)+j-1]);
		  dt = fmax(fabs(Anew[i*(ROWS+2)+j] - A[i*(ROWS+2)+j]),dt);
             }
        }
        tmp = A;
        A = Anew;
        Anew = tmp;
        iteration++;
    }
    double t_fim = omp_get_wtime();
    printf("Erro maximo na iteracao %d era %f\n", iteration-1, dt);
    printf("Tempo paralelo: \t %f com %d threads.\n", t_fim-t_inicio, omp_get_max_threads());
    free(Anew);
    free(A);
    return(0);
}

void iniciar(){
    int i,j;
    // iniciacao dos valores da grade
    for (i = 0; i <= ROWS+1; i++) {
         for (j = 0; j <= COLUMNS+1; j++) {
              A[i*(ROWS+2)+j] = 0.0;
              Anew[i*(ROWS+2)+j] = 0.0;
         }
    }
    // iniciacao dos valores de bordas laterais
    for (i = 0; i <= ROWS+1; i++) {
         Anew[(i*(ROWS+2))+0] = A[(i*(ROWS+2))+0] = 0.0;
         Anew[(i*(ROWS+2))+COLUMNS+1] = A[(i*(ROWS+2))+COLUMNS+1] = (100.0/ROWS)*i;
    }
    // iniciacao dos valores de bordas superiores/inferiores
    for (j = 0; j <= COLUMNS+1; j++) {
         Anew[(0*(ROWS+2))+j] = A[(0*(ROWS+2))+j] = 0.0;
	 Anew[(ROWS+1)*(ROWS+2)+j] = A[(ROWS+1)*(ROWS+2)+j] = (100.0/COLUMNS)*j;
    }
}


