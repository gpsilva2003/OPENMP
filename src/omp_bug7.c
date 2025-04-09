#include <omp.h>
#include <stdio.h>
#define NTHREADS 4
#define N 100
void mult(float soma);
float a[N], b[N];
int main (int argc, char *argv[]) {
  int i;
  float soma;
  for (i = 0; i < N; i++)
	a[i] = b[i] = 1.0 * i;
  soma = 0.0;
  #pragma omp parallel num_threads(NTHREADS) shared(soma)
  mult(soma);
  printf("Soma = %f\n", soma);
}

void mult(float soma) {
  int i;
  #pragma omp master
  printf("Número de threads: %d\n", omp_get_num_threads());
  #pragma omp for reduction(+: soma)
  for (i = 0; i < N; i++)
    	soma += a[i] * b[i];
  return;
}
