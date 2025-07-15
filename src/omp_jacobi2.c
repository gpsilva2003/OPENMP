#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define COLS    3000
#define ROWS       3000
#define MAX_TEMP_ERROR 0.01
#define NT  4


double Anew[ROWS+2][COLS+2];
double A[ROWS+2][COLS+2];
double *restrict temp, *restrict anewp, *restrict ap;
//double *temp, *anewp, *ap;

void iniciar();

int main(int argc, char *argv[]) { /* omp_jacobi2.c  */
int i, j;
int max_iterations=1000;
int iteration=1;
double dt=100;

    iniciar();
    anewp=Anew[0];
    ap=A[0];
    double inicio=omp_get_wtime();
    while (dt > MAX_TEMP_ERROR && iteration <= max_iterations ) {
#pragma omp parallel for  default(none) shared(ap, anewp) private(j)
    for (i = 1; i <= ROWS; i++) 
        for (j = 1; j <= COLS; j++) {
            *(anewp+(i*(COLS+2)+j))= 0.25 * ( *(ap+(i+1)*(COLS+2)+j) +  
            *(ap+(i-1)*(COLS+2)+j) + *(ap+i*(COLS+2)+j+1) + *(ap+i*(COLS+2)+j-1));
        }
    dt = 0.0;
        
#pragma omp parallel for reduction(max:dt) default(none) shared(ap, anewp) private(j)
    for (i = 1; i <= ROWS; i++) 
        for (j = 1; j <= COLS; j++) {
            dt = fmax( fabs(*(anewp+i*(COLS+2)+j)-(*(ap+i*(COLS+2)+j))), dt);
        }
    temp= anewp;
    anewp=ap;
    ap=temp;
    iteration++;
    }
    double fim=omp_get_wtime();
    printf("\n Erro maximo na iteracao %d era %f. O tempo de execução foi de %f  segundos\n", iteration-1, dt, fim-inicio);
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
