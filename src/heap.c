#include "heap.h"

#include <stdio.h>
#include "util.h"


int parent(int index){
    return index / 2;
}

int left_child(int index){
    return index*2;
}

int right_child(int index){
    return index*2 + 1;
}

int swap_if_less_than(heap_node* root, int index_parent, int index_child){
    heap_node temp = root[index_parent];
    if(temp.frequency > root[index_child].frequency){
	root[index_parent].frequency = root[index_child].frequency;
	root[index_parent].c = root[index_child].c;
	root[index_child].frequency = temp.frequency;
	root[index_child].c = temp.c;
	return 1;
    }
    return 0;
} 

void bubble_up(heap_node* root, int index){

    if (index == 0) return;

    int parent_index = parent(index);

    if (swap_if_less_than(root, parent_index, index))
	bubble_up(root, parent_index);
}

void heapify(heap* h, int frequency_table[256]){

    h->size = 0;
    
    for (int i = 0; i < 256; i++) {
	int frequency = frequency_table[i];
	if (frequency) insert(h, (char)i, frequency);  
    }
}

void insert(heap* h, char c, int frequency){
    
    int size = h->size++;
    if(size == MAX_CAPACITY_HEAP) FATAL_ERROR("Heap overflow");
    
    h->root[size].frequency = frequency;
    h->root[size].c = c;
    bubble_up(h->root, size);
}

heap_node peek_root(heap* h){
    return h->root[0];
}

heap_node pop_root(heap* h){
    heap_node hn = {};
    FATAL_ERROR("Function not implemented");
    return hn;
}

