#include <stdio.h>
#include <omp.h>
int main() {     /* off_codigo1.c  */
int var_firstprivate = 10;      
int var_private = 20;           
int vetor[5] = {1, 2, 3, 4, 5}; // para map(to:)
int result[5];                  // para map(from:)
int device_num = 0;             // ID do dispositivo
int tem_dispositivo = 1;        // para cláusula if

    // Verifica se há dispositivos disponíveis
    if (omp_get_num_devices() == 0) {
        printf("Nenhum dispositivo encontrado. A cláusula 'if' direcionará para o hospedeiro.\n");
        tem_dispositivo = 0;
    }
    #pragma omp target device(device_num) \
                         if(tem_dispositivo) \
                         map(to:vetor) \
                         map(from:result) \
                         firstprivate(var_firstprivate) \
                         private(var_private)
    {
        // Verifica se está executando no dispositivo ou
        // no hospedeiro (dentro da região target)
        if (omp_is_initial_device()) {
            printf("\nExecutando no Hospedeiro (devido à cláusula 'if' ou falta de dispositivos)\n");
        } else {
            printf("\nExecutando no Dispositivo %d\n", omp_get_device_num( ));
        }
        // Demonstração de firstprivate
        printf("No dispositivo: var_firstprivate = %d (valor inicial do hospedeiro)\n", var_firstprivate);
        // Demonstração de private
        var_private = 50; // Variável privada
        printf("No dispositivo: var_private = %d (iniciada no dispositivo)\n", var_private);
        // Processamento simples
        for (int i = 0; i < 5; i++) {
            result[i] = vetor[i] * var_firstprivate + var_private;
        }
        // Modificar var_firstprivate dentro da região target
        // não afeta a variável original do hospedeiro
        var_firstprivate = 999;
        printf("No dispositivo: var_firstprivate modificada para %d (não afeta o hospedeiro)\n", var_firstprivate);
    } // Fim da região target
    printf("\nValores finais no hospedeiro:\n");
    printf("var_firstprivate = %d (inalterada pela modificação no dispositivo)\n", var_firstprivate);
    printf("var_private = %d (inalterada pela modificação no dispositivo)\n", var_private);
    printf("Vetor result (após map(from:)) = {%d, %d, %d, %d, %d} \n",
           result[0], result[1], result[2], result[3], result[4]);
        // reflete a modificações feitas no dispositivo
    return 0;
}