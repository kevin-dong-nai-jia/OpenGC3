#include "ccgbt-test.h"
#include "../../src/tree/ccgbt.h"

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
        ccgbt_insert_left (ITER(tree), "\"OpenGC3\" ");

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
    // Insert / Push / Erase / Swap ...

    printf("\n\nTest 1: ");

    {
        ccgbt(char) tree;
        ccgbt_init(tree);

        ccgbt_iter_root   (ITER(tree));
        ccgbt_push_left   (ITER(tree), 'I');
        ccgbt_push_left   (ITER(tree), 'J');
        ccgbt_insert_right(ITER(tree), 'K');
        ccgbt_swap_subtree(ITER(tree));

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

        ccgbt_free(tree);
    }


    // Test 2
    // Traversal (Mixture)

    printf("\n\nTest 2: \n\n");

    {
        tree_t tree;
        tree_init(&tree);

        iter_t iter;
        tree_iter_init(&iter, &tree);
        tree_iter_root(&iter);

        tree_insert_left (&iter, '*');
        tree_insert_right(&iter, '*');

        tree_iter_left   (&iter);

        tree_insert_left (&iter, '2');
        tree_insert_right(&iter, '3');

        tree_push_right  (&iter, '*');
        tree_insert_left (&iter, '1');

        tree_push_left   (&iter, '+');
        tree_insert_right(&iter, '4');

        tree_iter_parent (&iter);
        tree_iter_right  (&iter);

        tree_insert_left (&iter, '5');
        tree_insert_right(&iter, '6');

        tree_push_left   (&iter, '-');
        tree_insert_right(&iter, '7');

        ccgbt_iter_parent(&iter);

        ccgbt_push_left  (&iter, '/');
        ccgbt_iter_left  (&iter);

        for (int cnt  = 0; cnt < 3; cnt++)
        {
            trav_t stat;

            CCGBT_INORDER(&iter, &stat)
            {
                ccgbt_iter_inorder(&iter, &stat);
                printf("%c ", GREF(&iter));
            }

            ccgbt_iter_restore(&iter, &stat);

            puts("== -36.2");
        }

        tree_free(&tree);
    }


    return 0;
}
