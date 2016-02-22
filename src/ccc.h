#ifndef _CCC_H_
#define _CCC_H_



/* CCC syntax */


#define CCC_STATEMENT_(_ccc_statement)                                         \
                                                                               \
    do {_ccc_statement} while (0)


#define CCC_VOID_EXPR_(_ccc_expression)                                        \
                                                                               \
    ((_ccc_expression), ((void)0))



/* CCC constants */


#if (CCC_DLLST_START - 0 <= 0)

    #undef  CCC_DLLST_START
    #define CCC_DLLST_START 16

#endif


#if (CCC_DLLST_RATIO - 0 <= 0)

    #undef  CCC_DLLST_RATIO
    #define CCC_DLLST_RATIO 2

#endif


#if (CCC_DLLST_LIMIT - 0 <= CCC_DLLST_START)

    #undef  CCC_DLLST_LIMIT
    #define CCC_DLLST_LIMIT 65536

#endif


/* CCC header files */


#include "ccc/dllst.h"



#endif
