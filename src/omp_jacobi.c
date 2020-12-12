#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define COLUMNS    1000
#define ROWS       1000
#define MAX_TEMP_ERROR 0.01

double Anew[ROWS+2][COLUMNS+2];
double A[ROWS+2][COLUMNS+2];

void iniciar();

int main(int argc, char *argv[]) { /* jacobi_seq.c  */
int i, j;
int max_iterations=3000;
int iteration=1;
double dt=100;

    iniciar();
    double inicio=omp_get_wtime();

    while (dt > MAX_TEMP_ERROR && iteration <= max_iterations ) {
#pragma omp parallel for    

    for (i = 1; i <= ROWS; i++) 
        for (j = 1; j <= COLUMNS; j++) {
            Anew[i][j] = 0.25 * (A[i+1][j] + 
            A[i-1][j] + A[i][j+1] + A[i][j-1]);
        }
    dt = 0.0;
        
#pragma omp parallel for private(j) reduction(max:dt)
    for (i = 1; i <= ROWS; i++) 
        for (j = 1; j <= COLUMNS; j++) {
            dt = fmax( fabs(Anew[i][j]-A[i][j]), dt);
            A[i][j] = Anew[i][j];
        }

    iteration++;
    }
     double fim=omp_get_wtime();
     printf("\n Erro maximo na iteracao %d era %f. O tempo de execução foi de %f  segundos\n", iteration-1, dt, fim-inicio);
    return(0);
}

void iniciar(){
    int i,j;

    for (i = 0; i <= ROWS+1; i++) {
        for (j = 0; j <= COLUMNS+1; j++) {
            A[i][j] = 0.0;
        }
    }

    for (i = 0; i <= ROWS+1; i++) {
        A[i][0] = 0.0;
        A[i][COLUMNS+1] = (100.0/ROWS)*i;
    }

    for (j = 0; j <= COLUMNS+1; j++) {
        A[0][j] = 0.0;
        A[ROWS+1][j] = (100.0/COLUMNS)*j;
    }
}
