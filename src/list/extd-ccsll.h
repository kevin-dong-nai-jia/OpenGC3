#ifndef OPENGC3_LIST_EXTD_CCSLL_H
#define OPENGC3_LIST_EXTD_CCSLL_H

#include "ccsll.h"
#include "extd-base.h"
#include "extd-ccdll.h"


/* ccsll operations extended */


#define  ccsll_move_into_prefetch(_ccsll_d, _ccsll_s)                          \
                                                                               \
         cc_ll_move_into(_ccsll_d, _ccsll_s, ccsll, _prefetch, _fast)

#define _ccsll_move_into_prefetch(_iter_l, _iter_m, _iter_r /* ! */)           \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    ccsll_iter_end ((_iter_l));                                                \
    ccsll_iter_head((_iter_m));                                                \
                                                                               \
    (_iter_l)->cont->tail.PRV = (_iter_r)->curr.node;                          \
                                                                               \
    ccsll_move_range_extd((_iter_l), (_iter_m),                                \
                          (_iter_r),  ccsll_size((_iter_m)->cont));            \
                                                                               \
    (_iter_l)->cont->head.NXT->PRV = &((_iter_l)->cont->head);                 \
)


#define  ccsll_merge_prefetch(_ccsll_d, _ccsll_s)                              \
                                                                               \
         ccsll_merge_prefetch_extd(_ccsll_d, _ccsll_s, SLEQ_NEXT, 0)

#define  ccsll_merge_prefetch_extd(_ccsll_d, _ccsll_s, _leq, _last)            \
                                                                               \
         cc_ll_merge_extd         (_ccsll_d, _ccsll_s, _leq,                   \
                                    ccsll, _prefetch, _fast, _last)

#define _ccsll_merge_prefetch_extd(_iter_l, _iter_m, _iter_r, _leq, _last)     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_l)->cont == (_iter_m)->cont ||                        \
                  (_iter_l)->cont == (_iter_r)->cont ||                        \
                  (_iter_m)->cont != (_iter_r)->cont))  break;                 \
                                                                               \
    __typeof__((_iter_l)->cont->pnode) _write;                                 \
    struct {  size_t _ofs;                                                     \
            __typeof__((_iter_l)->cont->pnode) *_arr[16]; } _queue;            \
                                                                               \
    _queue._ofs = 0;                                                           \
    for (size_t _idx = 0; _idx < ELEMOF(_queue._arr); _idx++)                  \
        _queue._arr[_idx] = &_write;                                           \
                                                                               \
    ccsll_iter_head((_iter_l));                                                \
    ccsll_iter_head((_iter_m));                                                \
    ccsll_iter_head((_iter_r));                                                \
                                                                               \
    for (register int _len = 0; ; _len = 0)                                    \
    {                                                                          \
        if (!(_last))                                                          \
        {                                                                      \
            while (!(ccsll_iter_at_end((_iter_l))) &&                          \
                    _leq((_iter_l), (_iter_m)))                                \
                ((void)ccdll_iter_incr_prefetch((_iter_l), &_queue));          \
                                                                               \
            while (!(ccsll_iter_at_end((_iter_r))) &&                          \
                    (ccsll_iter_at_end((_iter_l))  ||                          \
                      !_leq((_iter_l), (_iter_r))))                            \
                ((void)ccdll_iter_incr_prefetch((_iter_r), &_queue), ++_len);  \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            while (!(ccsll_iter_at_end((_iter_l))) &&                          \
                    _leq((_iter_l), (_iter_m)))                                \
                ((void)ccdll_iter_incr_backlink((_iter_l), &_write));          \
                                                                               \
            while (!(ccsll_iter_at_end((_iter_r))) &&                          \
                    (ccsll_iter_at_end((_iter_l))  ||                          \
                      !_leq((_iter_l), (_iter_r))))                            \
                ((void)ccdll_iter_incr_backlink((_iter_r), &_write), ++_len);  \
        }                                                                      \
                                                                               \
        ccsll_move_range_extd((_iter_l), (_iter_m), (_iter_r), _len);          \
                                                                               \
        if (ccsll_iter_at_end((_iter_m)))                                      \
        {                                                                      \
            ccsll_iter_copy((_iter_l), (_iter_r));                             \
                                                                               \
            while (!(ccsll_iter_at_end((_iter_l))))                            \
                (void)(ccdll_iter_incr_backlink((_iter_l), &_write));          \
            break;                                                             \
        }                                                                      \
                                                                               \
        ccsll_iter_copy((_iter_l), (_iter_r));                                 \
        ccsll_iter_init((_iter_r), (_iter_m)->cont, 0);                        \
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
STATEMENT_                                                                     \
(                                                                              \
    int _size;                                                                 \
    *(_ptrue) = 1;                                                             \
                                                                               \
    _size = 0;                                                                 \
    CCSLL_INCR((_ccsll)->_iter)  _size++;                                      \
    *(_ptrue) &= !!(ccsll_size((_ccsll)) == _size);                            \
)



#endif
