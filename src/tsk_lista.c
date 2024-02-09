typedef struct node node;
struct node {
    int data;
    node * next;
};
void process(node * p)
{
     /* o trabalho é feito aqui */
}
void increment_list_items(node * head) {
     #pragma omp parallel
     {
         #pragma omp single
         {
             node * p = head;
             while (p) {
                  #pragma omp task
                  // p é firstprivate por padrão
                  process(p);
                  p = p->next;
             }
         }
     }
}
