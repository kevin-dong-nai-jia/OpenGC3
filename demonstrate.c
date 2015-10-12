#include <stdio.h>
#include <stdlib.h>

#include "ccc/deque.h"


int main(void)
{
    {
        cc_deque(char*) msg = cc_deque_init;
        cc_deque(int)  date = cc_deque_init;

        cc_deque_push_back(msg, char*, "Hello Everyone:\n");
        cc_deque_push_back(msg, char*, "C-Container-Collection Project ");
        cc_deque_push_back(msg, char*, "is started on ");

        cc_deque_push_back(date, int, 20151010);

        {
            cc_deque_iter(char*) msg_iter = cc_deque_iter_init;
            cc_deque_iter(int)  date_iter = cc_deque_iter_begin(date);

            cc_deque_traversal(msg, msg_iter, char*)
                printf("%s", **msg_iter);

            printf("%d! ;-)\n", **date_iter);
        }
    }

    return 0;
}
