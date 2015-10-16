#include <stdio.h>
#include <stdlib.h>

#include "../src/ccc/deque.h"


int main(void)
{
    /* Test 1 */
    /* Push back ten pointers to char */

    printf("Test 1:\n");

    {
        cc_deque(char*) test1 = cc_deque_init;

        char *num_str[10] = { "five" , "six", "four", "seven", "three",
                              "eight", "two", "nine", "one"  , "ten"    };

        int cnt = 0;

        while (cnt <= 10)
        {
            cc_deque_iter(char*) test1_iter = cc_deque_iter_begin(test1);

            cc_deque_trav(test1, test1_iter, char*)
                printf("%s ", **test1_iter);

            puts("");

            cc_deque_push_front(test1, char*, num_str[cnt++]);
            cc_deque_push_back (test1, char*, num_str[cnt++]);
        }

        cc_deque_dealloc(test1, char*);
    }

    /* Test 2 */
    /* Check deallocation (with valgrind) */

    printf("\nTest 2:\n\n");

    {
        cc_deque(int) test2 = cc_deque_init;

        int cnt = 0;

        while (cnt <= 10000000)
        {
            cc_deque_push_back(test2, int, cnt++);

            if (cnt % 1000 == 0)
                printf("%d\r", cnt);
        }

        cc_deque_dealloc(test2, int);
    }

    /* Test 3 */
    /* Push back two structs */

    printf("\n\nTest 3:\n\n");

    {
        typedef struct
        {
            char *msg[2];
        } test3_struct;

        cc_deque(test3_struct) test3 = cc_deque_init;
        cc_deque_iter(test3_struct) test3_iter = cc_deque_iter_init;

        test3_struct test3_1, test3_2;

        test3_1.msg[0] = "Push";
        test3_1.msg[1] = "back";
        test3_2.msg[0] = "two";
        test3_2.msg[1] = "structs.\n";

        cc_deque_push_back(test3, test3_struct, test3_1);
        cc_deque_push_back(test3, test3_struct, test3_2);

        cc_deque_trav(test3, test3_iter, test3_struct)
            printf("%s ", (**test3_iter).msg[0]),
            printf("%s ", (**test3_iter).msg[1]);

        cc_deque_dealloc(test3, test3_struct);
    }

    return 0;
}
