#include <stdio.h>
#include <omp.h>
int main(int argc, char* argv[]) {  /* off_target_teams.c */
    int total = 0;
    int N = 100;
    // Compartilhada entre todas as equipes
    int shared_var = 10;
    // Região target (dispositivo) com paralelismo de equipes
    #pragma omp target teams num_teams(4) thread_limit(8) \
    firstprivate(N) shared(shared_var) reduction(+: total)
    {
        // Região paralela dentro de cada equipe
        #pragma omp parallel 
        {
            // Cada thread calcula uma contribuição
            int local = omp_get_team_num() + omp_get_thread_num();
            // Uso de shared_var e N
            total += (local + shared_var) % N;
        }
    }
    printf("Resultado final da redução: %d\n", total);
    return 0;
}
