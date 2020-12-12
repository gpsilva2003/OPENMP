#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) { /* omp_private2.c  */
int i, tid, n = 100000;
float a[n][4];

#pragma omp parallel default(none) private (i, tid) shared(a,n) num_threads(4)
{
    tid = omp_get_thread_num();
    for (i = 0; i < n; i++)
        a[i][tid] = 1.0;
}  /* Fim da região paralela */
return(0);
}