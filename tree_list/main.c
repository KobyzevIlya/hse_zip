#include "tree_list.h"
#include "union.h"
#include <stdlib.h>

//debug
void PrintTreeOnSide(const NODE* root, int level) {
    if (root) {
        PrintTreeOnSide(root->right, level + 1);
        for (int i = 0; i < level; i++) {
            printf("\t");
        }
        printf("%3d", root->freq);
        if (root->isSymb != 0)
            printf(":%c\n", root->symb);
        else
            printf("\n");
        PrintTreeOnSide(root->left, level + 1);
    }
}
//endofdebug

//debug
void Simmetric(const NODE* root) {
    if (root) {
        Simmetric(root->left);
        if (root->isSymb)
            printf("%c: %s\n", root->symb, root->code);
        Simmetric(root->right);
    }
}
//endofdebug

int main() {
    int* freq = init_array_with_zeroes(SYMBOLS_COUNT);
    char input_filename[] = "../input.txt";
    char output_filename[] = "../output.txt";

    long length;
    get_chars_frequency(input_filename,freq, &length);

    NODE* init = NULL;

    make_list(&init, freq);

    make_tree(&init);

    //debug
    PrintTreeOnSide(init, 0);
    //endofdebug

    create_codes(&init, 0);

    //debug
    printf("\n");
    Simmetric(init);
    //endofdebug

    //замена символов на биты
    change_symbols_to_bits(input_filename, output_filename, length, &init);

    //архивация
    archive(output_filename, length, &init);

    return 0;
}
