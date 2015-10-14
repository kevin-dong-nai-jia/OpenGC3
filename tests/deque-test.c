#include <stdio.h>
#include <stdlib.h>

#include "../ccc/deque.h"


int main(void)
{
    /* Test 1 */

    printf("Test 1:\n");

    {
        cc_deque(char*) test1 = cc_deque_init;

        char *num_str[10] = { "five" , "six", "four", "seven", "three",
                              "eight", "two", "nine", "one"  , "ten"    };

        int cnt = 0;

        while (cnt <= 10)
        {
            cc_deque_iter(char*) test1_iter = cc_deque_iter_begin(test1);

            cc_deque_traversal(test1, test1_iter, char*)
                printf("%s ", **test1_iter);

            puts("");

            cc_deque_push_front(test1, char*, num_str[cnt++]);
            cc_deque_push_back (test1, char*, num_str[cnt++]);
        }
    }

    return 0;
}
