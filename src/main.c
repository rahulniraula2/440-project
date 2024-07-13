#include <stdio.h>
#include <stdlib.h>

#include "hauffman_encode.h"
#include "util.h"

// In the report talk about why we are using 1 byte sizes - reason: exponential growth of memory
#define MAX_CHARACTERS 256

int frequency_table[MAX_CHARACTERS] = {};

char *file_to_encode = "src/resources/obama.txt";

FILE *open_file(char *file)
{
    FILE *f = fopen(file, "r");
    FATAL_IF_NULL(f, "Couldn't open the specified file");
    return f;
}

void output_file(FILE *f)
{
    char c = getc(f);
    while (c != EOF)
    {
        printf("%c", c);
        c = getc(f);
    }
}

void count_character_frequencies(FILE *f, int frequency_table[MAX_CHARACTERS])
{
    char c = getc(f);
    while (c != EOF)
    {
        int index = (int)c;
        frequency_table[index]++;
        c = getc(f);
    }
}

void output_frequency_table(int frequency_table[MAX_CHARACTERS])
{
    for (int i = 0; i < MAX_CHARACTERS; i++)
    {
        printf("%02hhx = %d\n", i, frequency_table[i]);
    }
}

int main()
{
    FILE *f = open_file(file_to_encode);

    count_character_frequencies(f, frequency_table);

    heap h;

    generate_hauffman_tree(&h, frequency_table);
    print_hauffman_codes(&h);
    
    
    fclose(f);
    return 0;
}
