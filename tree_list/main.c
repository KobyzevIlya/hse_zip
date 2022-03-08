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
    FILE* input = fopen(input_filename, "r");
    FILE* output = fopen(output_filename, "w");
    for (int i = 0; i < length; i++) {
        int symb = getc(input);
        find_and_print_code(&init, output, symb);
    }
    fclose(input);
    fclose(output);

    //union - заготовка
    FILE* fir_output = fopen(output_filename, "r");
    char str[length*10];
    fgets(str, length*10, fir_output);
    fclose(fir_output);
    int count = strlen(str) / BIT8;
    int tail = strlen(str) % BIT8;
    int len = count + 1;
    BIT_TO_CHAR symb;
    char* res = (char*)malloc(len*sizeof(char));
    for (int i = 0; i < len; i++) {
        symb.mbit.b1 = str[i*BIT8 + 0];
        symb.mbit.b2 = str[i*BIT8 + 1];
        symb.mbit.b3 = str[i*BIT8 + 2];
        symb.mbit.b4 = str[i*BIT8 + 3];
        symb.mbit.b5 = str[i*BIT8 + 4];
        symb.mbit.b6 = str[i*BIT8 + 5];
        symb.mbit.b7 = str[i*BIT8 + 6];
        symb.mbit.b8 = str[i*BIT8 + 7];
        res[i] = symb.symb;
    }
    FILE* sec_output = fopen(output_filename, "w");
    fprintf(sec_output, "%s", res);
    fclose(sec_output);

    return 0;
}
