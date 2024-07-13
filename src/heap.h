#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#define MAX_CAPACITY_HEAP 512

typedef struct heap_node
{
    int frequency;
    char c;
    struct heap_node *left;
    struct heap_node *right;
} heap_node;

typedef struct heap
{
    heap_node root[MAX_CAPACITY_HEAP];
    int size;
    int back_size;
} heap;

void heapify(heap *h, int *frequency_table);
void insert(heap *h, char c, int frequency);
heap_node peek_root(heap *h);
heap_node *pop_root(heap *h);

#endif
