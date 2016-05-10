#include <stdio.h>
#include <stdlib.h>

#define CCC_DLLST_START 10
#define CCC_DLLST_RATIO 10
#define CCC_DLLST_LIMIT 1000000

#include "../src/ccc.h"


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

        cc_dllst_trav(msg, msg_iter)
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

            cc_dllst_trav(list, iter)
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

        cc_dllst_trav(list, iter)
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

        cc_dllst_trav(list, iter)
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

        cc_dllst_trav_back(list, iter)
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
        cc_dllst_trav(list, iter)
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

        cc_dllst_trav(list, iter)
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

        cc_dllst_trav(list, iter)
            printf("%d ", cc_dllst_iter_dref(iter));

        cc_dllst_iter_head(iter, list);

        for (int cnt = 0; cnt <= 6; cnt++)
        {
            cc_dllst_iter_incr(iter);
            cc_dllst_insert(iter, 2 * cnt);
            cc_dllst_iter_incr(iter);
        }

        printf("-> ");
        cc_dllst_trav(list, iter)
            printf("%d ", cc_dllst_iter_dref(iter));

        cc_dllst_iter_begin(iter, list);

        for (int cnt = 0; cnt <= 5; cnt++)
        {
            cc_dllst_iter_incr(iter);
            cc_dllst_erase(iter);
        }

        printf("-> ");
        cc_dllst_trav(list, iter)
            printf("%d ", cc_dllst_iter_dref(iter));

        cc_dllst_iter_begin(iter, list);

        for (int cnt = 0; cnt <= 5; cnt++)
            cc_dllst_erase(iter);

        printf("-> ");
        cc_dllst_trav(list, iter)
            printf("%d ", cc_dllst_iter_dref(iter));

        cc_dllst_iter_begin(iter, list);

        cc_dllst_erase(iter);

        puts("-> (none)");
        cc_dllst_trav(list, iter)
            printf("%d ", cc_dllst_iter_dref(iter));

        cc_dllst_free(list);
    }


    /* Test 10 */
    /* Test splice */

    printf("\n\nTest 10: \n\n");

    {
        cc_dllst(int) list;
        cc_dllst_init(list);

        cc_dllst_iter(int) print, iter[3];
        cc_dllst_iter_init(print, list);
        cc_dllst_iter_init(iter[0], list);
        cc_dllst_iter_init(iter[1], list);
        cc_dllst_iter_init(iter[2], list);

        int pos[8][3] = { {2, 4, 6} , {-1, 2, 0}, {0, -1, -2}, {1, -1, 2},
                          {-4, 0, 3}, {0, 2, 0} , {1, 0, -1} , {1, -1, 1} };

        for (int cnt = 0; cnt < 7; cnt++)
            cc_dllst_push_back(list, cnt + 'A');

        cc_dllst_iter_head(iter[0], list);
        cc_dllst_iter_head(iter[1], list);
        cc_dllst_iter_head(iter[2], list);

        cc_dllst_trav(list, print)
            printf("%c ", cc_dllst_iter_dref(print));

        for (int cnt = 0; cnt < 8; cnt++)
        {
            cc_dllst_iter_advance(iter[0], pos[cnt][0]);
            cc_dllst_iter_advance(iter[1], pos[cnt][1]);
            cc_dllst_iter_advance(iter[2], pos[cnt][2]);

            cc_dllst_splice_range(iter[0], iter[1], iter[2]);

            printf("/ ");
            cc_dllst_trav(list, print)
                printf("%c ", cc_dllst_iter_dref(print));
        }

        puts("= A C E D B F G /");

        cc_dllst_free(list);
    }


    return 0;
}
