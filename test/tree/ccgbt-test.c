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

        ccgbt_iter_left   (ITER(tree));
        ccgbt_insert_right(ITER(tree), "the ");
        ccgbt_insert_left (ITER(tree), "\"C Container Collection\" ");

        ccgbt_iter_left   (ITER(tree));
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


    // Test 1
    // insert / push / erase / rotate ...

    printf("Test 1: \n\n");

    {
        ccgbt(char) tree;
        ccgbt_init(tree);

        ccgbt_iter_root   (ITER(tree));
        ccgbt_push_left   (ITER(tree), 'I');
        ccgbt_push_left   (ITER(tree), 'J');
        ccgbt_insert_right(ITER(tree), 'K');
        ccgbt_rotate      (ITER(tree));

        ccgbt_push_left   (ITER(tree), 'L');
        ccgbt_insert_right(ITER(tree), 'M');

        ccgbt_push_right  (ITER(tree), 'N');
        ccgbt_insert_left (ITER(tree), 'O');

        ccgbt_iter_right  (ITER(tree));
        ccgbt_erase_left  (ITER(tree));

        ccgbt_iter_right  (ITER(tree));
        ccgbt_erase_right (ITER(tree));

        ccgbt_iter_right  (ITER(tree));
        ccgbt_erase_left  (ITER(tree));

        ccgbt_iter_right  (ITER(tree));
        ccgbt_erase_right (ITER(tree));

        ccgbt_iter_right  (ITER(tree));
        ccgbt_erase_left  (ITER(tree));

        printf("%c%c\n", ccgbt_left(ITER(tree)), ccgbt_right(ITER(tree)));
    }


    return 0;
}
