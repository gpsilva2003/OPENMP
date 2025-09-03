#include <omp.h>
#include <stdio.h>
int main(int argc, char *argv[]) {  /* off_simples.c */
int N = 1000;
int a[N], b[N], c[N];
  // Iniciando os vetores no hospedeiro
  for (int i = 0; i < N; i++) {
       a[i] = i;
       b[i] = i * 2;
  }
  // Região paralela que será executada no dispositivo
  #pragma omp target map(to: a, b) map(from: c)
    for (int i = 0; i < N; i++) 
         c[i] = a[i] + b[i];
  // Exibindo os resultados
  for (int i = 0; i < 10; i++)
       printf("%d ", c[i]);
  printf("\n");
  return 0;
}
