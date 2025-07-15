#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

// Definições de constantes para o tamanho da matriz e o erro máximo permitido
#define COLUMNS    1000
#define ROWS       1000
#define MAX_TEMP_ERROR 0.01

// Matrizes globais para armazenar os valores atuais e os novos valores
double Anew[ROWS+2][COLUMNS+2];
double A[ROWS+2][COLUMNS+2];

// Protótipo da função para inicializar as matrizes
void iniciar();

int main(int argc, char *argv[]) { /* jacobi_seq.c  */
    int i, j; // Variáveis de controle para os loops
    int max_iterations = 3000; // Número máximo de iterações permitidas
    int iteration = 1; // Contador de iterações
    double dt = 100; // Diferença máxima inicial entre as matrizes

    // Inicializa as matrizes com os valores de contorno
    iniciar();
    // Marca o tempo de início da execução
    double inicio = omp_get_wtime();
    // Loop principal do método de Jacobi
    while (dt > MAX_TEMP_ERROR && iteration <= max_iterations) {
        // Atualiza os valores da matriz Anew com base nos valores de A
        #pragma omp parallel for // Paraleliza o loop externo
        for (i = 1; i <= ROWS; i++) {
            for (j = 1; j <= COLUMNS; j++) {
                Anew[i][j] = 0.25 * (A[i+1][j] + A[i-1][j] + A[i][j+1] + A[i][j-1]);
            }
        }
        // Reinicia o valor de dt para calcular o erro máximo
        dt = 0.0;
        // Atualiza a matriz A com os valores de Anew e calcula o erro máximo
        #pragma omp parallel for private(j) reduction(max:dt) // Paraleliza o loop com redução para encontrar o máximo
        for (i = 1; i <= ROWS; i++) {
            for (j = 1; j <= COLUMNS; j++) {
                dt = fmax(fabs(Anew[i][j] - A[i][j]), dt); // Calcula o erro máximo
                A[i][j] = Anew[i][j]; // Atualiza a matriz A
            }
        }
        // Incrementa o contador de iterações
        iteration++;
    }
    // Marca o tempo de fim da execução
    double fim = omp_get_wtime();
    // Imprime o erro máximo, o número de iterações e o tempo de execução
    printf("\n Erro maximo na iteracao %d era %f. O tempo de execução foi de %f segundos\n", iteration-1, dt, fim-inicio);
    return 0;
}
// Função para iniciar as matrizes com os valores de contorno
void iniciar() {
    int i, j;

    // Inicia toda a matriz A com zeros
    for (i = 0; i <= ROWS+1; i++) {
        for (j = 0; j <= COLUMNS+1; j++) {
            A[i][j] = 0.0;
        }
    }
    // Define os valores de contorno para as bordas esquerda e direita
    for (i = 0; i <= ROWS+1; i++) {
        A[i][0] = 0.0; // Borda esquerda
        A[i][COLUMNS+1] = (100.0 / ROWS) * i; // Borda direita com gradiente linear
    }
    // Define os valores de contorno para as bordas superior e inferior
    for (j = 0; j <= COLUMNS+1; j++) {
        A[0][j] = 0.0; // Borda superior
        A[ROWS+1][j] = (100.0 / COLUMNS) * j; // Borda inferior com gradiente linear
    }
}