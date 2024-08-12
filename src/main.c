#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "hauffman_encode.h"
#include "util.h"

// In the report talk about why we are using 1 byte sizes - reason: exponential growth of memory
#define MAX_CHARACTERS 256

int frequency_table[MAX_CHARACTERS] = {0};

char *file_to_encode = "src/resources/big.txt";

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
    fclose(f);

    heap h;
    generate_hauffman_tree(&h, frequency_table);

    coder lookup_table[256] = {0};
    generate_lookup_tables(&h, lookup_table);

    f = open_file(file_to_encode);
    FILE *output = fopen("output.bin", "wb");
    encode_input_with_tree(&h, lookup_table, f, output);
    fclose(output);
    fclose(f);

    uint32_t array[1024] = {};
    generate_precomputed_chars(&h, array);

    FILE *encoded_file = fopen("output.bin", "r");
    decode_input_with_lookup(&h, array, encoded_file);
    fclose(encoded_file);

    return 0;
}
