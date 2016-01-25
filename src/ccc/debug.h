#ifndef _CCC_DEBUG_H_
#define _CCC_DEBUG_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>



void* __cc_message(char* file, int line, char* type, int code, int count, ...)
{
    #ifdef CCC_DEBUG

    va_list ap;

    va_start(ap, count);

    int is_error = (strcmp(type, "ERROR") == 0);

    fprintf(stderr, "\n[ CCC ] %s:%d:", file, line);
    fprintf(stderr, "\n[ CCC ] %s: %s", type, is_error ? "  " : "");

    while (count--)
        fprintf(stderr, "%s%s", va_arg(ap, char*), count ? " " : "\n");

    va_end(ap);

        #ifdef CCC_DEBUG_STRICT

        if (is_error)
        {
            fprintf(stderr, "[ CCC ] TERMINATED: return status %d\n\n", code);
            exit(code);
        }

        #else

        (void)code;

        #endif

    #else

    (void)file, (void)line, (void)type, (void)code, (void)count;

    #endif

    return NULL;
}



/* CCC Warning (print the warning message if CCC_DEBUG is set) */


#define __cc_warning_dllst_is_empty(_cc_dllst_object)                      \
(                                                                          \
    __cc_message(__FILE__, __LINE__, "WARNING", 0,                         \
                 3, #_cc_dllst_object, "(a dllst container)", "is empty.") \
)


#define __cc_warning_dllst_iter_is_invalid(_cc_dllst_iter)                 \
(                                                                          \
    __cc_message(__FILE__, __LINE__, "WARNING", 0,                         \
                 3, #_cc_dllst_iter, "(a dllst iterator)", "is invalid.")  \
)



/* CCC Error (terminate the process if CCC_DEBUG_STRICT is set) */


#define __cc_error_dllst_iter_cannot_be_dereferenced(_cc_dllst_iter)       \
(                                                                          \
    __cc_message(__FILE__, __LINE__, "ERROR", -1,                          \
                 3, #_cc_dllst_iter, "(a dllst iterator)",                 \
                 "cannot be dereferenced.")                                \
)



#endif
