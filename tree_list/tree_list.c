#include "tree_list.h"

int* init_array_with_zeroes(int count) {
    int* arr = (int*)malloc(count*sizeof(int));
    for (int i = 0; i < count; i++) {
        arr[i] = 0;
    }
    return arr;
}

void get_chars_frequency(char filename[], int* freq_arr, long* length) {
    FILE* input = fopen(filename, "rb");
    if (!input) exit(1);
    fseek(input, 0, SEEK_END);
    *length = ftell(input);
    fseek(input, 0, SEEK_SET);
    for (int i = 0; i < *length; i++) {
        freq_arr[(unsigned char)fgetc(input)]++;
    }
    fclose(input);
}

void add_to_list (NODE** init, unsigned int freq, int symb, NODE* branch) {
    while (*init) {
        if ((*init)->freq > freq)
            break;
        init = &((*init)->next);
    }
    NODE* pnew = (NODE*)malloc(sizeof(NODE));

    pnew->freq = freq;
    pnew->symb = (char) symb;
    pnew->left = NULL;
    pnew->right = NULL;
    if (branch != NULL)
        pnew = branch;
    else
        pnew->isSymb = 1;
    pnew->next = *init;
    *init = pnew;
}

void make_list(NODE** init, int* freq_arr) {
    for (int i = 0; i < SYMBOLS_COUNT; i++) {
        if (freq_arr[i]) {
            add_to_list(&(*init), freq_arr[i], i, NULL);
        }
    }
}

NODE* make_node_from_two(const NODE* left, const NODE* right) {
    NODE* res = (NODE*)malloc(sizeof(NODE));
    res->freq = left->freq + right->freq;
    res->isSymb = 0;
    res->left = left;
    res->right = right;
    res->next = NULL;
    return res;
}

void make_tree(NODE** init) {
    while ((*init)->next) {
        NODE* new = make_node_from_two((*init), (*init)->next);
        if (((*init)->next)->next) {
            (*init) = ((*init)->next)->next;
            add_to_list(&(*init), new->freq, '0', new);
        }
        else
            (*init) = new;
    }
}

char code[CODE_SIZE];
void create_codes(NODE** init, int level) {
    if (*init) {
        if ((*init)->isSymb != 0) {
            code[level] = '\0';
            strcpy((*init)->code, code);
        }
        code[level] = '0';
        create_codes(&((*init)->left), level + 1);
        code[level] = '1';
        create_codes(&((*init)->right), level + 1);
    }
}

void find_and_print_code(NODE** init, FILE* file, int symb) {
    if (*init) {
        if ((*init)->isSymb && (int)(*init)->symb == symb) {
            fprintf(file, "%s", (*init)->code);
            return;
        }
        find_and_print_code(&((*init)->left), file, symb);
        find_and_print_code(&((*init)->right), file, symb);
    }
}

void change_symbols_to_bits(char input_filename[], char output_filename[], int length, NODE** init) {
    FILE* input = fopen(input_filename, "r");
    FILE* output = fopen(output_filename, "w");
    for (int i = 0; i < length; i++) {
        int symb = getc(input);
        find_and_print_code(init, output, symb);
    }
    fclose(input);
    fclose(output);
}
