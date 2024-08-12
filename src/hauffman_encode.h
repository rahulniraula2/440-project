#ifndef HAUFFMAN_ENCODE_H

#define HAUFFMAN_ENCODE_H

#define MAX_CAPACITY_HEAP 512

#include <stdio.h>
#include <stdint.h>

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

typedef struct coder
{
    unsigned int code;
    unsigned int length;
} coder;

// Export 256 to a config file
void generate_hauffman_tree(heap *h, int frequency_table[256]);
void generate_lookup_tables(heap *h, coder lookup_table[256]);
void encode_input_with_tree(heap *h, coder lookup_table[256], FILE *input_file, FILE *output_file);
void generate_precomputed_chars(heap *h, uint32_t array[512]);

void decode_input_with_lookup(heap *h, uint32_t array[512], FILE *input_file);

#endif
