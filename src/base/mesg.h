#ifndef OPENGC3_BASE_MESG_H
#define OPENGC3_BASE_MESG_H

#include <stdio.h>
#include <stdlib.h>

#include "misc.h"


/* error and fatal messages */


static const char CC_ERROR_MSG_MEMORY_LEAK[] = "Potential Memory Leak.";
static const char CC_ERROR_MSG_DOUBLE_FREE[] = "Potential Double Free.";

#define CC_ERROR(CC_ERROR_MSG)                                                 \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    fprintf(stderr, "OpenGC^3::" "ERROR: %s\n", CC_ERROR_MSG);                 \
)


static const char CC_FATAL_MSG_MALLOC_FAIL[] = "Memory Allocation Failure.";
static const int  CC_FATAL_MSG_MALLOC_FAIL_EXITCODE = -1;

#define CC_FATAL(CC_FATAL_MSG)                                                 \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    fprintf(stderr, "OpenGC^3::" "FATAL: %s\n", CC_FATAL_MSG);                 \
    exit(CC_FATAL_MSG##_EXITCODE);                                             \
)



#endif
