#include <omp.h>
#include <stdio.h>

#define NTHREADS 4
#define N 400

int main (int argc, char *argv[]) {
  int i, j, a[N][N];

  #pragma omp parallel num_threads(NTHREADS)
  {
    #pragma omp for
    for (i = 0; i < N; i++) 
      for (j = 0; j < N; j++) 
        a[i][j] = 1;
    #pragma omp for
    for (i = 1; i < N; i++) 
      for (j = 0; j < N; j++) 
        a[i][j] += a[i-1][j];
  }
  printf("a[%d][%d] = %d\n", N-1, N-1, a[N-1][N-1]);
}
