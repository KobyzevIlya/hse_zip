#ifndef TREE_LIST_TREE_LIST_H
#define TREE_LIST_TREE_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CODE_SIZE 100
#define SYMBOLS_COUNT 256

typedef struct node {
    unsigned char symb;
    unsigned int freq;
    struct node *next;
    int isSymb;
    char code[CODE_SIZE];
    struct node *right, *left;
}NODE;

int* init_array_with_zeroes(int count);
void get_chars_frequency(char filename[], int* freq_arr, long* length);
void add_to_list (NODE** init, unsigned int freq, int symb, NODE* branch);
void make_list(NODE** init, int* freq_arr);
void make_tree(NODE** init);
void create_codes(NODE** init, int level);
void find_and_print_code(NODE** init, FILE* file, int symb);
void change_symbols_to_bits(char input_filename[], char output_filename[], int length, NODE** init);

#endif //TREE_LIST_TREE_LIST_H
