#include <stdio.h>
#include <stdlib.h>

#define CCC_DEBUG
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

        cc_dllst_trav(msg, msg_iter)
            printf("%s", ***msg_iter);

        cc_dllst_dealloc(msg);
    }


    /* Test 1 */
    /* Push back ten pointers to char */

    printf("\n\nTest 1: \n");

    {
        cc_dllst(test1, char*);

        char *num_str[10] = { "five" , "six", "four", "seven", "three",
                              "eight", "two", "nine", "one"  , "ten"    };

        int cnt = 0;

        while (puts("") && cnt < 10)
        {
            cc_dllst_iter(test1_iter, test1);

            cc_dllst_push_front(test1, num_str[cnt++]);
            cc_dllst_push_back (test1, num_str[cnt++]);

            cc_dllst_trav(test1, test1_iter)
                printf("%s ", ***test1_iter);
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

        cc_dllst_trav(test3, test3_iter)
            printf("%s ", (***test3_iter).msg[0]),
            printf("%s ", (***test3_iter).msg[1]);

        cc_dllst_dealloc(test3);
    }


    /* Test 4 */
    /* Test debug mode */

    printf("\n\nTest 4: \n");

    {
        cc_dllst(test4, int);
        cc_dllst_iter(test4_iter, test4);

        cc_dllst_trav(test4, test4_iter)
            printf("%d", ***test4_iter);

        if (cc_dllst_iter_incr(test4_iter) == NULL)
            cc_dllst_iter_incr(test4_iter);

        (void)cc_dref(test4_iter);

        cc_dllst_dealloc(test4);
    }


    /* Test 5 */
    /* Element access */

    printf("\n\nTest 5: \n");

    {
        cc_dllst(test5, char*);
        cc_dllst_iter(test5_iter, test5);

        (void)cc_dllst_front(test5);
        (void)cc_dllst_back(test5);
        cc_dllst_push_front(test5, "\nFailed ");
        cc_dllst_push_front(test5, "to access.\n");
        cc_dllst_back(test5)  = "\nElement Access ";
        cc_dllst_front(test5) = "Succeed\n";

        cc_dllst_trav_rev(test5, test5_iter)
            printf("%s", ***test5_iter);

        cc_dllst_dealloc(test5);
    }


    return 0;
}
