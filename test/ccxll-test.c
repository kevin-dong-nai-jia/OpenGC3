#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/ccxll.h"


int main(void)
{
    // Test 0
    // Demonstration

    printf("Test 0: \n\n");

    {
        ccxll(char*) list;
        ccxll_init(list);

        ccxll_push_back(list, "Welcome to ");
        ccxll_push_back(list, "the \"C Container Collection\" Project!\n");

        CCXLL_INCR(ITER(list))
            printf("%s", DREF(ITER(list)));

        ccxll_free(list);
    }


    // Test 1
    // Push Back

    printf("\n\nTest 1: \n");

    {
        ccxll(char*) list;
        ccxll_init(list);

        char *num_str[10] = { "five" , "six", "four", "seven", "three",
                              "eight", "two", "nine", "one"  , "ten"    };

        int cnt = 0;

        while (puts("") && cnt < 10)
        {
            ccxll_push_front(list, num_str[cnt++]);
            ccxll_push_back (list, num_str[cnt++]);

            CCXLL_INCR(ITER(list))
                printf("%s ", DREF(ITER(list)));
        }

        ccxll_free(list);
    }


    // Test 2
    // Push Back

    printf("\n\nTest 2: ");

    {
        ccxll(int) list;
        ccxll_init(list);

        int cnt = 0;

        while (cnt < 10000000)
            ccxll_push_back(list, cnt++);

        ccxll_free(list);

        puts("Test deallocation.");
    }


    // Test 3
    // Push Back

    printf("\n\nTest 3: ");

    {
        typedef struct  {  char *msg[2];  }   msg_struct;

        ccxll(msg_struct) list;
        ccxll_init(list);

        msg_struct msg_1, msg_2;

        msg_1.msg[0] = "Push";
        msg_1.msg[1] = "back";
        msg_2.msg[0] = "two";
        msg_2.msg[1] = "structs.\n";

        ccxll_push_back(list, msg_1);
        ccxll_push_back(list, msg_2);

        CCXLL_INCR(ITER(list))
            printf("%s ", DREF(ITER(list)).msg[0]),
            printf("%s ", DREF(ITER(list)).msg[1]);

        ccxll_free(list);
    }


    // Test 4
    // Iter Incr

    printf("\n\nTest 4: ");

    {
        ccxll_extd(int, 9, NORMAL) list;
        ccxll_init(list);
        ccxll_iter_init(ITER_NTH(list, 8), list);

        CCXLL_INCR(ITER_NTH(list, 8))
            printf("%d", DREF(ITER_NTH(list, 8)));

        if (ccxll_iter_incr(ITER_NTH(list, 8)) == NULL)
            puts("The iterator points to the tail.");

        ccxll_free(list);
    }


    // Test 5
    // Back and Front

    printf("\n\nTest 5: ");

    {
        ccxll(char*) list;
        ccxll_init(list);

        (void)ccxll_front(list);
        (void)ccxll_back(list);

        ccxll_push_front(list, "Failed ");
        ccxll_push_front(list, "to access.\n");

        ccxll_front(list) = "Element ";
        ccxll_back (list) = "access succeeds.\n";

        CCXLL_INCR(ITER(list))
            printf("%s", DREF(ITER(list)));

        ccxll_free(list);
    }


    // Test 6
    // Push and Pop

    printf("\n\nTest 6: ");

    {
        ccxll(int) list;
        ccxll_init(list);

        ccxll_pop_front(list);

        ccxll_push_front(list, 9);
        ccxll_pop_front (list);

        ccxll_push_back(list, 1);
        ccxll_push_back(list, 3);
        ccxll_pop_back (list);

        CCXLL_INCR(ITER(list))
            printf("%d == 1\n", DREF(ITER(list)));

        ccxll_free(list);
    }


    // Test 7
    // Push and Clear

    printf("\n\nTest 7: \n");

    {
        ccxll(int) list;
        ccxll_init(list);

        puts("\nPushing back 10^7 elements...");
        for (int cnt = 0; cnt < 10000000; cnt++)
            ccxll_push_back(list, cnt);
        puts("Clearing the ccxll container...");

        ccxll_clear(list);

        puts("\nPushing back 10^7 elements...");
        for (int cnt = 0; cnt < 10000000; cnt++)
            ccxll_push_back(list, cnt);
        puts("Deallocating the ccxll container...");

        ccxll_free(list);
    }


    // Test 8
    // Packed

    printf("\n\nTest 8: \n\n");

    {
        ccxll_pckd(char) list;
        ccxll_init(list);

        for (int cnt = 0; cnt < 10000000; cnt++)
            ccxll_push_back(list, (char)cnt);

        char var = 0;

        CCXLL_INCR(ITER(list))
            if (var != 127)  printf("%3d ", (var = DREF(ITER(list))));
            else  break;

        puts("");

        ccxll_free(list);
    }


    // Test 9
    // Insert, Erase and Rearrange

    printf("\n\nTest 9: \n\n");

    {
        ccxll_pckd(int) list;
        ccxll_init(list);

        for (int cnt = 0; cnt < 6; cnt++)
            ccxll_push_back(list, 2 * cnt + 1);

        CCXLL_INCR(ITER(list))
            printf("%d ", DREF(ITER(list)));

        ccxll_iter_head(ITER(list));

        for (int cnt = 0; cnt <= 6; cnt++)
        {
            ccxll_iter_incr(ITER(list));
            ccxll_insert(ITER(list), 2 * cnt);
            ccxll_iter_incr(ITER(list));
        }

        CCXLL_INCR(ITER(list));
        ccxll_rearrange(list);

        printf("-> ");
        CCXLL_INCR(ITER(list))
            printf("%d ", DREF(ITER(list)));

        ccxll_iter_begin(ITER(list));

        for (int cnt = 0; cnt <= 5; cnt++)
        {
            ccxll_iter_incr(ITER(list));
            ccxll_erase(ITER(list));
        }

        ccxll_rearrange(list);

        printf("-> ");
        CCXLL_INCR(ITER(list))
            printf("%d ", DREF(ITER(list)));

        ccxll_iter_begin(ITER(list));

        for (int cnt = 0; cnt <= 5; cnt++)
            ccxll_erase(ITER(list));

        ccxll_rearrange(list);

        printf("-> ");
        CCXLL_INCR(ITER(list))
            printf("%d ", DREF(ITER(list)));

        ccxll_iter_begin(ITER(list));

        ccxll_erase(ITER(list));

        puts("-> (none)");
        CCXLL_INCR(ITER(list))
            printf("%d ", DREF(ITER(list)));

        ccxll_free(list);
    }


    // Test 10
    // Move Range

    printf("\n\nTest 10: \n\n");

    {
        ccxll_extd(int, 5, NORMAL) list;
        ccxll_init(list);

        int pos[8][3] = { {2, 4, 6} , {-1, 2, 0}, {0, -1, -2}, {1, -1, 2},
                          {-4, 0, 3}, {0, 2, 0} , {1, 0, -1} , {1, -1, 1} };

        for (int cnt = 0; cnt < 7; cnt++)
            ccxll_push_back(list, cnt + 'A');

        for (int idx = 0; idx < 3; idx++)
            ccxll_iter_head(ITER_NTH(list, idx));

        CCXLL_INCR(ITER_NTH(list, 4))
            printf("%c ", DREF(ITER_NTH(list, 4)));

        for (int cnt = 0; cnt < 8; cnt++)
        {
            for (int idx = 0; idx < 3; idx++)
                ccxll_iter_advance(ITER_NTH(list, idx), pos[cnt][idx]);

            ccxll_move_range_shared(ITER(list), ITER_NTH(list, 1),
                                                ITER_NTH(list, 2));

            printf("/ ");
            CCXLL_INCR(ITER_NTH(list, 4))
                printf("%c ", DREF(ITER_NTH(list, 4)));
        }

        puts("= A C E D B F G /");

        ccxll_free(list);
    }


    // Test 11
    // Merge Range

    printf("\n\nTest 11: \n\n");

    {
        ccxll_extd(int, 3, NORMAL) list;
        ccxll_init(list);

        int str1[] = {0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15};

        for (int cnt = 0; cnt < 2; cnt++)
        {
            for (int i = 0; i < 16; i++)
                ccxll_push_back(list, str1[i]);

            CCXLL_INCR(ITER(list))
                printf("%d ", DREF(ITER(list)));

            printf("-> ");

            switch (cnt)
            {
                case 0:
                for (int j = 0; j < 3; j++)
                    ccxll_iter_begin  (ITER_NTH(list, j));
                for (int j = 0; j < 3; j++)
                    ccxll_iter_advance(ITER_NTH(list, j), 8 * j);

                ccxll_merge_range(ITER(list), ITER_NTH(list, 1),
                                              ITER_NTH(list, 2));
                break;

                case 1:
                ccxll_sort(list);
                break;
            }

            CCXLL_INCR(ITER(list))
                printf("%d ", DREF(ITER(list)));

            puts("");

            ccxll_clear(list);
        }

        ccxll_free(list);
    }


    // Test 12
    // Sort

    printf("\n\nTest 12: \n\n");

    {
        ccxll(int) list;
        ccxll_init(list);

        int length = 1000;
        srand((unsigned)time(NULL));

        for (int cnt = 0; cnt < length; cnt++)
            ccxll_push_back(list, rand());

        ccxll_sort(list);

        CCXLL_INCR(ITER(list))
            printf("Max = %10d\r", DREF(ITER(list)));

        puts("");

        ccxll_free(list);
    }


    // Test 13
    // Sort Parallel

    printf("\n\nTest 13: \n\n");
    puts("Parallel sort is currently not available.");


    // Test 14
    // Reverse Range

    printf("\n\nTest 14: \n\n");

    {
        ccxll_extd(int, 2, NORMAL) list;
        ccxll_init(list);

        for (int i = 0; i < 10; i++)
            ccxll_push_back(list, i);

        ccxll_iter_begin(ITER_NTH(list, 0));
        ccxll_iter_begin(ITER_NTH(list, 1));
        ccxll_iter_advance(ITER_NTH(list, 0), 2);
        ccxll_iter_advance(ITER_NTH(list, 1), 7);

        ccxll_reverse_range(ITER_NTH(list, 0), ITER_NTH(list, 1));

        CCXLL_INCR(ITER(list))
            printf("%d ", DREF(ITER(list)));

        puts("= 0 1 2 6 5 4 3 7 8 9");

        ccxll_free(list);
    }


    // Test 15
    // Swap

    printf("\n\nTest 15: \n\n");

    {
        ccxll(int) list_a, list_b;
        ccxll_init(list_a);
        ccxll_init(list_b);

        for (int i = 0; i < 10; i++)
        {
            ccxll_push_back(list_a, 2 * i);
            ccxll_push_back(list_b, 2 * i + 1);
        }

        for (int cnt = 0; cnt < 3; cnt++)
        {
            printf("#%d   A : ", cnt + 1);
            CCXLL_INCR(ITER(list_a))
                printf("%d ", DREF(ITER(list_a)));
            printf("  /   B : ");
            CCXLL_INCR(ITER(list_b))
                printf("%d ", DREF(ITER(list_b)));
            printf("\n");

            ccxll_swap(list_a, list_b);
        }

        ccxll_free(list_a);
        ccxll_free(list_b);
    }


    // Test 16
    // Resize

    printf("\n\nTest 16: \n\n");

    {
        ccxll(int) list;
        ccxll_init(list);

        for (int cnt = 0; cnt < 16; cnt++)
            ccxll_push_back(list, cnt);

        printf("Origin : ");

        CCXLL_INCR(ITER(list))
            printf("%d ", DREF(ITER(list)));

        printf("\nRsz 10 : ");

        ccxll_resize(list, 10, 0);

        CCXLL_INCR(ITER(list))
            printf("%d ", DREF(ITER(list)));

        printf("\nRsz 16 : ");

        ccxll_resize(list, 16, 10);

        CCXLL_INCR(ITER(list))
            printf("%d ", DREF(ITER(list)));

        puts("");

        ccxll_free(list);
    }


    // Test 17
    // Move Range (Crossing)

    printf("\n\nTest 17: \n\n");

    {
        ccxll_extd(int, 2, NORMAL) list_a, list_b;
        ccxll_init(list_a);
        ccxll_init(list_b);

        for (int cnt = 0; cnt < 16; cnt += 2)
            ccxll_push_back(list_a, cnt);

        for (int cnt = 1; cnt < 16; cnt += 2)
            ccxll_push_back(list_b, cnt);

        CCXLL_INCR(ITER(list_a))
            printf("%d ", DREF(ITER(list_a)));
        printf("/ ");
        CCXLL_INCR(ITER(list_b))
            printf("%d ", DREF(ITER(list_b)));
        printf("  size = %d ", ccxll_size(list_a));
        printf("/ size = %d ", ccxll_size(list_b));

        ccxll_iter_head(ITER_NTH(list_a, 0));
        ccxll_iter_head(ITER_NTH(list_a, 1));
        ccxll_iter_advance(ITER_NTH(list_a, 0), 3);
        ccxll_iter_advance(ITER_NTH(list_a, 1), 6);

        ccxll_iter_head(ITER(list_b));
        ccxll_iter_advance(ITER(list_b), 3);

        ccxll_move_range_shared(ITER(list_b), ITER(list_a), ITER_NTH(list_a,1));
        ccxll_iter_init(ITER(list_a), list_a);

        puts("");
        CCXLL_INCR(ITER(list_a))
            printf("%d ", DREF(ITER(list_a)));
        printf("/ ");
        CCXLL_INCR(ITER(list_b))
            printf("%d ", DREF(ITER(list_b)));
        printf("  size = %d ", ccxll_size(list_a));
        printf("/ size = %d ", ccxll_size(list_b));

        puts("");

        ccxll_free(list_a);
        ccxll_free(list_b);
    }


    return 0;
}
