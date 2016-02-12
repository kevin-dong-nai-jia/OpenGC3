#ifndef _CCC_CCC_H_
#define _CCC_CCC_H_



/* Statement */


#define CCC_STATEMENT_(_ccc_statement)                                     \
                                                                           \
    do {_ccc_statement} while (0)



/* Bitwise operations */


#define _cc_xor_2_addrs(_cc_addr_a, _cc_addr_b)                            \
(                                                                          \
    (void*)((uintptr_t)(void*)(_cc_addr_a) ^                               \
            (uintptr_t)(void*)(_cc_addr_b))                                \
)


#define _cc_xor_3_addrs(_cc_addr_a, _cc_addr_b, _cc_addr_c)                \
(                                                                          \
    (void*)((uintptr_t)(void*)(_cc_addr_a) ^                               \
            (uintptr_t)(void*)(_cc_addr_b) ^                               \
            (uintptr_t)(void*)(_cc_addr_c))                                \
)



/* Header files */


#include "ccc/dllst.h"



#endif
