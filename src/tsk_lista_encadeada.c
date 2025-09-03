#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

typedef struct node node;
struct node {
    int data;
    node *next;
};

void process(node *p) {
    /* o trabalho é feito aqui */
    printf("Processando nó com valor: %d (thread %d)\n", p->data, omp_get_thread_num());
    sleep(1);
}

void increment_list_items(node *head) {
#pragma omp parallel
    #pragma omp single
    {
        node *p = head;
        while (p) {
            #pragma omp task
            process(p);
            p = p->next;
        }
    }
}

node* create_list(int n) {
    node *head = NULL, *temp = NULL;
    for (int i = 0; i < n; i++) {
        node *new_node = malloc(sizeof(node));
        new_node->data = i;
        new_node->next = NULL;
        if (!head) {
            head = new_node;
        } else {
            temp->next = new_node;
        }
        temp = new_node;
    }
    return head;
}

void free_list(node *head) {
    node *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

int main(int argc, char *argv[]) {
    node *head = create_list(5);

    increment_list_items(head);

    // Aguarda todas as tasks terminarem
    #pragma omp taskwait

    free_list(head);
    return 0;
}
