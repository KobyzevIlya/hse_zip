#include "union.h"

void simmetric(NODE* root, FILE* file) {
    if (root) {
        simmetric(root->left, file);
        if (root->isSymb) {
            if (root->symb != '\n' && root->symb != '\r')
                fprintf(file, "%c:%s ", root->symb, root->code);
            else if (root->symb == '\n')
                fprintf(file, "%s:%s ", "\\n", root->code);
            else if (root->symb == '\r')
                fprintf(file, "%s:%s ", "\\r", root->code);
        }
        simmetric(root->right, file);
    }
}

void archive(char output_filename[], int length, NODE** init) {
    FILE* get_codes = fopen(output_filename, "r");
    char str[length*10];
    fgets(str, length*10, get_codes);
    fclose(get_codes);
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

    //надо дописать хвост

    FILE* final = fopen(output_filename, "w");
    simmetric(*init, final);
    //fprintf(final, " %s", tail);
    fprintf(final, "\n%s", res);
    fclose(final);
}