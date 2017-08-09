#include "../../src/list/ccsll.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    // Test 0
    // Demonstration

    printf("Test 0: \n\n");

    {
        ccsll(char*) list;
        ccsll_init(list);

        ccsll_push_front(list, "the \"OpenGC3\" Project!\n");
        ccsll_push_front(list, "Welcome to ");

        CCSLL_INCR(ITER(list))
            printf("%s", SREF(ITER(list)));

        ccsll_free(list);
    }
}
