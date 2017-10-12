#ifndef OPENGC3_LIST_EXTD_CCSLL_H
#define OPENGC3_LIST_EXTD_CCSLL_H

#include "ccsll.h"
#include "extd-base.h"
#include "extd-ccdll.h"


/* ccsll operations extended */


#define  ccsll_merge_prefetch(_ccsll_d, _ccsll_s)                              \
                                                                               \
         ccsll_merge_prefetch_extd(_ccsll_d, _ccsll_s, SLEQ_NEXT)

#define  ccsll_merge_prefetch_extd(_ccsll_d, _ccsll_s, _leq)                   \
                                                                               \
         cc_ll_merge_extd         (_ccsll_d, _ccsll_s, _leq, ccsll, _prefetch)

#define _ccsll_merge_prefetch_extd(_iter_l, _iter_m, _iter_r, _leq)            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_l)->cont == (_iter_m)->cont ||                        \
                  (_iter_l)->cont == (_iter_r)->cont ||                        \
                  (_iter_m)->cont != (_iter_r)->cont))  break;                 \
                                                                               \
    int _ofs = 0;                                                              \
    __typeof__((_iter_l)->cont->pnode) _write, *_queue[64];                    \
                                                                               \
    for (int _idx = 0; _idx < 64; _idx++)                                      \
        (_queue)[_idx] = &_write;                                              \
                                                                               \
    ccsll_iter_head((_iter_l));                                                \
    ccsll_iter_head((_iter_m));                                                \
    ccsll_iter_head((_iter_r));                                                \
                                                                               \
    for (register int _len = 0; ; _len = 0)                                    \
    {                                                                          \
        while (!(ccsll_iter_at_end((_iter_l))) && _leq((_iter_l), (_iter_m)))  \
            (void)(ccdll_iter_incr_prefetch((_iter_l), _queue, &_ofs));        \
                                                                               \
        while (!(ccsll_iter_at_end((_iter_r))) &&                              \
                (ccsll_iter_at_end((_iter_l)) || !_leq((_iter_l), (_iter_r)))) \
            (void)(ccdll_iter_incr_prefetch((_iter_r), _queue, &_ofs), ++_len);\
                                                                               \
        ccsll_move_range_extd((_iter_l), (_iter_m), (_iter_r), _len);          \
                                                                               \
        if (ccsll_iter_at_end((_iter_m)))  break;                              \
                                                                               \
        ccsll_iter_copy((_iter_l), (_iter_r));                                 \
        ccsll_iter_init((_iter_r), (_iter_m)->cont) ;                          \
        ccsll_iter_head((_iter_r));                                            \
    }                                                                          \
)



/* ccsll integrity */


#define ccsll_is_sorted(_ccsll, _ptrue)                                        \
                                                                               \
        ccsll_is_sorted_extd(_ccsll, SLEQ, _ptrue)

#define ccsll_is_sorted_extd(_ccsll, _leq, _ptrue)                             \
                                                                               \
        cc_ll_is_sorted_extd(_ccsll, _leq, _ptrue, ccsll)


#define ccsll_is_robust(_ccsll, _ptrue)                                        \
                                                                               \
        cc_ll_is_robust(_ccsll, _ptrue, ccsll, CCSLL)



#endif
