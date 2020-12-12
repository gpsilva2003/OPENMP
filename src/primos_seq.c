#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


unsigned char bit_width(unsigned long long x) {
    return x == 0 ? 1 : 64 - __builtin_clzll(x);
}

// implementation for all unsigned integer types
unsigned usqrt(const unsigned n) {
    unsigned char shift = bit_width(n);
    shift += shift & 1; // round up to next multiple of 2

    unsigned result = 0;

    do {
        shift -= 2;
        result <<= 1; // leftshift the result to make the next guess
        result |= 1;  // guess that the next bit is 1
        result ^= result * result > (n >> shift); // revert if guess too high
    } while (shift != 0);

    return result;
}

#define PBITS32  ((1<<2) | (1<<3) | (1<<5) | (1<<7) | (1<<11) | (1<<13) | \
                  (1UL<<17) | (1UL<<19) | (1UL<<23) | (1UL<<29) | (1UL<<31))

int isprime(unsigned int n) {
    if (n < 32)
        return (PBITS32 >> n) & 1;
    if ((n & 1) == 0)
        return 0;
    for (unsigned int p = 3; p * p <= n; p += 2) {
        if (n % p == 0)
            return 0;
    }
    return 1;
}

int primo (int n) {
	int i;
       
	for (i = 3; i < (int)(sqrt(n) + 1); i+=2) 
	     if (n%i == 0) 
	         return 0;
	return 1;
}

int main(int argc, char *argv[]) { /* primos_seq.c  */
	double t_inicio, t_fim;
	long int i,n,total=0;

	if (argc < 2) {
        printf("Valor inválido! Entre com o valor do maior inteiro\n");
       	return 0;
    } else {
        n = strtol(argv[1], (char **) NULL, 10);
    }
	t_inicio = omp_get_wtime();
	
	for (i = 3; i <= n; i += 2) 	
	     if(isprime(i) == 1) total++;
		
    total += 1;    /* Acrescenta o dois, que também é primo */
	t_fim = omp_get_wtime();
	printf("Quant. de primos entre 1 e %ld: %ld \n", n, total);
	printf("Tempo de execução: %f \n", t_fim-t_inicio);
	return(0);
}
