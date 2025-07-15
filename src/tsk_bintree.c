#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

// Função para criar um novo nó na árvore
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Função para calcular a soma dos elementos na árvore
int sumTree(struct Node* root) {
    if (root == NULL) return 0;

    int leftSum = 0, rightSum = 0;

    #pragma omp task shared(leftSum) depend(out: leftSum)
    leftSum = sumTree(root->left);

    #pragma omp task shared(rightSum) depend(out: rightSum)
    rightSum = sumTree(root->right);

    #pragma omp taskwait

    return root->data + leftSum + rightSum;
}

int main() {
    // Construção da árvore
    struct Node* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->right->left = createNode(6);
    root->right->right = createNode(7);

    int totalSum = 0;

    #pragma omp parallel
    {
        #pragma omp single
        #pragma omp task depend(out: totalSum)
        totalSum = sumTree(root);
    }

    printf("A soma dos elementos na árvore é: %d\n", totalSum);

    return 0;
}


