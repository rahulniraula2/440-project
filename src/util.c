#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void FATAL_ERROR(char *error_message)
{
    printf("FATAL ERROR: %s\n", error_message);
    printf("EXITING\n");
    exit(1);
}

void FATAL_IF_NULL(void *pointer, char *message)
{
    if (pointer == NULL)
        FATAL_ERROR(message);
}
void level_order_traversal_print(heap_node *root)
{
    if (root == NULL)
        return;

    heap_node *queue[MAX_CAPACITY_HEAP];
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear)
    {
        heap_node *current = queue[front++];
        printf("\"%02hhx:%d\", ", current->c, current->frequency);

        if (current->left != NULL)
            queue[rear++] = current->left;

        if (current->right != NULL)
            queue[rear++] = current->right;
    }
}

void print_heap(heap *h)
{
    printf("[");

    level_order_traversal_print(h->root);

    printf("]\n");
}
