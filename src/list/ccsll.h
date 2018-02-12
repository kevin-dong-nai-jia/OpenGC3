#ifndef OPENGC3_LIST_CCSLL_H
#define OPENGC3_LIST_CCSLL_H

#include "base.h"
#include "../base/pool.h"
#include "../base/misc.h"
#include "../base/snym.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


/* ccsll create */


#define ccsll(elem_t)                                                          \
                                                                               \
        ccsll_extd(elem_t, 1, NORMAL)

#define ccsll_pckd(elem_t)                                                     \
                                                                               \
        ccsll_extd(elem_t, 1, PACKED)

#define ccsll_extd(elem_t, _n_iter, _ALIGN_)                                   \
                                                                               \
        typedef ccsll_struct_extd(elem_t, _n_iter, _ALIGN_) *CCSLL;  CCSLL

#define ccsll_type(elem_t)                                                     \
                                                                               \
        typedef ccsll_struct_extd(elem_t, 1, NORMAL) *


#define ccsll_struct(elem_t)                                                   \
                                                                               \
        ccsll_struct_extd(elem_t, 1, NORMAL)

#define ccsll_struct_pckd(elem_t)                                              \
                                                                               \
        ccsll_struct_extd(elem_t, 1, PACKED)

#define ccsll_struct_extd(elem_t, _n_iter, _ALIGN_)                            \
                                                                               \
    struct CCSLL_CONT                                                          \
    {                                                                          \
        int size,  last,  vcnt;                   /* size and node record */   \
        int start, ratio, thrsh;                  /* block increment info */   \
                                                                               \
        struct CCSLL_NODE                                                      \
        {   struct CCSLL_NODE *lnk[1];                                         \
            elem_t val;                           /* val with a next link */   \
        }   *avsp, *pnode, head, tail, swap;      /* available space list */   \
                                                                               \
        struct CCSLL_BLCK                                                      \
        {   struct CCSLL_BLCK *bprv, *bnxt;       /* points to prev block */   \
            int ncnt;                             /* the item of the node */   \
            PRAGMA_##_ALIGN_##_BGN                /* packed pragma starts */   \
            struct CCSLL_NODE nodes[];            /* node structure array */   \
            PRAGMA_##_ALIGN_##_END                /* the pragma ends here */   \
        }   *pool, *pblock;                       /* points to 1-st block */   \
                                                                               \
        struct CCSLL_ITER                                                      \
        {   struct CCSLL_PTRS                                                  \
            {   struct CCSLL_NODE *node;                                       \
            }   curr;                             /* points to curr  node */   \
            struct CCSLL_CONT *cont;              /* points to ccsll body */   \
        }   (*itarr)[_n_iter], *_iter, **_it;                                  \
                                                                               \
        struct CCSLL_CONT **_co;                  /* internal use _it _co */   \
                                                                               \
        int _it_base, _it_limit;                                               \
        int _co_base, _co_limit;                                               \
    }



/* ccsll initialize */


#define ccsll_init(_ccsll)                                                     \
                                                                               \
        ccsll_init_extd(_ccsll,     16,      2,  65536)

#define ccsll_init_extd(_ccsll, _start, _ratio, _thrsh)                        \
                                                                               \
        cc_ll_init_extd(_ccsll, _start, _ratio, _thrsh, ccsll)


#define _ccsll_init(_ccsll_dst, _ccsll_src, _alloc)                            \
                                                                               \
        _ccsll_init_extd(_ccsll_dst, -1,     -1,     -1, _alloc)

#define _ccsll_init_extd(_ccsll, _start, _ratio, _thrsh, _alloc)               \
                                                                               \
        _cc_ll_init_extd(_ccsll, _start, _ratio, _thrsh, _alloc, ccsll)


#define _ccsll_init_core(_ccsll, _alloc)                                       \
                                                                               \
        _cc_ll_init_core(_ccsll, _alloc, ccsll)


#define _ccsll_init_seed(_ccsll)                                               \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccsll)->size = 0,                                                        \
    (_ccsll)->last = (_ccsll)->vcnt = 0,                                       \
                                                                               \
    (_ccsll)->head.NXT = &((_ccsll)->tail),                                    \
    (_ccsll)->tail.NXT = NULL                                                  \
)


#define _ccsll_init_info(_ccsll, _start, _ratio, _thrsh)                       \
                                                                               \
        _cc_ll_init_info(_ccsll, _start, _ratio, _thrsh)


#define ccsll_iter_init(_iter, _ccsll, _alloc)                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = ((_alloc) ? NULL : (_iter)->curr.node),               \
    (_iter)->cont = (_ccsll)                                                   \
)


#define _ccsll_iter_init(_iter, _ccsll, _alloc)                                \
                                                                               \
        _cc_ll_iter_init(_iter, _ccsll, _alloc, ccsll)



/* ccsll destroy */


#define ccsll_free(_ccsll)  cc_ll_free(_ccsll)



/* ccsll access */


#define ccsll_front(_ccsll)  ((_ccsll)->head.NXT->val)



/* ccsll capacity */


#define ccsll_size(_ccsll)   ((_ccsll)->size)

#define ccsll_empty(_ccsll)  ((_ccsll)->head.NXT == &((_ccsll)->tail))



/* ccsll modifiers */


#define ccsll_push_front(_ccsll, _val)                                         \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    ccsll_push_front_alloc((_ccsll));                                          \
                                                                               \
    (_ccsll)->head.NXT->val = (_val);                                          \
)


#define ccsll_push_front_alloc(_ccsll)                                         \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _node_alloc((_ccsll)->pnode, (_ccsll));                                    \
                                                                               \
    (_ccsll)->pnode->NXT = (_ccsll)->head.NXT;                                 \
    (_ccsll)->head.NXT   = (_ccsll)->pnode;                                    \
                                                                               \
    (_ccsll)->size++;                                                          \
)


#define  ccsll_pop_front(_ccsll)                                               \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccsll_empty((_ccsll)))  break;                                         \
                                                                               \
    (_ccsll)->pnode    = (_ccsll)->head.NXT;                                   \
    (_ccsll)->head.NXT = (_ccsll)->pnode->NXT;                                 \
                                                                               \
    _node_clear((_ccsll)->pnode, (_ccsll));                                    \
                                                                               \
    (_ccsll)->size--;                                                          \
)


#define ccsll_insert(_iter, _val)                                              \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccsll_iter_at_head((_iter)))  break;                                   \
                                                                               \
    _node_alloc((_iter)->cont->pnode, (_iter)->cont);                          \
                                                                               \
    /* TODO */                                                                 \
                                                                               \
    (_iter)->cont->size++;                                                     \
)


#define ccsll_erase(_iter)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccsll_iter_at_head((_iter)) || ccsll_iter_at_tail((_iter)))  break;    \
                                                                               \
    /* TODO */                                                                 \
                                                                               \
    _node_clear((_iter)->curr.node, (_iter)->cont);                            \
                                                                               \
    (_iter)->cont->size--;                                                     \
)


#define ccsll_swap(_ccsll_a, _ccsll_b)                                         \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    XOR2_SWAP((_ccsll_a), (_ccsll_b));                                         \
)


#define ccsll_resize(_ccsll, _items, _val)                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _size = ccsll_size((_ccsll)) - (_items);                               \
                                                                               \
         if (_size > 0)  while(_size--)  ccsll_pop_front ((_ccsll));           \
    else if (_size < 0)  while(_size++)  ccsll_push_front((_ccsll), (_val));   \
)


#define ccsll_clear(_ccsll)                                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while (!(ccsll_empty((_ccsll))))     ccsll_pop_front((_ccsll));            \
)



/* ccsll operations */


#define ccsll_move(_iter_p, _iter_i)                                           \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_i)->curr.node == (_iter_p)->curr.node))  break;       \
                                                                               \
    if (_unlikely(ccsll_iter_at_tail((_iter_p)) ||                             \
                  ccsll_iter_at_end ((_iter_i)) ||                             \
                  ccsll_iter_at_tail((_iter_i))))  break;                      \
                                                                               \
    _ccsll_move((_iter_p), (_iter_i));                                         \
                                                                               \
    (_iter_p)->cont->size++;                                                   \
    (_iter_i)->cont->size--;                                                   \
)

#define _ccsll_move(_iter_p, _iter_i)                                          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    void *_pbup = (_iter_i)->curr.node->NXT->NXT;                              \
                                                                               \
    (_iter_i)->curr.node->NXT->NXT = (_iter_p)->curr.node->NXT;                \
    (_iter_p)->curr.node->NXT      = (_iter_i)->curr.node->NXT;                \
    (_iter_i)->curr.node->NXT      =  _pbup;                                   \
)


#define ccsll_move_begin(_iter_a, _iter_b)                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    ccsll_iter_head((_iter_a));                                                \
    ccsll_iter_head((_iter_b));                                                \
                                                                               \
    ccsll_move((_iter_a), (_iter_b));                                          \
)


#define ccsll_move_range(_iter_p, _iter_l, _iter_r)                            \
                                                                               \
        ccsll_move_range_extd(_iter_p, _iter_l, _iter_r,    -1)

#define ccsll_move_range_extd(_iter_p, _iter_l, _iter_r, _dist)  /* (l, r] */  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely(ccsll_iter_at_tail((_iter_p)) ||                             \
                  ccsll_iter_at_tail((_iter_r))))  break;                      \
                                                                               \
    if (_unlikely((_iter_l)->curr.node == (_iter_r)->curr.node))  break;       \
                                                                               \
    if (_unlikely((_iter_l)->cont != (_iter_r)->cont))  break;                 \
                                                                               \
    if (_unlikely((_iter_p)->cont != (_iter_r)->cont))                         \
    {                                                                          \
        int _dist_m = (_dist);                                                 \
                                                                               \
        if (_dist_m <  0)                                                      \
            ccsll_iter_distance((_iter_l), (_iter_r), &_dist_m);               \
                                                                               \
        if (_dist_m <= 0)  break;                                              \
                                                                               \
        (_iter_p)->cont->size += _dist_m;                                      \
        (_iter_r)->cont->size -= _dist_m;                                      \
        (_iter_r)->cont = (_iter_p)->cont;                                     \
    }                                                                          \
                                                                               \
    void *_pbup = (_iter_p)->curr.node->NXT;                                   \
                                                                               \
    (_iter_p)->curr.node->NXT = (_iter_l)->curr.node->NXT;                     \
    (_iter_l)->curr.node->NXT = (_iter_r)->curr.node->NXT;                     \
    (_iter_r)->curr.node->NXT =  _pbup;                                        \
)


#define  ccsll_move_into(_ccsll_d, _ccsll_s)                                   \
                                                                               \
         cc_ll_move_into(_ccsll_d, _ccsll_s, ccsll, , _fast)

#define _ccsll_move_into(_iter_l, _iter_m, _iter_r)                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    ccsll_iter_end ((_iter_l));                                                \
    ccsll_iter_head((_iter_m));                                                \
                                                                               \
    ccsll_move_range_extd((_iter_l), (_iter_m),                                \
                          (_iter_r),  ccsll_size((_iter_m)->cont));            \
)


#define  ccsll_merge(_ccsll_d, _ccsll_s)                                       \
                                                                               \
         ccsll_merge_extd(_ccsll_d, _ccsll_s, SLEQ_NEXT, 0)

#define  ccsll_merge_extd(_ccsll_d, _ccsll_s, _leq, _last)                     \
                                                                               \
         cc_ll_merge_extd(_ccsll_d, _ccsll_s, _leq, ccsll, , _fast, _last)

#define _ccsll_merge_extd(_iter_l, _iter_m, _iter_r, _leq, _last)              \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_l)->cont == (_iter_m)->cont ||                        \
                  (_iter_l)->cont == (_iter_r)->cont ||                        \
                  (_iter_m)->cont != (_iter_r)->cont))  break;                 \
                                                                               \
    ccsll_iter_head((_iter_l));                                                \
    ccsll_iter_head((_iter_m));                                                \
    ccsll_iter_head((_iter_r));                                                \
                                                                               \
    for (register int _len = 0; ; _len = 0)                                    \
    {                                                                          \
        while (!(ccsll_iter_at_end((_iter_l))) && _leq((_iter_l), (_iter_m)))  \
            ((void)ccsll_iter_incr((_iter_l)));                                \
                                                                               \
        while (!(ccsll_iter_at_end((_iter_r))) &&                              \
                (ccsll_iter_at_end((_iter_l)) || !_leq((_iter_l), (_iter_r)))) \
            ((void)ccsll_iter_incr((_iter_r)), ++_len);                        \
                                                                               \
        ccsll_move_range_extd((_iter_l), (_iter_m), (_iter_r), _len);          \
                                                                               \
        if (ccsll_iter_at_end((_iter_m)))                                      \
        {                                                                      \
            ccsll_iter_copy((_iter_l), (_iter_r));                             \
                                                                               \
            while (!(ccsll_iter_at_end((_iter_l))))                            \
                (void)(ccdll_iter_incr((_iter_l)));                            \
            break;                                                             \
        }                                                                      \
                                                                               \
        ccsll_iter_copy((_iter_l), (_iter_r));                                 \
        ccsll_iter_init((_iter_r), (_iter_m)->cont, 0);                        \
        ccsll_iter_head((_iter_r));                                            \
    }                                                                          \
)


#define  ccsll_sort(_ccsll)                                                    \
                                                                               \
         ccsll_sort_extd(_ccsll, SLEQ_NEXT)

#define  ccsll_sort_extd(_ccsll, _leq)                                         \
                                                                               \
         cc_ll_sort_extd(_ccsll, _leq, ccsll, )

#define _ccsll_sort_extd(_ccsll, _carry, _pbuck, _iter_a, _iter_b, _leq)       \
                                                                               \
        _cc_ll_sort_extd(_ccsll, _carry, _pbuck, _iter_a, _iter_b, _leq, ccsll,)



/* ccsll comparator */


#define ccsll_comp_leq(_iter_a, _iter_b)       (SREF((_iter_a)) <=             \
                                                SREF((_iter_b)))

#define ccsll_comp_leq_next(_iter_a, _iter_b)  (SREF_NEXT((_iter_a)) <=        \
                                                SREF_NEXT((_iter_b)))

#define ccsll_comp_geq(_iter_a, _iter_b)       (SREF((_iter_a)) >=             \
                                                SREF((_iter_b)))

#define ccsll_comp_geq_next(_iter_a, _iter_b)  (SREF_NEXT((_iter_a)) >=        \
                                                SREF_NEXT((_iter_b)))



/* ccsll iterators */


#define ccsll_iter_copy(_iter_dst, _iter_src)                                  \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    *(_iter_dst) = *(_iter_src)                                                \
)


#define ccsll_iter_head(_iter)                                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = &((_iter)->cont->head)                                \
)


#define ccsll_iter_tail(_iter)                                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = &((_iter)->cont->tail)                                \
)


#define ccsll_iter_begin(_iter)                                                \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node =  ((_iter)->cont->head.NXT)                            \
)


#define ccsll_iter_end(_iter)                                                  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    ccsll_iter_head((_iter));                                                  \
                                                                               \
    while (!(ccsll_iter_at_end((_iter))))                                      \
        (void)(ccsll_iter_incr((_iter)));                                      \
)


#define ccsll_iter_at_head(_iter)   ( (_iter)->curr.node ==                    \
                                    &((_iter)->cont->head) )

#define ccsll_iter_at_tail(_iter)   ( (_iter)->curr.node ==                    \
                                    &((_iter)->cont->tail) )

#define ccsll_iter_at_begin(_iter)  ( (_iter)->curr.node ==                    \
                                      (_iter)->cont->head.NXT )

#define ccsll_iter_at_end(_iter)    ( (_iter)->curr.node->NXT ==               \
                                    &((_iter)->cont->tail))


#define ccsll_iter_incr(_iter)                                                 \
(                                                                              \
    ccsll_iter_at_tail((_iter)) ? (NULL) :                                     \
    ((_iter)->curr.node = (_iter)->curr.node->NXT)->NXT                        \
)


#define ccsll_iter_advance(_iter, _diff)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _len = (_diff);                                                        \
                                                                               \
    if (_len > 0)  while (ccsll_iter_incr((_iter)) && --_len);                 \
)


#define ccsll_iter_distance(_iter_a, _iter_b, _pdist)                          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_init((_iter_a)->cont, 1, _base_d1, ccsll);                             \
                                                                               \
    ccsll_iter_copy(_it((_iter_a)->cont, _base_d1, 0), (_iter_a));             \
                                                                               \
    STATEMENT_                                                                 \
    (                                                                          \
        (*(_pdist)) = 0;                                                       \
        if ((_iter_a)->cont != (_iter_b)->cont)  break;                        \
                                                                               \
        while ((_iter_a)->curr.node != (_iter_b)->curr.node && ++(*(_pdist)))  \
               if (!(ccsll_iter_incr((_iter_a))))  break;                      \
                                                                               \
        if ((_iter_a)->curr.node == (_iter_b)->curr.node)  break;              \
        else  (*(_pdist)) = 0;                                                 \
    );                                                                         \
                                                                               \
    ccsll_iter_copy((_iter_a), _it((_iter_a)->cont, _base_d1, 0));             \
                                                                               \
    _it_clear((_iter_a)->cont, 1, , );                                         \
)



/* ccsll traversor */


#define CCSLL_INCR(_iter)  CC_LL_INCR(_iter, ccsll)

#ifndef CC_STRICT

#define CCSLL_INCR_AUTO(_pval, _ccsll)                                         \
                                                                               \
        CCSLL_INCR_EXTD(_pval, _ccsll, (void)0)

#define CCSLL_INCR_EXTD(_pval, _ccsll, ...)                                    \
                                                                               \
        CC_LL_INCR_EXTD(_pval, _ccsll, ccsll, __VA_ARGS__)

#endif // CC_STRICT



#endif
