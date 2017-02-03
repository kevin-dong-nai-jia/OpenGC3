// #define CCC_STRICT
#include "../src/ccdll.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    // Test 0
    // Demonstration

    printf("Test 0: \n\n");

    {
        ccdll(char*) list;
        ccdll_init(list);

        ccdll_push_back(list, "Welcome to ");
        ccdll_push_back(list, "the \"C Container Collection\" Project!\n");

        CCDLL_INCR(ITER(list))
            printf("%s", DREF(ITER(list)));

        ccdll_free(list);
    }


    return 0;
}
