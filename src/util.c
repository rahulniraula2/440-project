#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void FATAL_ERROR(char* error_message){
    printf("FATAL ERROR: %s\n", error_message);
    printf("EXITING\n");
    exit(1);
}

void FATAL_IF_NULL(void* pointer, char* message){
    if(pointer == NULL) FATAL_ERROR(message);
}
