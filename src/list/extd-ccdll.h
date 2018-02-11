#ifndef OPENGC3_LIST_EXTD_CCDLL_H
#define OPENGC3_LIST_EXTD_CCDLL_H

#include "ccdll.h"
#include "extd-base.h"
#include "extd-ccsll.h"


/* ccdll operations extended */


#define  ccdll_sort_prefetch(_ccdll)                                           \
                                                                               \
         ccdll_sort_prefetch_extd(_ccdll, SLEQ_NEXT)

#define  ccdll_sort_prefetch_extd(_ccdll, _leq)                                \
                                                                               \
         cc_ll_sort_extd         (_ccdll, _leq, ccdll, _prefetch)

#define _ccdll_sort_prefetch_extd(_ccdll,  _carry,  _pbuck,                    \
                                  _iter_a, _iter_b, _leq)                      \
        _cc_ll_sort_extd         (_ccdll,  _carry,  _pbuck,                    \
                                  _iter_a, _iter_b, _leq,  ccsll, _prefetch)



/* ccdll iterators extended */


#define ccdll_iter_incr_prefetch(_iter, _pqueue)                               \
(                                                                              \
    *((_pqueue)->_arr[((_pqueue)->_ofs)]) =  ((_iter)->curr.node),             \
      (_pqueue)->_arr[((_pqueue)->_ofs)]  = &((_iter)->curr.node->PRV),        \
     ((_pqueue)->_ofs) = (((_pqueue)->_ofs) + 1) % ELEMOF((_pqueue)->_arr),    \
    (_prefetch((_iter)->curr.node->PRV)),                                      \
                                                                               \
    ccdll_iter_incr((_iter))                                                   \
)


#define ccdll_iter_incr_backlink(_iter, _pwrite)                               \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (void)(ccdll_iter_incr((_iter))),                                          \
                                                                               \
    (_prefetch((_iter)->curr.node->PRV)),                                      \
    (_iter)->curr.node->PRV = *(_pwrite),                                      \
    *(_pwrite) = ((_iter)->curr.node)                                          \
)



/* ccdll integrity */


#define ccdll_is_sorted(_ccdll, _ptrue)                                        \
                                                                               \
        ccdll_is_sorted_extd(_ccdll, DLEQ, _ptrue)

#define ccdll_is_sorted_extd(_ccdll, _leq, _ptrue)                             \
                                                                               \
        cc_ll_is_sorted_extd(_ccdll, _leq, _ptrue, ccdll)


#define ccdll_is_robust(_ccdll, _ptrue)                                        \
                                                                               \
        cc_ll_is_robust(_ccdll, _ptrue, ccdll, CCDLL)



#endif
