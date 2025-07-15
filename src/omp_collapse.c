#include <stdio.h>
int main(int argc, char *argv[]) { /* omp_collapse.c  */
int j, k, jlast, klast;
#pragma omp parallel
{
    #pragma omp for collapse(2) lastprivate(jlast, klast)
    for (k=1; k<=3; k++)
        for (j=1; j<=7; j++)
        {
            jlast=j;
            klast=k;
        }
    #pragma omp single
    printf("%d %d\n", klast, jlast);
}
}