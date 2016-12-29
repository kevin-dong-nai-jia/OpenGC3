#ifndef CC_MESG_H
#define CC_MESG_H

#include <stdio.h>
#include <stdlib.h>

#include "misc.h"


/* error and fatal messages */


const char CC_ERROR_MSG_MEMORY_LEAK[] = "Potential Memory Leak Problem.";
const char CC_ERROR_MSG_DOUBLE_FREE[] = "Potential Double Free Problem.";

#define CC_ERROR(CC_ERROR_MSG)                                                 \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    fprintf(stderr, "> CCC::ERROR: %s\n", CC_ERROR_MSG);                       \
)


const char CC_FATAL_MSG_MALLOC_FAIL[] = "Memory Space Allocation Failure.";
const int  CC_FATAL_MSG_MALLOC_FAIL_EXITCODE = -1;

#define CC_FATAL(CC_FATAL_MSG)                                                 \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    fprintf(stderr, "> CCC::FATAL: %s\n", CC_FATAL_MSG);                       \
    exit(CC_FATAL_MSG##_EXITCODE);                                             \
)



#endif
