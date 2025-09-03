#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[])  { /* off_teams_distribute.c */
  int N = 1000;
  int a[N], b[N], c[N];
  // Valores iniciais dos vetores no hospedeiro
  for (int i = 0; i < N; i++) {
       a[i] = i;
       b[i] = i * 2;
  }
// Região paralela no dispositivo (GPU ou outro acelerador)
#pragma omp target teams distribute parallel for simd
  for (int i = 0; i < N; i++) 
       c[i] = a[i] + b[i];
  // Só os 10 primerios resultados   
  for (int i = 0; i < 10; i++)  
       printf("%d ", c[i]);
  printf("\n");
  return 0;
}
