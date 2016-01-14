#include <stdio.h>
#include <stdlib.h>

#include "../src/ccc/dllst.h"


int main(void)
{
    /* Test 0 */
    /* For demonstration */

    printf("Test 0:\n\n");

    {
        cc_dllst(msg, char*);
        cc_dllst_iter(msg_iter, msg);

        cc_dllst_push_back(msg, "Hello everyone:\n");
        cc_dllst_push_back(msg, "Welcome to C Container Collection Project!\n");

        cc_dllst_trav(msg_iter, msg)
            printf("%s", ***msg_iter);

        cc_dllst_dealloc(msg);
    }


    /* Test 1 */
    /* Push back ten pointers to char */

    printf("\n\nTest 1:");

    {
        cc_dllst(test1, char*);

        char *num_str[10] = { "five" , "six", "four", "seven", "three",
                              "eight", "two", "nine", "one"  , "ten"    };

        int cnt = 0;

        while (puts("") && cnt <= 10)
        {
            cc_dllst_iter(test1_iter, test1);

            cc_dllst_trav(test1_iter, test1)
                printf("%s ", ***test1_iter);

            cc_dllst_push_front(test1, num_str[cnt++]);
            cc_dllst_push_back (test1, num_str[cnt++]);
        }

        cc_dllst_dealloc(test1);
    }


    /* Test 2 */
    /* Check deallocation */

    printf("\n\nTest 2: (no output)\n");

    {
        cc_dllst(test2, int);

        int cnt = 0;

        while (cnt <= 10000000)
            cc_dllst_push_back(test2, cnt++);

        cc_dllst_dealloc(test2);
    }


    /* Test 3 */
    /* Push back two structs */

    printf("\n\nTest 3: ");

    {
        typedef struct
        {
            char *msg[2];
        } test3_struct;

        cc_dllst(test3, test3_struct);
        cc_dllst_iter(test3_iter, test3);

        test3_struct test3_1, test3_2;

        test3_1.msg[0] = "Push";
        test3_1.msg[1] = "back";
        test3_2.msg[0] = "two";
        test3_2.msg[1] = "structs.\n";

        cc_dllst_push_back(test3, test3_1);
        cc_dllst_push_back(test3, test3_2);

        cc_dllst_trav(test3_iter, test3)
            printf("%s ", (***test3_iter).msg[0]),
            printf("%s ", (***test3_iter).msg[1]);

        cc_dllst_dealloc(test3);
    }


    return 0;
}
