#ifndef _CC_MSG_H_
#define _CC_MSG_H_

#include <stdio.h>
#include <stdlib.h>

#include "misc.h"


/* error and fatal messages */


#define _CC_ERROR_MSG_MEMORY_LEAK          "Potential Memory Leak Problem."
#define _CC_ERROR_MSG_DOUBLE_FREE          "Potential Double Free Problem."

#define _CC_ERROR(_CC_ERROR_MSG)                                               \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    fprintf(stderr, "> CCC::ERROR: %s\n", _CC_ERROR_MSG);                      \
)


#define _CC_FATAL_MSG_MALLOC_FAIL          "Memory Space Allocation Failure."
#define _CC_FATAL_MSG_MALLOC_FAIL_EXITCODE (-1)

#define _CC_FATAL(_CC_FATAL_MSG)                                               \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    fprintf(stderr, "> CCC::FATAL: %s\n", _CC_FATAL_MSG);                      \
    exit(_CC_FATAL_MSG##_EXITCODE);                                            \
)



#endif
