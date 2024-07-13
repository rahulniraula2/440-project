#include "hauffman_encode.h"

#include <stdio.h>
#include "util.h"

// Forward declarations
void create_huffman_tree(heap *h);
int parent(int index);
int left_child(int index);
int right_child(int index);
void swap(heap_node *root, int i1, int i2);
void bubble_up(heap_node *root, int index);
void insert_node(heap *h, heap_node node);
heap_node *insert_node_to_the_back_of_the_pool(heap *h, heap_node node);
int get_min_child(heap *h, int index);
void bubble_down(heap *h, int index);
heap_node* pop_root(heap* h);
void insert(heap* h, char c, int frequency);

void initialize_heap(heap *h)
{
    for (int i = 0; i < MAX_CAPACITY_HEAP; i++)
    {
        heap_node empty = {0, '\0', NULL, NULL};
        h->root[i] = empty;
    }
    h->size = 0;
    h->back_size = 0;
}

void create_huffman_tree(heap *h)
{
    while (h->size > 1)
    {
        heap_node *left = pop_root(h);
        heap_node *right = pop_root(h);
        heap_node parent = {left->frequency + right->frequency, EOF, left, right};
        insert_node(h, parent);
    }
}

int parent(int index)
{
    return (index - 1) / 2;
}

int left_child(int index)
{
    return (index * 2) + 1;
}

int right_child(int index)
{
    return (index * 2) + 2;
}

void swap(heap_node *root, int i1, int i2)
{
    heap_node temp = root[i1];
    root[i1] = root[i2];
    root[i2] = temp;
}

void bubble_up(heap_node *root, int index)
{
    if (index == 0)
        return;

    int parent_index = parent(index);

    if (root[parent_index].frequency > root[index].frequency)
    {
        swap(root, parent_index, index);
        bubble_up(root, parent_index);
    }
}

void insert_all_from_frequency_table(heap *h, int frequency_table[256]){
    for (int i = 0; i < 256; i++)
    {
        int frequency = frequency_table[i];
        if (frequency)
            insert(h, (char)i, frequency);
    }
}

void generate_hauffman_tree(heap *h, int frequency_table[256])
{
    initialize_heap(h);
    insert_all_from_frequency_table(h, frequency_table);
    create_huffman_tree(h);
}

void insert_node(heap *h, heap_node node)
{
    if (h->size == MAX_CAPACITY_HEAP)
        FATAL_ERROR("Heap overflow");
    int size = h->size++;
    h->root[size] = node;
    bubble_up(h->root, size);
}

void insert(heap *h, char c, int frequency)
{
    heap_node hn = {frequency, c, NULL, NULL};
    insert_node(h, hn);
}

heap_node *insert_node_to_the_back_of_the_pool(heap *h, heap_node node)
{
    int back_size = h->back_size++;
    int index = MAX_CAPACITY_HEAP - back_size - 1;
    if (index < h->size)
        FATAL_ERROR("Back size overflow");
    h->root[index] = node;
    return &(h->root[index]);
}

int get_min_child(heap *h, int index)
{
    int leftIndex = left_child(index);
    int rightIndex = right_child(index);
    int size = h->size;
    heap_node *root = h->root;

    if (leftIndex >= size)
        return -1;

    if (rightIndex >= size)
        return leftIndex;

    return root[leftIndex].frequency < root[rightIndex].frequency ? leftIndex : rightIndex;
}

void bubble_down(heap *h, int index)
{
    int min_child = get_min_child(h, index);
    if (min_child == -1)
        return;

    if (h->root[min_child].frequency < h->root[index].frequency)
    {
        swap(h->root, index, min_child);
        bubble_down(h, min_child);
    }
}

heap_node *pop_root(heap *h)
{
    if (h->size == 0)
        FATAL_ERROR("Heap underflow");

    heap_node root = *h->root;
    int size = --h->size;
    heap_node last = h->root[size];
    *h->root = last;
    bubble_down(h, 0);
    return insert_node_to_the_back_of_the_pool(h, root);
}
