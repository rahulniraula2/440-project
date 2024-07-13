#ifndef UTIL_H
#define UTIL_H

#include "heap.h"

void FATAL_ERROR(char *error_message);

void FATAL_IF_NULL(void *pointer, char *message);

void print_heap(heap *h);

#endif
