#if defined(PREFIX_CCSLL)
    #define prefix(func)  ccsll##func
    #define PREFIX(FUNC)  CCSLL##FUNC
    #include "../../src/list/extd-ccsll.h"
#else
    #error "UNDEFINE PREFIX_CC[S]LL"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    // Test 0
    // Demonstration

    printf("Test 0: \n\n");

    {
        prefix()(char*) list;
        prefix(_init)(list);

        prefix(_push_front)(list, "the \"OpenGC3\" Project!\n");
        prefix(_push_front)(list, "Welcome to ");

        PREFIX(_INCR)(ITER(list))
            printf("%s", SREF(ITER(list)));

        prefix(_free)(list);
    }


    // Test 1
    // Move Range

    printf("\n\nTest 1: \n\n");

    {
        prefix(_extd)(int, 5, NORMAL) list;
        prefix(_init)(list);

        int pos[8][3] = { {1, 3, 5}, {2, 3, 5}, {4, 1, 3}, {5, 1, 3},
                          {1, 3, 4}, {2, 3, 4}, {4, 2, 3}, {5, 2, 3} };

        for (int cnt = 6; cnt >= 0; cnt--)
            prefix(_push_front)(list, cnt + 'A');

        for (int idx = 0; idx < 3; idx++)
            prefix(_iter_head)(ITER_NTH(list, idx));

        PREFIX(_INCR)(ITER_NTH(list, 4))
            printf("%c ", SREF(ITER_NTH(list, 4)));

        for (int cnt = 0; cnt < 8; cnt++)
        {
            for (int idx = 0; idx < 3; idx++)
            {
                prefix(_iter_head)   (ITER_NTH(list, idx));
                prefix(_iter_advance)(ITER_NTH(list, idx), pos[cnt][idx]);
            }

            prefix(_move_range)(ITER(list),ITER_NTH(list, 1),ITER_NTH(list, 2));

            printf("/ ");
            PREFIX(_INCR)(ITER_NTH(list, 4))
                printf("%c ", SREF(ITER_NTH(list, 4)));
        }

        puts("= A C E D B F G /");

        prefix(_free)(list);
    }


    // Test 2
    // Move Begin

    printf("\n\nTest 2: \n\n");

    {
        prefix()(int) list_a, list_b;
        prefix(_init)(list_a);
        prefix(_init)(list_b);

        for (int cnt = 0; cnt < 10; cnt++)
            prefix(_push_front)(list_a, cnt);

        PREFIX(_INCR)(ITER(list_a))
            printf("%d ", SREF(ITER(list_a)));

        printf("-> ");

        for (int cnt = 0; cnt < 10; cnt++)
            ccsll_move_begin(ITER(list_b), ITER(list_a));

        PREFIX(_INCR)(ITER(list_b))
            printf("%d ", SREF(ITER(list_b)));

        puts("");

        prefix(_free)(list_a);
        prefix(_free)(list_b);
    }


    // Test 3
    // Merge

    printf("\n\nTest 3: \n\n");

    {
        prefix()(int) list_a, list_b;
        prefix(_init)(list_a);
        prefix(_init)(list_b);

        srand((unsigned)time(NULL));

        for (int cnt = 8; cnt >= 0; cnt--)
            prefix(_push_front)(list_a, rand() % 8 + (8 * cnt) + 17);
        for (int cnt = 8; cnt >= 0; cnt--)
            prefix(_push_front)(list_b, rand() % 8 + (8 * cnt) + 21);

        PREFIX(_INCR)(ITER(list_a))
            printf("%d ", SREF(ITER(list_a)));
        printf("<- ");
        PREFIX(_INCR)(ITER(list_b))
            printf("%d ", SREF(ITER(list_b)));
        puts("");

        printf("=> ");
        ccsll_merge(list_a, list_b);
        PREFIX(_INCR)(ITER(list_a))
            printf("%d ", SREF(ITER(list_a)));
        puts("");
        PREFIX(_INCR)(ITER(list_b))
            printf("%d ", SREF(ITER(list_b)));
        puts("");

        prefix(_free)(list_a);
        prefix(_free)(list_b);
    }
}
