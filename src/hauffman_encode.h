#ifndef HAUFFMAN_ENCODE_H
#define HAUFFMAN_ENCODE_H

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

//Export 256 to a config file
void generate_hauffman_tree(heap* h, int frequency_table[256]);
void print_hauffman_codes(heap* h);




#endif
