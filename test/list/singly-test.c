#if defined(PREFIX_CCSLL)
    #include "../../src/list/ccsll.h"
    #define prefix(func)  ccsll##func
    #define PREFIX(FUNC)  CCSLL##FUNC
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


    // Test
    // Sort


    // printf("Test 1: \n\n");

    /* {
        prefix()(int) list;
        prefix(_init)(list);

        for (int i = 0; i < 1000; i++)
            prefix(_push_front)(list, rand());

        prefix(_sort)(list);

        PREFIX(_INCR)(ITER(list))
            printf("%d\n", SREF(ITER(list)));

        prefix(_free)(list);
    } */
}
