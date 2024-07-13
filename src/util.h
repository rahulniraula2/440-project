#ifndef UTIL_H
#define UTIL_H

#include "hauffman_encode.h"

void FATAL_ERROR(char *error_message);

void FATAL_IF_NULL(void *pointer, char *message);

void print_hauffman_codes(heap *h);

void print_complete_heap_as_array(heap *h);

#endif
