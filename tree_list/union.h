#ifndef TREE_LIST_UNION_H
#define TREE_LIST_UNION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree_list.h"

#define BIT8 8

typedef union bit_to_char {
    char symb;
    struct bit {
        unsigned b1 : 1;
        unsigned b2 : 1;
        unsigned b3 : 1;
        unsigned b4 : 1;
        unsigned b5 : 1;
        unsigned b6 : 1;
        unsigned b7 : 1;
        unsigned b8 : 1;
    }mbit;
}BIT_TO_CHAR;

void archive(char output_filename[], int length, NODE** init);

#endif //TREE_LIST_UNION_H
