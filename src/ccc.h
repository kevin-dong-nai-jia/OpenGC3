#ifndef _CCC_H_
#define _CCC_H_



/* CCC properties */


#define CCC_STATEMENT_(_ccc_statement)                                     \
                                                                           \
    do {_ccc_statement} while (0)


#define CCC_VOID_EXPR_(_ccc_expression)                                    \
                                                                           \
    ((_ccc_expression), ((void)0))



/* CCC header files */


#include "ccc/dllst.h"



#endif
