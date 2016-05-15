#include <stdio.h>
#include <stdlib.h>

#define  CC_DLLST_START 10
#define  CC_DLLST_RATIO 10
#define  CC_DLLST_THRSH 1000000
#include "../src/dllst.h"

#define INCR_LOOP_(_cnt, _num) for (int _cnt = 0; _cnt < (_num); _cnt++)


int main(void)
{
    /* Test 0 */
    /* For demonstration */

    printf("Test 0:\n\n");

    {
        cc_dllst(char*) msg;
        cc_dllst_init(msg);

        cc_dllst_iter(char*) msg_iter;
        cc_dllst_iter_init(msg_iter, msg);

        cc_dllst_push_back(msg, "Hello everyone:\n");
        cc_dllst_push_back(msg, "Welcome to C Container Collection Project!\n");

        INCR_TRAV_(msg, msg_iter)
            printf("%s", cc_dllst_iter_dref(msg_iter));

        cc_dllst_free(msg);
    }


    /* Test 1 */
    /* Push back ten pointers to char */

    printf("\n\nTest 1: \n");

    {
        cc_dllst(char*) list;
        cc_dllst_init(list);

        char *num_str[10] = { "five" , "six", "four", "seven", "three",
                              "eight", "two", "nine", "one"  , "ten"    };

        int cnt = 0;

        while (puts("") && cnt < 10)
        {
            cc_dllst_iter(char*) iter;
            cc_dllst_iter_init(iter, list);

            cc_dllst_push_front(list, num_str[cnt++]);
            cc_dllst_push_back (list, num_str[cnt++]);

            INCR_TRAV_(list, iter)
                printf("%s ", cc_dllst_iter_dref(iter));
        }

        cc_dllst_free(list);
    }


    /* Test 2 */
    /* Deallocation */

    printf("\n\nTest 2: ");

    {
        cc_dllst(int) list;
        cc_dllst_init(list);

        int cnt = 0;

        while (cnt < 10000000)
            cc_dllst_push_back(list, cnt++);

        cc_dllst_free(list);

        puts("Test deallocation.");
    }


    /* Test 3 */
    /* Push back two structs */

    printf("\n\nTest 3: ");

    {
        typedef struct
        {
            char *msg[2];
        } msg_struct;

        cc_dllst(msg_struct) list;
        cc_dllst_init(list);

        cc_dllst_iter(msg_struct) iter;
        cc_dllst_iter_init(iter, list);

        msg_struct msg_1, msg_2;

        msg_1.msg[0] = "Push";
        msg_1.msg[1] = "back";
        msg_2.msg[0] = "two";
        msg_2.msg[1] = "structs.\n";

        cc_dllst_push_back(list, msg_1);
        cc_dllst_push_back(list, msg_2);

        INCR_TRAV_(list, iter)
            printf("%s ", cc_dllst_iter_dref(iter).msg[0]),
            printf("%s ", cc_dllst_iter_dref(iter).msg[1]);

        cc_dllst_free(list);
    }


    /* Test 4 */
    /* Empty container */

    printf("\n\nTest 4: ");

    {
        cc_dllst(int) list;
        cc_dllst_init(list);

        cc_dllst_iter(int) iter;
        cc_dllst_iter_init(iter, list);

        INCR_TRAV_(list, iter)
            printf("%d", cc_dllst_iter_dref(iter));

        if (cc_dllst_iter_incr(iter) == NULL)
            puts("The iterator points to the tail.");

        (void)cc_dllst_iter_dref(iter);

        cc_dllst_free(list);
    }


    /* Test 5 */
    /* Element access */

    printf("\n\nTest 5: ");

    {
        cc_dllst(char*) list;
        cc_dllst_init(list);

        cc_dllst_iter(char*) iter;
        cc_dllst_iter_init(iter, list);

        (void)cc_dllst_front(list);
        (void)cc_dllst_back(list);
        cc_dllst_push_front(list, "Failed ");
        cc_dllst_push_front(list, "to access.\n");
        cc_dllst_back(list)  = "Element access ";
        cc_dllst_front(list) = "succeed.\n";

        DECR_TRAV_(list, iter)
            printf("%s", cc_dllst_iter_dref(iter));

        cc_dllst_free(list);
    }


    /* Test 6 */
    /* Pop elements */

    printf("\n\nTest 6: ");

    {
        cc_dllst(int) list;
        cc_dllst_init(list);

        cc_dllst_iter(int) iter;
        cc_dllst_iter_init(iter, list);

        cc_dllst_pop_front(list);

        cc_dllst_push_front(list, 1);
        cc_dllst_pop_front(list);

        cc_dllst_push_back(list, 1);
        cc_dllst_push_back(list, 2);
        cc_dllst_pop_back(list);
        INCR_TRAV_(list, iter)
            printf("Only '%d' remains in the container.\n",
                   cc_dllst_iter_dref(iter));

        cc_dllst_free(list);
    }


    /* Test 7 */
    /* Frequently push and pop elements */

    printf("\n\nTest 7: \n");

    {
        cc_dllst(int) list;
        cc_dllst_init(list);

        puts("\nPushing back 10^7 elements...");
        for (int cnt = 0; cnt < 10000000; cnt++)
            cc_dllst_push_back(list, cnt);
        puts("Clearing the dllst container...");
        cc_dllst_clear(list);

        puts("\nPushing back 10^7 elements...");
        for (int cnt = 0; cnt < 10000000; cnt++)
            cc_dllst_push_back(list, cnt);
        puts("Deallocating the dllst container...");
        cc_dllst_free(list);
    }


    /* Test 8 */
    /* Test packed structure */

    printf("\n\nTest 8: \n\n");

    {
        #pragma pack(push, 1)
        cc_dllst(char) list;
        cc_dllst_init(list);

        cc_dllst_iter(char) iter;
        cc_dllst_iter_init(iter, list);
        #pragma pack(pop)

        for (int cnt = 0; cnt < 10000000; cnt++)
            cc_dllst_push_back(list, cnt);

        INCR_TRAV_(list, iter)
        {
            char var;

            if ((var = cc_dllst_iter_dref(iter)) > 0)
                printf("%03d%c", var, (var != 127) ? ' ' : '\n');
            else if (var != 0)
                break;
        }

        cc_dllst_free(list);
    }


    /* Test 9 */
    /* Test insert and erase */

    printf("\n\nTest 9: \n\n");

    {
        cc_dllst(int) list;
        cc_dllst_init(list);

        cc_dllst_iter(int) iter;
        cc_dllst_iter_init(iter, list);

        for (int cnt = 0; cnt < 6; cnt++)
            cc_dllst_push_back(list, 2 * cnt + 1);

        INCR_TRAV_(list, iter)
            printf("%d ", cc_dllst_iter_dref(iter));

        cc_dllst_iter_head(iter, list);

        for (int cnt = 0; cnt <= 6; cnt++)
        {
            cc_dllst_iter_incr(iter);
            cc_dllst_insert(iter, 2 * cnt);
            cc_dllst_iter_incr(iter);
        }

        printf("-> ");
        INCR_TRAV_(list, iter)
            printf("%d ", cc_dllst_iter_dref(iter));

        cc_dllst_iter_begin(iter, list);

        for (int cnt = 0; cnt <= 5; cnt++)
        {
            cc_dllst_iter_incr(iter);
            cc_dllst_erase(iter);
        }

        printf("-> ");
        INCR_TRAV_(list, iter)
            printf("%d ", cc_dllst_iter_dref(iter));

        cc_dllst_iter_begin(iter, list);

        for (int cnt = 0; cnt <= 5; cnt++)
            cc_dllst_erase(iter);

        printf("-> ");
        INCR_TRAV_(list, iter)
            printf("%d ", cc_dllst_iter_dref(iter));

        cc_dllst_iter_begin(iter, list);

        cc_dllst_erase(iter);

        puts("-> (none)");
        INCR_TRAV_(list, iter)
            printf("%d ", cc_dllst_iter_dref(iter));

        cc_dllst_free(list);
    }


    /* Test 10 */
    /* Test move */

    printf("\n\nTest 10: \n\n");

    {
        cc_dllst(int) list;
        cc_dllst_init(list);

        cc_dllst_iter(int) iter, move[3];
        cc_dllst_iter_init(iter, list);
        cc_dllst_iter_init(move[0], list);
        cc_dllst_iter_init(move[1], list);
        cc_dllst_iter_init(move[2], list);

        int pos[8][3] = { {2, 4, 6} , {-1, 2, 0}, {0, -1, -2}, {1, -1, 2},
                          {-4, 0, 3}, {0, 2, 0} , {1, 0, -1} , {1, -1, 1} };

        for (int cnt = 0; cnt < 7; cnt++)
            cc_dllst_push_back(list, cnt + 'A');

        cc_dllst_iter_head(move[0], list);
        cc_dllst_iter_head(move[1], list);
        cc_dllst_iter_head(move[2], list);

        INCR_TRAV_(list, iter)
            printf("%c ", cc_dllst_iter_dref(iter));

        for (int cnt = 0; cnt < 8; cnt++)
        {
            cc_dllst_iter_advance(move[0], pos[cnt][0]);
            cc_dllst_iter_advance(move[1], pos[cnt][1]);
            cc_dllst_iter_advance(move[2], pos[cnt][2]);

            cc_dllst_move_range(move[0], move[1], move[2]);

            printf("/ ");
            INCR_TRAV_(list, iter)
                printf("%c ", cc_dllst_iter_dref(iter));
        }

        puts("= A C E D B F G /");

        cc_dllst_free(list);
    }


    /* Test 11 */
    /* Test merge */

    printf("\n\nTest 11: \n\n");

    {
        cc_dllst(int) list;
        cc_dllst_init(list);

        cc_dllst_iter(int) iter, move[4];
        cc_dllst_iter_init(iter, list);
        INCR_LOOP_(cnt, 4) cc_dllst_iter_init(move[cnt], list);

        int str1[] = {0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15};

        INCR_LOOP_(cnt, 16) cc_dllst_push_back(list, str1[cnt]);

        INCR_TRAV_(list, iter)
            printf("%d ", cc_dllst_iter_dref(iter));

        printf("-> ");

        INCR_LOOP_(cnt, 3) cc_dllst_iter_begin(move[cnt], list);
        INCR_LOOP_(cnt, 3) cc_dllst_iter_advance(move[cnt], 8 * cnt);

        cc_dllst_merge_range(move[0], move[1], move[2],
                             move[3], CC_DLLST_DEFAULT_COMP);

        INCR_TRAV_(list, iter)
            printf("%d ", cc_dllst_iter_dref(iter));

        puts("");

        cc_dllst_clear(list);

        cc_dllst_free(list);
    }


    return 0;
}
