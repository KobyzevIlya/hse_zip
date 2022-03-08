#include "tree_list.h"
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

//debug
void Simmetric(const NODE* root) {
    if (root) {
        Simmetric(root->left);
        if (root->isSymb)
            printf("%c: %s\n", root->symb, root->code);
        Simmetric(root->right);
    }
}

int main() {
    int* freq = init_array_with_zeroes(SYMBOLS_COUNT);
    char filename[] = "../input.txt";

    long length;
    get_chars_frequency(filename,freq, &length);

    NODE* init = NULL;

    make_list(&init, freq);

    make_tree(&init);

    //debug
    PrintTreeOnSide(init, 0);

    create_codes(&init, 0);
    //debug
    printf("\n");
    Simmetric(init);


    return 0;
}
