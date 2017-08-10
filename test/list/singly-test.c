#if defined(PREFIX_CCSLL)
    #include "../../src/list/ccsll.h"
    #define prefix(func)  ccsll##func
    #define PREFIX(FUNC)  CCSLL##FUNC
#else
    #error "UNDEFINE PREFIX_CC[S]LL"
#endif


#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(void)
{
    // Test 0
    // Demonstration

    printf("Test 0: \n\n");

    {
        prefix()(char*) list;
        prefix(_init)(list);

        prefix(_push_front)(list, "the \"OpenGC3\" Project!\n");
        prefix(_push_front)(list, "Welcome to ");

        PREFIX(_INCR)(ITER(list))
            printf("%s", SREF(ITER(list)));

        prefix(_free)(list);
    }
}
