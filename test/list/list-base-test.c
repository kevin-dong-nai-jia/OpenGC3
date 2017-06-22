#ifdef  DEBUG_CCDLL
#include "../../src/list/ccdll.h"
#define prefix(func)  ccdll##func
#define PREFIX(FUNC)  CCDLL##FUNC
#endif // DEBUG_CCDLL

#ifdef  DEBUG_CCXLL
#include "../../src/list/ccxll.h"
#define prefix(func)  ccxll##func
#define PREFIX(FUNC)  CCXLL##FUNC
#endif // DEBUG_CCXLL

#if !defined(DEBUG_CCDLL) && !defined(DEBUG_CCXLL)
#error "should be included from : cc[dx]ll-test.c"
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

        prefix(_push_back)(list, "Welcome to ");
        prefix(_push_back)(list, "the \"OpenGC3\" Project!\n");

        PREFIX(_INCR)(ITER(list))
            printf("%s", LREF(ITER(list)));

        prefix(_free)(list);
    }


    // Test 1
    // Push Back

    printf("\n\nTest 1: \n");

    {
        prefix()(char*) list;
        prefix(_init)(list);

        char *num_str[10] = { "five" , "six", "four", "seven", "three",
                              "eight", "two", "nine", "one"  , "ten"    };

        int cnt = 0;

        while (puts("") && cnt < 10)
        {
            prefix(_push_front)(list, num_str[cnt++]);
            prefix(_push_back )(list, num_str[cnt++]);

            PREFIX(_INCR)(ITER(list))
                printf("%s ", LREF(ITER(list)));
        }

        prefix(_free)(list);
    }


    // Test 2
    // Push Back

    printf("\n\nTest 2: ");

    {
        prefix()(int) list;
        prefix(_init)(list);

        int cnt = 0;

        while (cnt < 10000000)
            prefix(_push_back)(list, cnt++);

        prefix(_free)(list);

        puts("Test deallocation.");
    }


    // Test 3
    // Push Back

    printf("\n\nTest 3: ");

    {
        typedef struct  {  char *msg[2];  }   msg_struct;

        prefix()(msg_struct) list;
        prefix(_init)(list);

        msg_struct msg_1, msg_2;

        msg_1.msg[0] = "Push ";
        msg_1.msg[1] = "back ";
        msg_2.msg[0] = "two ";
        msg_2.msg[1] = "structs.\n";

        prefix(_push_back)(list, msg_1);
        prefix(_push_back)(list, msg_2);

        PREFIX(_INCR)(ITER(list))
            printf("%s", LREF(ITER(list)).msg[0]),
            printf("%s", LREF(ITER(list)).msg[1]);

        prefix(_free)(list);
    }


    // Test 4
    // Iter Incr

    printf("\n\nTest 4: ");

    {
        prefix(_extd)(int, 9, NORMAL) list;
        prefix(_init)(list);
        prefix(_iter_init)(ITER_NTH(list, 8), list);

        PREFIX(_INCR)(ITER_NTH(list, 8))
            printf("%d", LREF(ITER_NTH(list, 8)));

        if (prefix(_iter_incr)(ITER_NTH(list, 8)) == NULL)
            puts("The iterator points to the tail.");

        prefix(_free)(list);
    }


    // Test 5
    // Back and Front

    printf("\n\nTest 5: ");

    {
        prefix()(char*) list;
        prefix(_init)(list);

        (void)prefix(_front)(list);
        (void)prefix(_back )(list);

        prefix(_push_front)(list, "Failed ");
        prefix(_push_front)(list, "to access.\n");

        prefix(_front)(list) = "Element ";
        prefix(_back )(list) = "access succeeds.\n";

        PREFIX(_INCR)(ITER(list))
            printf("%s", LREF(ITER(list)));

        prefix(_free)(list);
    }


    // Test 6
    // Push and Pop

    printf("\n\nTest 6: ");

    {
        prefix()(int) list;
        prefix(_init)(list);

        prefix(_pop_front)(list);

        prefix(_push_front)(list, 9);
        prefix(_pop_front )(list);

        prefix(_push_back)(list, 1);
        prefix(_push_back)(list, 3);
        prefix(_pop_back )(list);

        PREFIX(_INCR)(ITER(list))
            printf("%d == 1\n", LREF(ITER(list)));

        prefix(_free)(list);
    }


    // Test 7
    // Push and Clear

    printf("\n\nTest 7: \n");

    {
        prefix()(int) list;
        prefix(_init)(list);

        puts("\nPushing back 10^7 elements...");
        for (int cnt = 0; cnt < 10000000; cnt++)
            prefix(_push_back)(list, cnt);
        puts("Clearing the container...");

        prefix(_clear)(list);

        puts("\nPushing back 10^7 elements...");
        for (int cnt = 0; cnt < 10000000; cnt++)
            prefix(_push_back)(list, cnt);
        puts("Deallocating the container...");

        prefix(_free)(list);
    }


    // Test 8
    // Packed

    printf("\n\nTest 8: \n\n");

    {
        prefix(_pckd)(char) list;
        prefix(_init)(list);

        for (int cnt = 0; cnt < 10000000; cnt++)
            prefix(_push_back)(list, (char)cnt);

        char var = 0;

        PREFIX(_INCR)(ITER(list))
            if (var != 127)  printf("%3d ", (var = LREF(ITER(list))));
            else  break;

        puts("");

        prefix(_free)(list);
    }


    // Test 9
    // Insert, Erase

    printf("\n\nTest 9: \n\n");

    {
        prefix(_pckd)(int) list;
        prefix(_init)(list);

        for (int cnt = 0; cnt < 6; cnt++)
            prefix(_push_back)(list, 2 * cnt + 1);

        PREFIX(_INCR)(ITER(list))
            printf("%d ", LREF(ITER(list)));

        prefix(_iter_head)(ITER(list));

        for (int cnt = 0; cnt <= 6; cnt++)
        {
            prefix(_iter_incr)(ITER(list));
            prefix(_insert)(ITER(list), 2 * cnt);
            prefix(_iter_incr)(ITER(list));
        }

        PREFIX(_INCR)(ITER(list));

        printf("-> ");
        PREFIX(_INCR)(ITER(list))
            printf("%d ", LREF(ITER(list)));

        prefix(_iter_begin)(ITER(list));

        for (int cnt = 0; cnt <= 5; cnt++)
        {
            prefix(_iter_incr)(ITER(list));
            prefix(_erase)(ITER(list));
        }

        printf("-> ");
        PREFIX(_INCR)(ITER(list))
            printf("%d ", LREF(ITER(list)));

        prefix(_iter_begin)(ITER(list));

        for (int cnt = 0; cnt <= 5; cnt++)
            prefix(_erase)(ITER(list));

        printf("-> ");
        PREFIX(_INCR)(ITER(list))
            printf("%d ", LREF(ITER(list)));

        prefix(_iter_begin)(ITER(list));

        prefix(_erase)(ITER(list));

        puts("-> (none)");
        PREFIX(_INCR)(ITER(list))
            printf("%d ", LREF(ITER(list)));

        prefix(_free)(list);
    }


    // Test 10
    // Move Range

    printf("\n\nTest 10: \n\n");

    {
        prefix(_extd)(int, 5, NORMAL) list;
        prefix(_init)(list);

        int pos[8][3] = { {2, 4, 6} , {-1, 2, 0}, {0, -1, -2}, {1, -1, 2},
                          {-4, 0, 3}, {0, 2, 0} , {1, 0, -1} , {1, -1, 1} };

        for (int cnt = 0; cnt < 7; cnt++)
            prefix(_push_back)(list, cnt + 'A');

        for (int idx = 0; idx < 3; idx++)
            prefix(_iter_head)(ITER_NTH(list, idx));

        PREFIX(_INCR)(ITER_NTH(list, 4))
            printf("%c ", LREF(ITER_NTH(list, 4)));

        for (int cnt = 0; cnt < 8; cnt++)
        {
            for (int idx = 0; idx < 3; idx++)
                prefix(_iter_advance)(ITER_NTH(list, idx), pos[cnt][idx]);

            prefix(_move_range)(ITER(list),ITER_NTH(list, 1),ITER_NTH(list, 2));

            printf("/ ");
            PREFIX(_INCR)(ITER_NTH(list, 4))
                printf("%c ", LREF(ITER_NTH(list, 4)));
        }

        puts("= A C E D B F G /");

        prefix(_free)(list);
    }


    // Test 11
    // Merge Range and Sort

    printf("\n\nTest 11: \n\n");

    {
        prefix(_extd)(int, 3, NORMAL) list;
        prefix(_init)(list);

        int str1[] = {0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15};

        for (int cnt = 0; cnt < 2; cnt++)
        {
            for (int i = 0; i < 16; i++)
                prefix(_push_back)(list, str1[i]);

            PREFIX(_INCR)(ITER(list))
                printf("%d ", LREF(ITER(list)));

            printf("-> ");

            switch (cnt)
            {
                case 0:
                for (int j = 0; j < 3; j++)
                    prefix(_iter_begin)  (ITER_NTH(list, j));
                for (int j = 0; j < 3; j++)
                    prefix(_iter_advance)(ITER_NTH(list, j), 8 * j);

                prefix(_merge_range)(ITER(list), ITER_NTH(list, 1),
                                                 ITER_NTH(list, 2));
                break;

                case 1:
                prefix(_sort)(list);
                break;
            }

            PREFIX(_INCR)(ITER(list))
                printf("%d ", LREF(ITER(list)));

            puts("");

            prefix(_clear)(list);
        }

        prefix(_free)(list);
    }


    // Test 12
    // Sort

    printf("\n\nTest 12: \n\n");

    {
        prefix()(int) list;
        prefix(_init)(list);

        int length = 1000;
        srand((unsigned)time(NULL));

        for (int cnt = 0; cnt < length; cnt++)
            prefix(_push_back)(list, rand());

        prefix(_sort)(list);

        PREFIX(_INCR)(ITER(list))
            printf("Max = %10d\r", LREF(ITER(list)));

        puts("");

        prefix(_free)(list);
    }


    // Test 13
    // Sort Parallel

    printf("\n\nTest 13: \n\n");
    puts("Parallel sort is currently not available.");


    // Test 14
    // Reverse Range

    printf("\n\nTest 14: \n\n");

    {
        #ifdef DEBUG_CCDLL
        puts("NOT IMPLEMENTED");
        #endif // DEBUG_CCDLL

        #ifdef DEBUG_CCXLL
        prefix(_extd)(int, 2, NORMAL) list;
        prefix(_init)(list);

        for (int i = 0; i < 10; i++)
            prefix(_push_back)(list, i);

        prefix(_iter_begin)(ITER_NTH(list, 0));
        prefix(_iter_begin)(ITER_NTH(list, 1));
        prefix(_iter_advance)(ITER_NTH(list, 0), 2);
        prefix(_iter_advance)(ITER_NTH(list, 1), 7);

        prefix(_reverse_range)(ITER_NTH(list, 0), ITER_NTH(list, 1));

        PREFIX(_INCR)(ITER(list))
            printf("%d ", LREF(ITER(list)));

        puts("= 0 1 2 6 5 4 3 7 8 9");

        prefix(_free)(list);
        #endif // DEBUG_CCXLL
    }


    // Test 15
    // Swap

    printf("\n\nTest 15: \n\n");

    {
        prefix()(int) list_a, list_b;
        prefix(_init)(list_a);
        prefix(_init)(list_b);

        for (int i = 0; i < 10; i++)
        {
            prefix(_push_back)(list_a, 2 * i);
            prefix(_push_back)(list_b, 2 * i + 1);
        }

        for (int cnt = 0; cnt < 3; cnt++)
        {
            printf("#%d   A : ", cnt + 1);
            PREFIX(_INCR)(ITER(list_a))
                printf("%d ", LREF(ITER(list_a)));
            printf("  /   B : ");
            PREFIX(_INCR)(ITER(list_b))
                printf("%d ", LREF(ITER(list_b)));
            printf("\n");

            prefix(_swap)(list_a, list_b);
        }

        prefix(_free)(list_a);
        prefix(_free)(list_b);
    }


    // Test 16
    // Resize

    printf("\n\nTest 16: \n\n");

    {
        prefix()(int) list;
        prefix(_init)(list);

        for (int cnt = 0; cnt < 16; cnt++)
            prefix(_push_back)(list, cnt);

        printf("Origin : ");

        PREFIX(_INCR)(ITER(list))
            printf("%d ", LREF(ITER(list)));

        printf("\nRsz 10 : ");

        prefix(_resize)(list, 10, 0);

        PREFIX(_INCR)(ITER(list))
            printf("%d ", LREF(ITER(list)));

        printf("\nRsz 16 : ");

        prefix(_resize)(list, 16, 10);

        PREFIX(_INCR)(ITER(list))
            printf("%d ", LREF(ITER(list)));

        puts("");

        prefix(_free)(list);
    }


    // Test 17
    // Move Range (Crossing)

    printf("\n\nTest 17: \n\n");

    {
        prefix(_extd)(int, 2, NORMAL) list_a, list_b;
        prefix(_init)(list_a);
        prefix(_init)(list_b);

        for (int cnt = 0; cnt < 16; cnt += 2)
            prefix(_push_back)(list_a, cnt);

        for (int cnt = 1; cnt < 16; cnt += 2)
            prefix(_push_back)(list_b, cnt);

        PREFIX(_INCR)(ITER(list_a))
            printf("%d ", LREF(ITER(list_a)));
        printf("/ ");
        PREFIX(_INCR)(ITER(list_b))
            printf("%d ", LREF(ITER(list_b)));
        printf("  size = %d ", prefix(_size)(list_a));
        printf("/ size = %d ", prefix(_size)(list_b));

        prefix(_iter_head)(ITER_NTH(list_a, 0));
        prefix(_iter_head)(ITER_NTH(list_a, 1));
        prefix(_iter_advance)(ITER_NTH(list_a, 0), 3);
        prefix(_iter_advance)(ITER_NTH(list_a, 1), 6);

        prefix(_iter_head)(ITER(list_b));
        prefix(_iter_advance)(ITER(list_b), 3);

        prefix(_move_range)(ITER(list_b), ITER(list_a), ITER_NTH(list_a, 1));
        prefix(_iter_init)(ITER(list_a), list_a);

        puts("");
        PREFIX(_INCR)(ITER(list_a))
            printf("%d ", LREF(ITER(list_a)));
        printf("/ ");
        PREFIX(_INCR)(ITER(list_b))
            printf("%d ", LREF(ITER(list_b)));
        printf("  size = %d ", prefix(_size)(list_a));
        printf("/ size = %d ", prefix(_size)(list_b));

        puts("");

        prefix(_free)(list_a);
        prefix(_free)(list_b);
    }


    // Test 18
    // Move (Crossing)

    printf("\n\nTest 18: \n\n");

    {
        prefix(_extd)(int, 2, NORMAL) list_a, list_b;
        prefix(_init)(list_a);
        prefix(_init)(list_b);

        for (int cnt = 0; cnt < 13; cnt++)
        {
            prefix(_push_back)(list_a, cnt);
            prefix(_push_back)(list_b, cnt + 13);
        }

        PREFIX(_INCR)(ITER_NTH(list_a, 1))
            printf("%2d ", LREF(ITER_NTH(list_a, 1)));
        printf("   size = %d (A)\n", prefix(_size)(list_a));
        PREFIX(_INCR)(ITER_NTH(list_b, 1))
            printf("%2d ", LREF(ITER_NTH(list_b, 1)));
        printf("   size = %d (B)\n", prefix(_size)(list_b));

        prefix(_iter_begin)(ITER(list_a));
        prefix(_iter_begin)(ITER(list_b));
        prefix(_move)      (ITER(list_a), ITER(list_b));

        PREFIX(_INCR)(ITER_NTH(list_a, 1))
            printf("%2d ", LREF(ITER_NTH(list_a, 1)));
        printf("size = %d (A)\n", prefix(_size)(list_a));
        PREFIX(_INCR)(ITER_NTH(list_b, 1))
            printf("%2d ", LREF(ITER_NTH(list_b, 1)));
        printf("\t  size = %d (B)\n", prefix(_size)(list_b));

        prefix(_iter_advance)(ITER(list_a), 1);
        prefix(_iter_advance)(ITER(list_b), 1);
        prefix(_move)        (ITER(list_a), ITER(list_b));

        PREFIX(_INCR)(ITER_NTH(list_a, 1))
            printf("%2d ", LREF(ITER_NTH(list_a, 1)));
        printf("size = %d (A)\n", prefix(_size)(list_a));
        PREFIX(_INCR)(ITER_NTH(list_b, 1))
            printf("%2d ", LREF(ITER_NTH(list_b, 1)));
        printf("\t  size = %d (B)\n", prefix(_size)(list_b));

        prefix(_iter_advance)(ITER(list_a), -2);
        prefix(_iter_init)   (ITER(list_b), list_b);
        prefix(_iter_tail)   (ITER(list_b));
        prefix(_move)        (ITER(list_b), ITER(list_a));

        PREFIX(_INCR)(ITER_NTH(list_a, 1))
            printf("%2d ", LREF(ITER_NTH(list_a, 1)));
        printf("   size = %d (A)\n", prefix(_size)(list_a));
        PREFIX(_INCR)(ITER_NTH(list_b, 1))
            printf("%2d ", LREF(ITER_NTH(list_b, 1)));
        printf("   size = %d (B)\n", prefix(_size)(list_b));

        prefix(_iter_head)    (ITER(list_a));
        prefix(_iter_tail)    (ITER(list_b));
        prefix(_iter_advance) (ITER(list_b), -1);
        prefix(_reverse_range)(ITER(list_a), ITER(list_b));

        PREFIX(_INCR)(ITER_NTH(list_a, 1))
            printf("%2d ", LREF(ITER_NTH(list_a, 1)));
        printf("   size = %d (A)\n", prefix(_size)(list_a));
        PREFIX(_INCR)(ITER_NTH(list_b, 1))
            printf("%2d ", LREF(ITER_NTH(list_b, 1)));
        printf("   size = %d (B)\n", prefix(_size)(list_b));

        prefix(_free)(list_a);
        prefix(_free)(list_b);
    }


    // Test 19
    // Merge (Crossing)

    printf("\n\nTest 19: \n\n");

    {
        prefix()(int) list_a, list_b;
        prefix(_init)(list_a);
        prefix(_init)(list_b);

        for (int cnt = 0; cnt < 10; cnt++)
            prefix(_push_back)(list_a, rand() % 10);
        for (int cnt = 0; cnt < 10; cnt++)
            prefix(_push_back)(list_b, rand() % 10);

        PREFIX(_INCR)(ITER(list_a))
            printf("%d ", LREF(ITER(list_a)));
        printf("/ ");
        PREFIX(_INCR)(ITER(list_b))
            printf("%d ", LREF(ITER(list_b)));
        printf("\n");

        prefix(_sort)(list_a);
        prefix(_sort)(list_b);

        PREFIX(_INCR)(ITER(list_a))
            printf("%d ", LREF(ITER(list_a)));
        printf("/ ");
        PREFIX(_INCR)(ITER(list_b))
            printf("%d ", LREF(ITER(list_b)));
        printf("\n");

        prefix(_merge)(list_a, list_b);

        PREFIX(_INCR)(ITER(list_a))
            printf("%d ", LREF(ITER(list_a)));
        printf("/ ");
        PREFIX(_INCR)(ITER(list_b))
            printf("%d ", LREF(ITER(list_b)));
        printf("\n");

        prefix(_free)(list_a);
        prefix(_free)(list_b);
    }


    // Test 20
    // Distance

    printf("\n\nTest 20: \n\n");

    {
        prefix()(int) list_a, list_b;
        prefix(_init)(list_a);
        prefix(_init)(list_b);

        for (int cnt = 0; cnt < 13; cnt++)
            prefix(_push_back)(list_a, cnt);
        for (int cnt = 0; cnt < 13; cnt++)
            prefix(_push_back)(list_b, cnt);

        prefix(_iter_head)(ITER(list_a));
        prefix(_iter_head)(ITER(list_b));
        prefix(_iter_advance)(ITER(list_a), 0);
        prefix(_iter_advance)(ITER(list_b), 8);

        int dist = 0;

        prefix(_iter_distance)(ITER(list_a), ITER(list_b), &dist);
        printf("distance = %d / ", dist);

        prefix(_iter_init)(ITER(list_b), list_a);
        prefix(_iter_head)(ITER(list_b));
        prefix(_iter_advance)(ITER(list_b), 8);

        prefix(_iter_distance)(ITER(list_a), ITER(list_b), &dist);
        printf("%d / ", dist);

        prefix(_iter_distance)(ITER(list_b), ITER(list_a), &dist);
        printf("%d \n", dist);

        prefix(_free)(list_a);
        prefix(_free)(list_b);
    }


    // Test 21
    // Demonstration

    #ifndef CC_STRICT

    printf("\n\nTest 21: \n\n");

    {
        prefix()(int) list;                     //  declare a list of type int
        prefix(_init)(list);                    //  initialize the list record

        for (int cnt = 8; cnt-- > 0; )          //
            prefix(_push_back)(list, rand());   //  insert "rand()" to the end

        prefix(_sort)(list);                    //  sort with comparator: XLEQ

        PREFIX(_INCR_AUTO)(pnum, list)          //  traverse the list forward:
            printf("num = %d\n", *pnum);        //  access elems through iters

        prefix(_free)(list);                    //  destroy the list after use
    }

    #endif // CC_STRICT


    return 0;
}
