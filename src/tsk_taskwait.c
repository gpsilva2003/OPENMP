#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Definição da estrutura do nó
struct node {
    struct node *left;
    struct node *right;
    int value;
};

// Simula o processamento de um nó
void process(struct node *p) {
    printf("Processando nó com valor: %d\n", p->value);
}

// Travessia pós-ordem paralela com tasks
void postorder_traverse(struct node *p) {
    if (p == NULL) return;

    if (p->left)
        #pragma omp task // p é firstprivate por padrão
        postorder_traverse(p->left);

    if (p->right)
        #pragma omp task // p é firstprivate por padrão
        postorder_traverse(p->right);

    #pragma omp taskwait
    process(p);
}

// Cria uma árvore binária simples para demonstração
struct node* criar_arvore_demo() {
    struct node *raiz = malloc(sizeof(struct node));
    struct node *esq = malloc(sizeof(struct node));
    struct node *dir = malloc(sizeof(struct node));

    raiz->value = 1;
    raiz->left = esq;
    raiz->right = dir;

    esq->value = 2;
    esq->left = NULL;
    esq->right = NULL;

    dir->value = 3;
    dir->left = NULL;
    dir->right = NULL;

    return raiz;
}

int main(int argc, char* argv[]) {   /* tsk_taskwait.c */
    struct node *raiz = criar_arvore_demo();

    #pragma omp parallel
    {
        #pragma omp single
        postorder_traverse(raiz);
    }

    // Liberação da memória (boa prática)
    free(raiz->left);
    free(raiz->right);
    free(raiz);

    return 0;
}
