// #define CCC_STRICT

#include "../../src/ccgbt.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    // Test 0
    // Demonstration

    printf("Test 0: \n\n");

    {
        ccgbt(char*) tree;
        ccgbt_init(tree);

        ccgbt_iter_root   (ITER(tree));
        ccgbt_insert_left (ITER(tree), "Welcome to ");
        ccgbt_insert_right(ITER(tree), "the ");

        ccgbt_iter_parent (ITER(tree));
        ccgbt_insert_left (ITER(tree), "\"C Container Collection\" ");
        ccgbt_insert_right(ITER(tree), "Project!\n");

        ccgbt_iter_root   (ITER(tree));
        ccgbt_iter_left   (ITER(tree));
        printf("%s",  GREF(ITER(tree)));

        ccgbt_iter_right  (ITER(tree));
        printf("%s",  GREF(ITER(tree)));

        ccgbt_iter_parent (ITER(tree));
        ccgbt_iter_left   (ITER(tree));
        printf("%s",  GREF(ITER(tree)));

        ccgbt_iter_right  (ITER(tree));
        printf("%s",  GREF(ITER(tree)));

        ccgbt_free(tree);
    }


    return 0;
}
