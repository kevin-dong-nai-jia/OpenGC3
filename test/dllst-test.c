#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/dllst.h"

#define INCR_LOOP_(_a, _b)           for (int _a = 0; _a < (_b); _a++)
#define INCR_LOOP2_(_a, _b, _c, _d)  INCR_LOOP_(_a, _b)  INCR_LOOP_(_c, _d)


int main(void)
{
    /* Test 0 */
    /* For demonstration */

    printf("Test 0: \n\n");

    {
        dllst(char*) msg;
        dllst_init(msg);

        dllst_iter(char*) msg_iter;
        dllst_iter_init(msg_iter, msg);

        dllst_push_back(msg, "Hello everyone:\n");
        dllst_push_back(msg, "Welcome to C Container Collection Project!\n");

        DLLST_TRAVERSAL(msg, msg_iter)
            printf("%s", dllst_iter_dref(msg_iter));

        dllst_free(msg);
    }


    /* Test 1 */
    /* Test push back */

    printf("\n\nTest 1: \n");

    {
        dllst(char*) list;
        dllst_init(list);

        char *num_str[10] = { "five" , "six", "four", "seven", "three",
                              "eight", "two", "nine", "one"  , "ten"    };

        int cnt = 0;

        while (puts("") && cnt < 10)
        {
            dllst_iter(char*) iter;
            dllst_iter_init(iter, list);

            dllst_push_front(list, num_str[cnt++]);
            dllst_push_back (list, num_str[cnt++]);

            DLLST_TRAVERSAL(list, iter)
                printf("%s ", dllst_iter_dref(iter));
        }

        dllst_free(list);
    }


    /* Test 2 */
    /* Test free */

    printf("\n\nTest 2: ");

    {
        dllst(int) list;
        dllst_init(list);

        int cnt = 0;

        while (cnt < 10000000)
            dllst_push_back(list, cnt++);

        dllst_free(list);

        puts("Test deallocation.");
    }


    /* Test 3 */
    /* Test push */

    printf("\n\nTest 3: ");

    {
        typedef struct
        {
            char *msg[2];

        }   msg_struct;

        dllst(msg_struct) list;
        dllst_init(list);

        dllst_iter(msg_struct) iter;
        dllst_iter_init(iter, list);

        msg_struct msg_1, msg_2;

        msg_1.msg[0] = "Push";
        msg_1.msg[1] = "back";
        msg_2.msg[0] = "two";
        msg_2.msg[1] = "structs.\n";

        dllst_push_back(list, msg_1);
        dllst_push_back(list, msg_2);

        DLLST_TRAVERSAL(list, iter)
            printf("%s ", dllst_iter_dref(iter).msg[0]),
            printf("%s ", dllst_iter_dref(iter).msg[1]);

        dllst_free(list);
    }


    /* Test 4 */
    /* Test incr */

    printf("\n\nTest 4: ");

    {
        dllst(int) list;
        dllst_init(list);

        dllst_iter(int) iter;
        dllst_iter_init(iter, list);

        DLLST_TRAVERSAL(list, iter)
            printf("%d", dllst_iter_dref(iter));

        if (dllst_iter_incr(iter) == NULL)
            puts("The iterator points to the tail.");

        (void)dllst_iter_dref(iter);

        dllst_free(list);
    }


    /* Test 5 */
    /* Test access */

    printf("\n\nTest 5: ");

    {
        dllst(char*) list;
        dllst_init(list);

        dllst_iter(char*) iter;
        dllst_iter_init(iter, list);

        (void)dllst_front(list);
        (void)dllst_back(list);
        dllst_push_front(list, "Failed ");
        dllst_push_front(list, "to access.\n");
        dllst_back(list)  = "Element access ";
        dllst_front(list) = "succeed.\n";

        DLLST_BACKWARD_TRAVERSAL(list, iter)
            printf("%s", dllst_iter_dref(iter));

        dllst_free(list);
    }


    /* Test 6 */
    /* Test pop */

    printf("\n\nTest 6: ");

    {
        dllst(int) list;
        dllst_init(list);

        dllst_iter(int) iter;
        dllst_iter_init(iter, list);

        dllst_pop_front(list);

        dllst_push_front(list, 1);
        dllst_pop_front (list);

        dllst_push_back(list, 1);
        dllst_push_back(list, 2);
        dllst_pop_back (list);

        DLLST_TRAVERSAL(list, iter)
            printf("'%d' == '1'\n",  dllst_iter_dref(iter));

        dllst_free(list);
    }


    /* Test 7 */
    /* Test push and pop */

    printf("\n\nTest 7: \n");

    {
        dllst(int) list;
        dllst_init(list);

        puts("\nPushing back 10^7 elements...");
        for (int cnt = 0; cnt < 10000000; cnt++)
            dllst_push_back(list, cnt);
        puts("Clearing the dllst container...");
        dllst_clear(list);

        puts("\nPushing back 10^7 elements...");
        for (int cnt = 0; cnt < 10000000; cnt++)
            dllst_push_back(list, cnt);
        puts("Deallocating the dllst container...");
        dllst_free(list);
    }


    /* Test 8 */
    /* Test packed structure */

    printf("\n\nTest 8: \n\n");

    {
        dllst_pckd(char) list;
        dllst_init(list);

        dllst_iter(char) iter;
        dllst_iter_init(iter, list);

        for (int cnt = 0; cnt < 10000000; cnt++)
            dllst_push_back(list, cnt);

        char var = 0;

        DLLST_TRAVERSAL(list, iter)
            if (var != 127)  printf("%03d ", (var = dllst_iter_dref(iter)));
            else  break;

        puts("");

        dllst_free(list);
    }


    /* Test 9 */
    /* Test insert and erase */

    printf("\n\nTest 9: \n\n");

    {
        dllst_pckd(int) list;
        dllst_init(list);

        dllst_iter_pckd(int) iter;
        dllst_iter_init(iter, list);

        for (int cnt = 0; cnt < 6; cnt++)
            dllst_push_back(list, 2 * cnt + 1);

        DLLST_TRAVERSAL(list, iter)
            printf("%d ", dllst_iter_dref(iter));

        dllst_iter_head(iter, list);

        for (int cnt = 0; cnt <= 6; cnt++)
        {
            dllst_iter_incr(iter);
            dllst_insert(iter, 2 * cnt);
            dllst_iter_incr(iter);
        }

        printf("-> ");
        DLLST_TRAVERSAL(list, iter)
            printf("%d ", dllst_iter_dref(iter));

        dllst_iter_begin(iter, list);

        for (int cnt = 0; cnt <= 5; cnt++)
        {
            dllst_iter_incr(iter);
            dllst_erase(iter);
        }

        printf("-> ");
        DLLST_TRAVERSAL(list, iter)
            printf("%d ", dllst_iter_dref(iter));

        dllst_iter_begin(iter, list);

        for (int cnt = 0; cnt <= 5; cnt++)
            dllst_erase(iter);

        printf("-> ");
        DLLST_TRAVERSAL(list, iter)
            printf("%d ", dllst_iter_dref(iter));

        dllst_iter_begin(iter, list);

        dllst_erase(iter);

        puts("-> (none)");
        DLLST_TRAVERSAL(list, iter)
            printf("%d ", dllst_iter_dref(iter));

        dllst_free(list);
    }


    /* Test 10 */
    /* Test move */

    printf("\n\nTest 10: \n\n");

    {
        dllst(int) list;
        dllst_init(list);

        dllst_iter(int) iter, move[3];
        dllst_iter_init(iter, list);
        dllst_iter_init(move[0], list);
        dllst_iter_init(move[1], list);
        dllst_iter_init(move[2], list);

        int pos[8][3] = { {2, 4, 6} , {-1, 2, 0}, {0, -1, -2}, {1, -1, 2},
                          {-4, 0, 3}, {0, 2, 0} , {1, 0, -1} , {1, -1, 1} };

        for (int cnt = 0; cnt < 7; cnt++)
            dllst_push_back(list, cnt + 'A');

        dllst_iter_head(move[0], list);
        dllst_iter_head(move[1], list);
        dllst_iter_head(move[2], list);

        DLLST_TRAVERSAL(list, iter)
            printf("%c ", dllst_iter_dref(iter));

        for (int cnt = 0; cnt < 8; cnt++)
        {
            dllst_iter_advance(move[0], pos[cnt][0]);
            dllst_iter_advance(move[1], pos[cnt][1]);
            dllst_iter_advance(move[2], pos[cnt][2]);

            dllst_move_range(move[0], move[1], move[2]);

            printf("/ ");
            DLLST_TRAVERSAL(list, iter)
                printf("%c ", dllst_iter_dref(iter));
        }

        puts("= A C E D B F G /");

        dllst_free(list);
    }


    /* Test 11 */
    /* Test merge */

    printf("\n\nTest 11: \n\n");

    {
        dllst(int) list;
        dllst_init(list);

        dllst_iter(int) iter, iters[4];
        dllst_iter_init(iter, list);
        INCR_LOOP_(cnt, 4)  dllst_iter_init(iters[cnt], list);

        int str1[] = {0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15};

        for (int cnt = 0; cnt < 2; cnt++)
        {
            INCR_LOOP_(cnt, 16)  dllst_push_back(list, str1[cnt]);

            DLLST_TRAVERSAL(list, iter)
                printf("%d ", dllst_iter_dref(iter));

            printf("-> ");

            switch (cnt)
            {
                case 0:
                INCR_LOOP_(cnt, 3)  dllst_iter_begin(iters[cnt], list);
                INCR_LOOP_(cnt, 3)  dllst_iter_advance(iters[cnt], 8 * cnt);

                dllst_merge_range(iters[0], iters[1], iters[2], iters[3]);
                break;

                case 1:
                dllst_sort(list, iters);
                break;
            }

            DLLST_TRAVERSAL(list, iter)
                printf("%d ", dllst_iter_dref(iter));

            puts("");

            dllst_clear(list);
        }

        dllst_free(list);
    }


    /* Test 12 */
    /* Test sort */

    printf("\n\nTest 12: \n\n");

    {
        dllst(int) list;
        dllst_init(list);

        dllst_iter(int) iter, iters[4];
        dllst_iter_init(iter, list);
        INCR_LOOP_(cnt, 4)  dllst_iter_init(iters[cnt], list);

        int length = 1000;
        srand(time(NULL));

        INCR_LOOP_(cnt, length)  dllst_push_back(list, rand());

        dllst_sort(list, iters);

        DLLST_TRAVERSAL(list, iter)
            printf("Max = %10d\r", dllst_iter_dref(iter));

        puts("");

        dllst_free(list);
    }


    /* Test 13 */
    /* Test sort parallel */

    printf("\n\nTest 13: \n\n");

    {
        const int n = 2;

        dllst(int) list, lists[n];
        dllst_init(list);
        INCR_LOOP_(i, n) dllst_init(lists[i]);

        dllst_iter(int) iter, iters[n][4];
        dllst_iter_init(iter, list);
        INCR_LOOP2_(i, n, j, 4)  dllst_iter_init(iters[i][j], lists[i]);

        int length = 10000;
        INCR_LOOP_(cnt, length)  dllst_push_back(list, length - cnt);

        dllst_sort_openmp(list, lists, iters, n);

        DLLST_TRAVERSAL(list, iter)
            printf("Ten thousand = %d\r", dllst_iter_dref(iter));

        puts("");

        dllst_free(list);
        INCR_LOOP_(i, n)  dllst_free(lists[i]);
    }


    return 0;
}
