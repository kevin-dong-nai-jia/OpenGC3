#ifndef OPENGC3_VECT_CCARR_H
#define OPENGC3_VECT_CCARR_H

#include "../base/misc.h"
#include "../base/snym.h"

#include <limits.h>
#include <string.h>


/* ccarr create */


#define ccarr(width, _size)                                                    \
                                                                               \
        typedef ccarr_struct(width, _size) CCARR;  CCARR

#define ccarr_type(width, _size)                                               \
                                                                               \
        typedef ccarr_struct(width, _size)

#define ccarr_struct(width, _size)                                             \
                                                                               \
        struct CCARR_CONT  { size_t ecnt; uint##width##_t arr[_size]; }

#define CCARR_INIT(_ccarr) { ELEMOF((_ccarr).arr), {0} }


/* ccarr initialize */

#define ccarr_init(_ccarr)                                                     \
                                                                               \
        ccarr_init_extd(_ccarr, ELEMOF((_ccarr).arr))

#define ccarr_init_extd(_ccarr, _ecnt)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccarr).ecnt = (_ecnt),                                                   \
    memset((_ccarr).arr, 0, sizeof((_ccarr).arr[0]) * (_ecnt))                 \
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
    for (size_t _nth = 0, _dne = 0; _nth < (_ccarr).ecnt && !_dne; )           \
        _dne = !(ELEM_NTH((_ccarr), _nth++)++ == UMAXOF(ELEM((_ccarr))));      \
)


#define ccarr_incr_val(_ccarr, _val)                /* TODO : OPTIMIZE */      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    for (uint64_t _cnt = 0; _cnt < (_val); _cnt++)  ccarr_incr((_ccarr));      \
)


#define ccarr_xor_arr(_ccarr_a, _ccarr_b, _out_ccarr)                          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    for (size_t _nth = 0; _nth < (_ccarr_a).ecnt; _nth++)                      \
        (_out_ccarr)->arr[_nth] = (_ccarr_a).arr[_nth] ^ (_ccarr_b).arr[_nth]; \
)


#define ccarr_ham_arr(_ccarr_a, _ccarr_b, _out_ham)                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    for (size_t _nth = 0; _nth < (_ccarr_a).ecnt; _nth++)                      \
        for (uint64_t _xor = (_ccarr_a).arr[_nth] ^                            \
                             (_ccarr_b).arr[_nth]; _xor; _xor >>= 1)           \
            *(_out_ham) += !!(_xor % 2);                                       \
)


#define ccarr_set_bit(_ccarr, _nth_bit)                                        \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccarr).arr[(_nth_bit) / BITSOF((_ccarr).arr[0])] |=                      \
          (1 << ((_nth_bit) % BITSOF((_ccarr).arr[0])))                        \
)


#define ccarr_check_bit(_ccarr, _nth_bit) !!                                   \
(                                                                              \
    (_ccarr).arr[(_nth_bit) / BITSOF((_ccarr).arr[0])]                         \
        & (1 << ((_nth_bit) % BITSOF((_ccarr).arr[0])))                        \
)


#endif
