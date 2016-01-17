#ifndef _CCC_DEBUG_H_
#define _CCC_DEBUG_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>



void* __cc_message(char* file, int line, char* type, int count, ...)
{
    #ifdef CCC_DEBUG

    va_list ap;

    va_start(ap, count);

    fprintf(stderr, "\n[ CCC ] %s:%d:", file, line);
    fprintf(stderr, "\n[ CCC ] %s: ", type);

    while (count--)
        fprintf(stderr, "%s%s", va_arg(ap, char*), count ? " " : "\n");

    va_end(ap);

    if (strcmp(type, "ERROR") == 0)
        exit(-1);

    #else

    (void)file, (void)line, (void)type, (void)count;

    #endif

    return NULL;
}


#define __cc_warning_dllst_empty(_cc_dllst_object)                         \
(                                                                          \
    __cc_message(__FILE__, __LINE__, "WARNING", 3,                         \
                 #_cc_dllst_object, "(a dllst container)", "is empty.")    \
)


#define __cc_warning_dllst_iter_is_invalid(_cc_dllst_iter)                 \
(                                                                          \
    __cc_message(__FILE__, __LINE__, "WARNING", 3,                         \
                 #_cc_dllst_iter, "(a dllst iterator)", "is invalid.")     \
)



#endif
