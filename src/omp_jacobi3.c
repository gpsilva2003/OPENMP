#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define COLS    3000
#define ROWS       3000
#define MAX_TEMP_ERROR 0.001

double **restrict A, **restrict Anew, **restrict tmp;

void iniciar();

int main(int argc, char *argv[]) { /* omp_jacobi3.c  */
int i, j;
int max_iterations=1000;
int iteration=1;
double dt=100;

A = (double **) malloc((ROWS+2) * sizeof(double*));
for(int i = 0; i < (ROWS+2); i++) 
	A[i] = (double *) malloc((COLS+2) * sizeof(double));

Anew = (double **) malloc((ROWS+2) * sizeof(double*));
for(int i = 0; i < (ROWS+2); i++) 
	Anew[i] = (double *) malloc((COLS+2) * sizeof(double));

    iniciar();
    double inicio=omp_get_wtime();
    while (dt > MAX_TEMP_ERROR && iteration <= max_iterations ) {
    dt = 0.0;
#pragma omp parallel for reduction(max:dt) schedule(static) default(none) private(j) shared(A, Anew)  
    for (i = 1; i <= ROWS; i++) 
        for (j = 1; j <= COLS; j++) {
            Anew[i][j] = 0.25 * (A[i+1][j] + 
            A[i-1][j] + A[i][j+1] + A[i][j-1]);
            dt = fmax( fabs(Anew[i][j]-A[i][j]), dt);
        }
    tmp = Anew;
    Anew = A;
    A = tmp;
    iteration++;
    }
    double fim=omp_get_wtime();
    printf("\n Erro maximo na iteracao %d era %f. O tempo de execução foi de %f  segundos com %d threads\n", iteration-1, dt, fim-inicio, omp_get_max_threads());
    return(0);
}

void iniciar(){
    int i,j;

    for (i = 0; i <= ROWS+1; i++) {
        for (j = 0; j <= COLS+1; j++) {
            A[i][j] = 0.0;
            Anew[i][j] = 0.0;
        }
    }

    for (i = 0; i <= ROWS+1; i++) {
        A[i][0] = 0.0;
        Anew[i][0] = 0.0;
        A[i][COLS+1] = (100.0/ROWS)*i;
        Anew[i][COLS+1] = (100.0/ROWS)*i;
    }

    for (j = 0; j <= COLS+1; j++) {
        A[0][j] = 0.0;
        Anew[0][j] = 0.0;
        A[ROWS+1][j] = (100.0/COLS)*j;
        Anew[ROWS+1][j] = (100.0/COLS)*j;
    }
}
