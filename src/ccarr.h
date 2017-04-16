#ifndef CC_ARR_H
#define CC_ARR_H

#include "misc.h"
#include "snym.h"

#include <limits.h>
#include <string.h>


/* synonym */

#define CCARR       ADDID(CCARR)
#define CCARR_CONT  ADDID(CCARR_CONT)


/* ccarr create */


#define ccarr(width, size)                                                     \
                                                                               \
    typedef ccarr_struct(width, size) CCARR;  CCARR

#define ccarr_struct(width, size)                                              \
                                                                               \
    struct CCARR_CONT  {  uint##width##_t arr[(size)];  }

#define CCARR_INIT  {{0}}


/* ccarr initialize */

#define ccarr_init(_ccarr)                                                     \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    memset((_ccarr).arr, 0, sizeof((_ccarr).arr))                              \
)


/* ccarr modifiers */

#define ccarr_or_val(_ccarr, _val)                                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    const size_t _seg =       BITSOF((_ccarr).arr[0]);                         \
    const size_t _min = MIN_2(BITSOF((_ccarr).arr), BITSOF(_val));             \
                                                                               \
    for (size_t _bit = 0; _bit < _min; _bit += _seg)                           \
        (_ccarr).arr[_bit / _seg] |= (_val) >> _bit & UMAXOF(ELEM((_ccarr)));  \
)


#define ccarr_incr(_ccarr)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    for (size_t _nth = 0, _dne = 0; _nth < ELEMOF((_ccarr).arr) && !_dne; )    \
        _dne = !(ELEM_NTH((_ccarr), _nth++)++ == UMAXOF(ELEM((_ccarr))));      \
)


#endif
