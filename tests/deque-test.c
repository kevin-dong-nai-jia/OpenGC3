#include <stdio.h>
#include <stdlib.h>

#include "../src/ccc/deque.h"


int main(void)
{
    /* Test 1 */
    /* Push back ten pointers to char */

    printf("Test 1:");

    {
        cc_deque(test1, char*);

        char *num_str[10] = { "five" , "six", "four", "seven", "three",
                              "eight", "two", "nine", "one"  , "ten"    };

        int cnt = 0;

        while (puts("") && cnt <= 10)
        {
            cc_deque_iter(test1_iter, test1);
            test1_iter = cc_deque_iter_begin(test1);

            cc_deque_trav(test1_iter, test1)
                printf("%s ", **test1_iter);

            cc_deque_push_front(test1, num_str[cnt++]);
            cc_deque_push_back (test1, num_str[cnt++]);
        }

        cc_deque_dealloc(test1);
    }


    /* Test 2 */
    /* Check deallocation */

    printf("\n\nTest 2: (no output)\n");

    {
        cc_deque(test2, int);

        int cnt = 0;

        while (cnt <= 10000000)
            cc_deque_push_back(test2, cnt++);

        cc_deque_dealloc(test2);
    }


    /* Test 3 */
    /* Push back two structs */

    printf("\n\nTest 3: ");

    {
        typedef struct
        {
            char *msg[2];
        } test3_struct;

        cc_deque(test3, test3_struct);
        cc_deque_iter(test3_iter, test3);

        test3_struct test3_1, test3_2;

        test3_1.msg[0] = "Push";
        test3_1.msg[1] = "back";
        test3_2.msg[0] = "two";
        test3_2.msg[1] = "structs.\n";

        cc_deque_push_back(test3, test3_1);
        cc_deque_push_back(test3, test3_2);

        cc_deque_trav(test3_iter, test3)
            printf("%s ", (**test3_iter).msg[0]),
            printf("%s ", (**test3_iter).msg[1]);

        cc_deque_dealloc(test3);
    }


    return 0;
}
