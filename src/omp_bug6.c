#include <omp.h>
#include <stdio.h>

#define NTHREADS 4
#define N 1000000
#define PI 3.1415926535
#define DELTA .01415926535

int main (int argc, char *argv[]) {
  int nthreads, tid, i;
  float a[N], b[N];
  omp_lock_t locka, lockb;

  omp_init_lock(&locka);
  omp_init_lock(&lockb);
  #pragma omp parallel num_threads(NTHREADS) private(tid,i) shared(nthreads,a,b,locka,lockb) 
  {
    tid = omp_get_thread_num();
    #pragma omp master
    {
      nthreads = omp_get_num_threads();
      printf("Número de threads: %d\n", nthreads);
    }
    #pragma omp barrier
    printf("Thread %d: a começar\n", tid);
    #pragma omp barrier
    #pragma omp sections nowait
    {
      #pragma omp section
      {
        omp_set_lock(&locka);
        printf("Thread %d: a iniciar a[]\n",tid);
        for (i = 0; i < N; i++)
          a[i] = i * DELTA;
        omp_set_lock(&lockb);
        printf("Thread %d: a calcular b[] utilizando a[]\n",tid);
        for (i = 0; i < N; i += 2)
          b[i] += a[i];
        omp_unset_lock(&lockb);
        omp_unset_lock(&locka);
      }
      #pragma omp section
      {
        omp_set_lock(&lockb);
        printf("Thread %d: a iniciar b[]\n",tid);
        for (i = 0; i < N; i++)
          b[i] = i * PI;
        omp_set_lock(&locka);
        printf("Thread %d: a calcular a[] utilizando b[]\n",tid);
        for (i = 1; i < N; i += 2)
          a[i] += b[i];
        omp_unset_lock(&locka);
        omp_unset_lock(&lockb);
      }
    }  /* #pragma omp sections */
    #pragma omp barrier
    printf("Thread %d: a terminar\n", tid);
  }  /* #pragma omp parallel */
  omp_destroy_lock(&locka);
  omp_destroy_lock(&lockb);
}
