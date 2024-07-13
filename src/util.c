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

void print_complete_heap_as_array(heap *h)
{
    printf("[");

    for (int i=0; i<h->size; i++){
	// TODO: add printf of single element
	FATAL_ERROR("NOT IMPLEMENTED PRINT HEAP");
    }

    printf("]\n");
}

// Huffman tree functions
void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);

    printf("\n");
}

// printCodes Taken from https: // www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
void printCodes(heap_node *root, int arr[],
                int top)

{

    // Assign 0 to left edge and recur
    if (root->left)
    {

        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right)
    {

        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (root->left == NULL && root->right == NULL)
    {

        printf("'%c': ", root->c);
        printArr(arr, top);
    }
}

void print_hauffman_codes(heap* h){    
    int arr[100], top = 0;
    printCodes(h->root, arr, top);
}


