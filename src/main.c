#include <stdio.h>
#include <stdlib.h>

#include "heap.h"
#include "util.h"

// In the report talk about why we are using 1 byte sizes - reason: exponential growth of memory
#define MAX_CHARACTERS 256

int frequency_table[MAX_CHARACTERS] = {};

char* file_to_encode = "src/resources/obama.txt";

FILE* open_file(char* file){
    FILE* f = fopen(file, "r");
    FATAL_IF_NULL(f, "Couldn't open the specified file");
    return f;
}

void output_file(FILE* f){
    char c = getc(f);
    while (c != EOF){
	printf("%c", c);
	c = getc(f);
    }
}

void count_character_frequencies(FILE* f, int frequency_table[MAX_CHARACTERS]){
    char c = getc(f);
    while(c != EOF){
	int index = (int)c;
	frequency_table[index]++;
	c = getc(f);
    }
}

void output_frequency_table(int frequency_table[MAX_CHARACTERS]){
    for (int i = 0; i < MAX_CHARACTERS; i++){
	printf("%02hhx = %d\n", i, frequency_table[i]); 
    }
}


void print_heap(heap* h){
    printf("[");
    for (int i = 0; i < h->size; i++){
	printf("\"%02xxh:%d\", ", h->root[i].c, h->root[i].frequency);
    }
    printf("]\n");	     
}


int main()
{
    FILE* f = open_file(file_to_encode);

    count_character_frequencies(f, frequency_table);

    heap h;

    heapify(&h, frequency_table);

    printf("Size of the heap is %d\n", h.size);
    printf("Min element in the heap is for %02hhx, with count %d\n", peek_root(&h).c, peek_root(&h).frequency);

    print_heap(&h);
    fclose(f);
    return 0;
}

