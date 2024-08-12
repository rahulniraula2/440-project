#include "hauffman_encode.h"
#include "util.h"

// ========================= FUNCTION DECLARATIONS =========================

// Heap Operations
void insert(heap *h, char c, int frequency);
void insert_node(heap *h, heap_node node);
heap_node *insert_node_to_the_back_of_the_pool(heap *h, heap_node node);
heap_node *pop_root(heap *h);

// Heap Helper Functions
int parent(int index);
int left_child(int index);
int right_child(int index);
void bubble_up(heap_node *root, int index);
void bubble_down(heap *h, int index);
void swap(heap_node *root, int i1, int i2);
int get_min_child(heap *h, int index);
void initialize_heap(heap *h);

// Huffman Encoding Helpers
void create_huffman_tree(heap *h);
void insert_all_from_frequency_table(heap *h, int frequency_table[256]);

// ========================= HEAP OPERATIONS =========================

void insert(heap *h, char c, int frequency)
{
    heap_node hn = {frequency, c, NULL, NULL};
    insert_node(h, hn);
}

void insert_node(heap *h, heap_node node)
{
    if (h->size == MAX_CAPACITY_HEAP)
        FATAL_ERROR("Heap overflow");
    int size = h->size++;
    h->root[size] = node;
    bubble_up(h->root, size);
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

// ========================= HEAP HELPER FUNCTIONS =========================

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

// ========================= HUFFMAN TREE CREATION =========================

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

void insert_all_from_frequency_table(heap *h, int frequency_table[256])
{
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

// =========================== Encoding =================================

void print_lookup_table(coder lookup_table[256])
{
    printf("Printing Lookup Table\n");
    for (int i = 0; i < 256; i++)
    {
        unsigned int length = lookup_table[i].length;
        unsigned int code = lookup_table[i].code;
        unsigned int start = 1 << (length - 1);

        if (length == 0)
            continue;

        printf("%02hhx(%c): ", i, (char)i);

        for (int j = 0; j < length; j++)
        {
            char p = '0';
            if (code & start)
            {
                p = '1';
            }
            printf("%c", p);
            start = start >> 1;
        }

        printf("\n");
    }
}

void generate_lookup_tables_helper(heap_node *curr, coder lookup_table[256], unsigned int buffer, unsigned int buffer_length)
{

    if (curr == NULL)
        return;

    if (curr->left == NULL && curr->right == NULL)
    {
        int index = (int)curr->c;
        lookup_table[index].code = buffer;
        lookup_table[index].length = buffer_length;
    }

    // Go left
    unsigned int left_buffer = (buffer << 1);
    unsigned int left_buffer_length = buffer_length + 1;
    generate_lookup_tables_helper(curr->left, lookup_table, left_buffer, left_buffer_length);

    // Go right
    unsigned int right_buffer = (buffer << 1) | (1);
    unsigned int right_buffer_length = buffer_length + 1;
    generate_lookup_tables_helper(curr->right, lookup_table, right_buffer, right_buffer_length);
}

void generate_lookup_tables(heap *h, coder lookup_table[256])
{
    generate_lookup_tables_helper(h->root, lookup_table, 0, 0);
    print_lookup_table(lookup_table);
}

// Function to write bits to the output file
void write_bits(FILE *output_file, unsigned int *buffer, int *bit_count, unsigned int bits, int bit_length)
{
    for (int i = bit_length - 1; i >= 0; i--)
    {
        if (bits & (1 << i))
        {
            *buffer = (*buffer << 1) | 1;
        }
        else
        {
            *buffer = (*buffer << 1);
        }

        (*bit_count)++;

        if (*bit_count == 8)
        {
            fwrite(buffer, 1, 1, output_file);
            *buffer = 0;
            *bit_count = 0;
        }
    }
}

void printcoder(coder c)
{
    printf("Code: %u, Length: %u\n", c.code, c.length);
}
// Function to encode the input using the Huffman tree and the lookup table
void encode_input_with_tree(heap *h, coder lookup_table[256], FILE *input, FILE *output_file)
{
    unsigned int buffer = 0;
    int bit_count = 0;

    int c;
    while ((c = fgetc(input)) != EOF)
    {
        coder code = lookup_table[c];
        // printcoder(code);
        write_bits(output_file, &buffer, &bit_count, code.code, code.length);
    }

    // Write the remaining bits in the buffer if any
    if (bit_count > 0)
    {
        fwrite(&buffer, 1, 8 / bit_count, output_file);
    }

    char end_of_text = 0xF3;
    fwrite(&end_of_text, 1, 1, output_file);
}

void heap_get_char(heap *h, heap_node *hn, uint32_t pattern, uint32_t pos, uint32_t *pattern_count, uint32_t p_buffer, uint32_t *c_count, char c[3])
{

    if (*c_count > 3)
    {
        // printf("OVERLOADED\n");
        return;
    }

    // printf("Pattern: %u, c_count: %u, pattern_count: %u, c1: %c, c2: %c, c3: %c\n", pattern, *c_count, *pattern_count, c[0], c[1], c[2]);

    FATAL_IF_NULL(hn, "Heap Node was null -- when trying to precompute");

    if (hn->left == NULL && hn->right == NULL)
    {
        c[(*c_count)++] = hn->c;
        *pattern_count += p_buffer;
        return heap_get_char(h, h->root, pattern, pos, pattern_count, 0, c_count, c);
    }

    if (!pos)
        return;

    int tree_direction = pattern & pos;
    heap_node *new_node;
    if (tree_direction)
    {
        new_node = hn->right;
    }
    else
    {
        new_node = hn->left;
    }
    p_buffer++;

    heap_get_char(h, new_node, pattern, pos >> 1, pattern_count, p_buffer, c_count, c);
}

uint32_t precompute_result(heap *h, uint32_t pattern)
{

    char c[3] = {};
    ;
    uint32_t c_count = 0;
    uint32_t pattern_count = 0;
    uint32_t pos = 0b100000000;

    heap_get_char(h, h->root, pattern, pos, &pattern_count, 0, &c_count, c);

    uint32_t ret = 0;

    ret |= (c_count << 24);
    ret |= (pattern_count << 26);
    ret |= (c[0] << 16);
    ret |= (c[1] << 8);
    ret |= (c[2] << 0);
    // printf("P: %u, c_count: %u, p_count: %u, c1: %c, c2: %c, c3: %c\n", pattern, c_count, pattern_count, c[0], c[1], c[2]);
    return ret;
}

void generate_precomputed_chars(heap *h, uint32_t array[1024])
{

    for (uint32_t i = 0; i < 1024; i++)
    {
        array[i] = precompute_result(h, i);
    }
}

char lookup_using_tree_helper(heap_node *hn, uint32_t pattern, uint32_t pos, uint32_t *pattern_count)
{
    FATAL_IF_NULL(hn, "Heap Node was null -- when trying to decode");

    if (!pos)
    {
        FATAL_ERROR("Ran out of bits to decode");
    }

    if (hn->left == NULL && hn->right == NULL)
    {
        return hn->c;
    }

    int tree_direction = pattern & pos;

    heap_node *new_node;
    if (tree_direction)
    {
        new_node = hn->right;
    }
    else
    {
        new_node = hn->left;
    }

    (*pattern_count)++;

    return lookup_using_tree_helper(new_node, pattern, pos >> 1, pattern_count);
}

uint32_t lookup_using_tree(heap *h, uint32_t buffer, uint32_t buffer_count)
{
    uint32_t pos = 1 << (buffer_count - 1);
    uint32_t pattern_count = 0;
    char c = lookup_using_tree_helper(h->root, buffer, pos, &pattern_count);
    printf("%c", c);
    return pattern_count;
}

void decode_input_with_lookup(heap *h, uint32_t array[1024], FILE *input_file)
{
    uint32_t buffer = 0;
    uint32_t buffer_count = 0;

    uint32_t c;

    c = fgetc(input_file);

    while (c != EOF)
    {
        if (buffer_count <= 24)
        {
            buffer = (buffer << 8) | c;
            buffer_count += 8;
            c = fgetc(input_file);
        }

        // printf("Buffer: %u, Buffer Count: %u\n", buffer, buffer_count);

        if (buffer_count >= 9)
        {
            uint32_t pattern = buffer >> (buffer_count - 9) & 0b111111111;

            if (pattern > 1023)
            {
                printf("Pattern: %u\n", pattern);
                FATAL_ERROR("Invalid pattern");
            }

            uint32_t decoded = array[pattern];

            uint32_t c_count = (decoded >> 24) & 0b11;

            if (c_count == 0)
            {
                if (buffer_count > 24)
                {
                    uint32_t read_bits = lookup_using_tree(h, buffer, buffer_count);
                    buffer_count -= read_bits;
                }
            }
            else
            {
                char c[3] = {};
                c[0] = (decoded >> 16) & 0xFF;
                c[1] = (decoded >> 8) & 0xFF;
                c[2] = (decoded & 0xFF);

                for (int i = 0; i < c_count; i++)
                {
                    printf("%c", c[i]);
                }

                uint32_t pattern_count = (decoded >> 26);
                buffer_count -= pattern_count;
            }
        }
    }
}
