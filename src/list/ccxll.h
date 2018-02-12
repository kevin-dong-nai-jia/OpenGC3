#ifndef OPENGC3_LIST_CCXLL_H
#define OPENGC3_LIST_CCXLL_H

#include "base.h"
#include "../base/pool.h"
#include "../base/misc.h"
#include "../base/snym.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


/* ccxll create */


#define ccxll(elem_t)                                                          \
                                                                               \
        ccxll_extd(elem_t, 1, NORMAL)

#define ccxll_pckd(elem_t)                                                     \
                                                                               \
        ccxll_extd(elem_t, 1, PACKED)

#define ccxll_extd(elem_t, _n_iter, _ALIGN_)                                   \
                                                                               \
        typedef ccxll_struct_extd(elem_t, _n_iter, _ALIGN_) *CCXLL;  CCXLL

#define ccxll_type(elem_t)                                                     \
                                                                               \
        typedef ccxll_struct_extd(elem_t, 1, NORMAL) *


#define link_t void*

#define ccxll_struct(elem_t)                                                   \
                                                                               \
        ccxll_struct_extd(elem_t, 1, NORMAL)

#define ccxll_struct_pckd(elem_t)                                              \
                                                                               \
        ccxll_struct_extd(elem_t, 1, PACKED)

#define ccxll_struct_extd(elem_t, _n_iter, _ALIGN_)                            \
                                                                               \
    struct CCXLL_CONT                                                          \
    {                                                                          \
        int size,  last,  vcnt;                   /* size and node record */   \
        int start, ratio, thrsh;                  /* block increment info */   \
                                                                               \
        struct CCXLL_NODE                                                      \
        {   link_t lnk[1];                                                     \
            elem_t val;                           /* val with an xor link */   \
        }   *avsp, *pnode, swap;                  /* available space list */   \
                                                                               \
        union  CCXLL_HDTL                                                      \
        {   link_t lnk[1];                                                     \
            struct CCXLL_NODE *stnl;              /* points to same addr. */   \
        }   head, tail;                           /* two pseudo sentinels */   \
                                                                               \
        struct CCXLL_BLCK                                                      \
        {   struct CCXLL_BLCK *bprv, *bnxt;       /* next and prev blocks */   \
            PRAGMA_##_ALIGN_##_BGN                /* packed pragma starts */   \
            int ncnt;                             /* the item of the node */   \
            struct CCXLL_NODE nodes[];            /* node structure array */   \
            PRAGMA_##_ALIGN_##_END                /* the pragma ends here */   \
        }   *pool, *pblock;                       /* points to 1-st block */   \
                                                                               \
        struct CCXLL_ITER                                                      \
        {   union CCXLL_PTRS                                                   \
            {   link_t lnk[1];                                                 \
                struct CCXLL_NODE *node;                                       \
            }   prev, curr, next;                 /* points to p/v/n node */   \
            struct CCXLL_CONT *cont;              /* points to ccxll body */   \
        }   (*itarr)[_n_iter], *_iter, **_it;                                  \
                                                                               \
        struct CCXLL_CONT **_co;                  /* internal use _it _co */   \
                                                                               \
        int _it_base, _it_limit;                                               \
        int _co_base, _co_limit;                                               \
    }



/* ccxll initialize */


#define ccxll_init(_ccxll)                                                     \
                                                                               \
        ccxll_init_extd(_ccxll,     16,      2,  65536)

#define ccxll_init_extd(_ccxll, _start, _ratio, _thrsh)                        \
                                                                               \
        cc_ll_init_extd(_ccxll, _start, _ratio, _thrsh, ccxll)


#define _ccxll_init(_ccxll_dst, _ccxll_src, _alloc)                            \
                                                                               \
        _ccxll_init_extd(_ccxll_dst, -1,     -1,     -1, _alloc)

#define _ccxll_init_extd(_ccxll, _start, _ratio, _thrsh, _alloc)               \
                                                                               \
        _cc_ll_init_extd(_ccxll, _start, _ratio, _thrsh, _alloc, ccxll)


#define _ccxll_init_core(_ccxll, _alloc)                                       \
                                                                               \
        _cc_ll_init_core(_ccxll, _alloc, ccxll)


#define _ccxll_init_seed(_ccxll)                                               \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccxll)->size = 0,                                                        \
    (_ccxll)->last = (_ccxll)->vcnt = 0,                                       \
                                                                               \
    (_ccxll)->head.XOR = &((_ccxll)->tail),                                    \
    (_ccxll)->tail.XOR = &((_ccxll)->head)                                     \
)


#define _ccxll_init_info(_ccxll, _start, _ratio, _thrsh)                       \
                                                                               \
        _cc_ll_init_info(_ccxll, _start, _ratio, _thrsh)


#define ccxll_iter_init(_iter, _ccxll, _alloc)                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->prev.node = ((_alloc) ? NULL : (_iter)->prev.node),               \
    (_iter)->curr.node = ((_alloc) ? NULL : (_iter)->curr.node),               \
    (_iter)->next.node = ((_alloc) ? NULL : (_iter)->next.node),               \
    (_iter)->cont = (_ccxll)                                                   \
)


#define _ccxll_iter_init(_iter, _ccxll, _alloc)                                \
                                                                               \
        _cc_ll_iter_init(_iter, _ccxll, _alloc, ccxll)



/* ccxll destroy */


#define ccxll_free(_ccxll)  cc_ll_free(_ccxll)



/* ccxll access */


#define ccxll_front(_ccxll)  ((_ccxll)->head.stnl->val)

#define ccxll_back(_ccxll)   ((_ccxll)->tail.stnl->val)



/* ccxll capacity */


#define ccxll_size(_ccxll)   ((_ccxll)->size)

#define ccxll_empty(_ccxll)  ((_ccxll)->head.XOR == &((_ccxll)->tail) &&       \
                              (_ccxll)->tail.XOR == &((_ccxll)->head))



/* ccxll modifiers */


#define  ccxll_push_front(_ccxll, _val)  _ccxll_push(_ccxll, _val, head)

#define  ccxll_push_back(_ccxll, _val)   _ccxll_push(_ccxll, _val, tail)

#define _ccxll_push(_ccxll, _val, _hdtl_)                                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _ccxll_push_alloc((_ccxll), _hdtl_);                                       \
                                                                               \
    (_ccxll)->pnode->val = (_val);                                             \
)


#define ccxll_push_front_alloc(_ccxll) _ccxll_push_alloc(_ccxll, head)

#define ccxll_push_back_alloc(_ccxll)  _ccxll_push_alloc(_ccxll, tail)

#define _ccxll_push_alloc(_ccxll, _hdtl_)                                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _node_alloc((_ccxll)->pnode, (_ccxll));                                    \
                                                                               \
    (_ccxll)->pnode->XOR       = XOR2(&((_ccxll)->_hdtl_.XOR),                 \
                                        (_ccxll)->_hdtl_.XOR);                 \
                                                                               \
    (_ccxll)->_hdtl_.stnl->XOR = XOR3(&((_ccxll)->_hdtl_.XOR),                 \
                                        (_ccxll)->_hdtl_.stnl->XOR,            \
                                      &((_ccxll)->pnode->XOR));                \
                                                                               \
    (_ccxll)->_hdtl_.XOR = &((_ccxll)->pnode->XOR);                            \
                                                                               \
    (_ccxll)->size++;                                                          \
)


#define  ccxll_pop_front(_ccxll)  _ccxll_pop(_ccxll, head)

#define  ccxll_pop_back(_ccxll)   _ccxll_pop(_ccxll, tail)

#define _ccxll_pop(_ccxll, _hdtl_)                                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_empty((_ccxll)))  break;                                         \
                                                                               \
    (_ccxll)->pnode = (_ccxll)->_hdtl_.stnl;                                   \
                                                                               \
    (_ccxll)->_hdtl_.XOR       = XOR2(&((_ccxll)->_hdtl_.XOR),                 \
                                        (_ccxll)->_hdtl_.stnl->XOR);           \
                                                                               \
    (_ccxll)->_hdtl_.stnl->XOR = XOR3(&((_ccxll)->_hdtl_.XOR),                 \
                                        (_ccxll)->_hdtl_.stnl->XOR,            \
                                      &((_ccxll)->pnode->XOR));                \
                                                                               \
    _node_clear((_ccxll)->pnode, (_ccxll));                                    \
                                                                               \
    (_ccxll)->size--;                                                          \
)


#define ccxll_insert(_iter, _val)                                              \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_iter_at_head((_iter)))  break;                                   \
                                                                               \
    _node_alloc((_iter)->cont->pnode, (_iter)->cont);                          \
                                                                               \
    (_iter)->cont->pnode->val = (_val);                                        \
                                                                               \
    (_iter)->next.node = (_iter)->curr.node;                                   \
    (_iter)->curr.node = (_iter)->cont->pnode;                                 \
                                                                               \
    (_iter)->curr.node->XOR = XOR2((_iter)->prev.XOR, (_iter)->next.XOR);      \
                                                                               \
    (_iter)->prev.node->XOR = XOR3((_iter)->prev.node->XOR,                    \
                                   (_iter)->next.XOR,                          \
                                 &((_iter)->cont->pnode->XOR));                \
    (_iter)->next.node->XOR = XOR3((_iter)->next.node->XOR,                    \
                                   (_iter)->prev.XOR,                          \
                                 &((_iter)->cont->pnode->XOR));                \
    (_iter)->cont->size++;                                                     \
)


#define ccxll_erase(_iter)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_iter_at_head((_iter)) || ccxll_iter_at_tail((_iter)))  break;    \
                                                                               \
    (_iter)->prev.node->XOR = XOR3((_iter)->prev.node->XOR,                    \
                                   (_iter)->next.XOR, (_iter)->curr.XOR);      \
    (_iter)->next.node->XOR = XOR3((_iter)->next.node->XOR,                    \
                                   (_iter)->prev.XOR, (_iter)->curr.XOR);      \
                                                                               \
    _node_clear((_iter)->curr.node, (_iter)->cont);                            \
                                                                               \
    (_iter)->curr.XOR =      (_iter)->next.XOR;                                \
    (_iter)->next.XOR = XOR2((_iter)->curr.node->XOR, (_iter)->prev.XOR);      \
                                                                               \
    (_iter)->cont->size--;                                                     \
)


#define ccxll_swap(_ccxll_a, _ccxll_b)                                         \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    XOR2_SWAP((_ccxll_a), (_ccxll_b));                                         \
)


#define ccxll_resize(_ccxll, _items, _val)                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _size = ccxll_size((_ccxll)) - (_items);                               \
                                                                               \
         if (_size > 0)  while(_size--)  ccxll_pop_back ((_ccxll));            \
    else if (_size < 0)  while(_size++)  ccxll_push_back((_ccxll), (_val));    \
)


#define ccxll_clear(_ccxll)                                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while (!(ccxll_empty((_ccxll))))     ccxll_pop_back ((_ccxll));            \
)



/* ccxll operations */


#define ccxll_move(_iter_p, _iter_i)                                           \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_i)->curr.node == (_iter_p)->prev.node))  break;       \
                                                                               \
    if (_unlikely(ccxll_iter_at_head((_iter_p)) ||                             \
                  ccxll_iter_at_head((_iter_i)) ||                             \
                  ccxll_iter_at_tail((_iter_i))))  break;                      \
                                                                               \
    _ccxll_move((_iter_p), (_iter_i));                                         \
                                                                               \
    (_iter_i)->cont->size--;                                                   \
    (_iter_i)->cont = (_iter_p)->cont;                                         \
    (_iter_i)->cont->size++;                                                   \
)

#define _ccxll_move(_iter_p, _iter_i)                                          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_iter_i)->prev.node->XOR = XOR3((_iter_i)->prev.node->XOR,                \
                                     (_iter_i)->next.XOR, (_iter_i)->curr.XOR);\
    (_iter_i)->next.node->XOR = XOR3((_iter_i)->next.node->XOR,                \
                                     (_iter_i)->prev.XOR, (_iter_i)->curr.XOR);\
                                                                               \
    (_iter_i)->prev.XOR = (_iter_p)->prev.XOR;                                 \
    (_iter_i)->next.XOR = (_iter_p)->curr.XOR;                                 \
                                                                               \
    (_iter_i)->curr.node->XOR = XOR2((_iter_i)->prev.XOR, (_iter_i)->next.XOR);\
                                                                               \
    (_iter_i)->prev.node->XOR = XOR3((_iter_i)->prev.node->XOR,                \
                                     (_iter_i)->next.XOR, (_iter_i)->curr.XOR);\
    (_iter_i)->next.node->XOR = XOR3((_iter_i)->next.node->XOR,                \
                                     (_iter_i)->prev.XOR, (_iter_i)->curr.XOR);\
                                                                               \
    (_iter_p)->prev.XOR =      (_iter_i)->curr.XOR;                            \
    (_iter_p)->next.XOR = XOR2((_iter_p)->prev.XOR, (_iter_p)->curr.node->XOR);\
)


#define ccxll_move_begin(_iter_a, _iter_b)                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    ccxll_iter_begin((_iter_a));                                               \
    ccxll_iter_begin((_iter_b));                                               \
                                                                               \
    ccxll_move((_iter_a), (_iter_b));                                          \
)


#define ccxll_move_range(_iter_p, _iter_l, _iter_r)                            \
                                                                               \
        ccxll_move_range_extd(_iter_p, _iter_l, _iter_r,    -1)

#define ccxll_move_range_extd(_iter_p, _iter_l, _iter_r, _dist)  /* [l, r) */  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely(ccxll_iter_at_head ((_iter_p)) ||                            \
                  ccxll_iter_at_head ((_iter_l))))  break;                     \
                                                                               \
    if (_unlikely((_iter_l)->curr.node == (_iter_r)->curr.node))  break;       \
                                                                               \
    if (_unlikely((_iter_l)->cont != (_iter_r)->cont))  break;                 \
                                                                               \
    if (_unlikely((_iter_p)->cont != (_iter_l)->cont))                         \
    {                                                                          \
        int _dist_m = (_dist);                                                 \
                                                                               \
        if (_dist_m <  0)                                                      \
            ccxll_iter_distance((_iter_l), (_iter_r), &_dist_m);               \
                                                                               \
        if (_dist_m <= 0)  break;                                              \
                                                                               \
        (_iter_p)->cont->size += _dist_m;                                      \
        (_iter_l)->cont->size -= _dist_m;                                      \
        (_iter_l)->cont = (_iter_p)->cont;                                     \
    }                                                                          \
                                                                               \
    link_t _p_c = (_iter_p)->curr.XOR;                                         \
    link_t _l_c = (_iter_l)->curr.XOR;                                         \
    link_t _r_c = (_iter_r)->curr.XOR;                                         \
    link_t _p_p = (_iter_p)->prev.XOR;                                         \
    link_t _l_p = (_iter_l)->prev.XOR;                                         \
    link_t _r_p = (_iter_r)->prev.XOR;                                         \
                                                                               \
    (_iter_p)->prev.node->XOR = XOR3((_iter_p)->prev.node->XOR, _p_c, _l_c);   \
    (_iter_l)->prev.node->XOR = XOR3((_iter_l)->prev.node->XOR, _l_c, _r_c);   \
    (_iter_r)->prev.node->XOR = XOR3((_iter_r)->prev.node->XOR, _r_c, _p_c);   \
                                                                               \
    (_iter_p)->curr.node->XOR = XOR3((_iter_p)->curr.node->XOR, _p_p, _r_p);   \
    (_iter_l)->curr.node->XOR = XOR3((_iter_l)->curr.node->XOR, _l_p, _p_p);   \
    (_iter_r)->curr.node->XOR = XOR3((_iter_r)->curr.node->XOR, _r_p, _l_p);   \
                                                                               \
    if (_unlikely((_iter_p)->next.XOR == _l_c))  (_iter_p)->next.XOR = _r_c;   \
    if           ((_iter_l)->next.XOR == _r_c)   (_iter_l)->next.XOR = _p_c;   \
    if (_unlikely((_iter_r)->next.XOR == _p_c))  (_iter_r)->next.XOR = _l_c;   \
                                                                               \
    (_iter_p)->prev.XOR = XOR2((_iter_p)->curr.node->XOR, (_iter_p)->next.XOR);\
    (_iter_l)->prev.XOR = XOR2((_iter_l)->curr.node->XOR, (_iter_l)->next.XOR);\
    (_iter_r)->prev.XOR = XOR2((_iter_r)->curr.node->XOR, (_iter_r)->next.XOR);\
)


#define  ccxll_move_into(_ccxll_d, _ccxll_s)                                   \
                                                                               \
         cc_ll_move_into(_ccxll_d, _ccxll_s, ccxll, , )

#define _ccxll_move_into(_iter_l, _iter_m, _iter_r)                            \
                                                                               \
        _cc_ll_move_into(_iter_l, _iter_m, _iter_r, ccxll)


#define  ccxll_merge(_ccxll_d, _ccxll_s)                                       \
                                                                               \
         ccxll_merge_extd(_ccxll_d, _ccxll_s, XLEQ, 0)

#define  ccxll_merge_extd(_ccxll_d, _ccxll_s, _leq, _last)                     \
                                                                               \
         cc_ll_merge_extd(_ccxll_d, _ccxll_s, _leq, ccxll, , , _last)

#define _ccxll_merge_extd(_iter_l, _iter_m, _iter_r, _leq, _last)              \
                                                                               \
        _cc_ll_merge_extd(_iter_l, _iter_m, _iter_r, _leq, ccxll, )


#define  ccxll_merge_range(_iter_l, _iter_m, _iter_r)                          \
                                                                               \
         ccxll_merge_range_extd(_iter_l, _iter_m, _iter_r, XLEQ)

#define  ccxll_merge_range_extd(_iter_l, _iter_m, _iter_r, _leq)               \
                                                                               \
         cc_ll_merge_range_extd(_iter_l, _iter_m, _iter_r, _leq, ccxll, )

#define _ccxll_merge_range_extd(_iter_l, _iter_m, _iter_r, _iter_x, _leq)      \
                                                                               \
        _cc_ll_merge_range_extd(_iter_l, _iter_m, _iter_r, _iter_x, _leq, ccxll)


#define  ccxll_sort(_ccxll)                                                    \
                                                                               \
         ccxll_sort_extd(_ccxll, XLEQ)

#define  ccxll_sort_extd(_ccxll, _leq)                                         \
                                                                               \
         cc_ll_sort_extd(_ccxll, _leq, ccxll, )

#define _ccxll_sort_extd(_ccxll, _carry, _pbuck, _iter_a, _iter_b, _leq)       \
                                                                               \
        _cc_ll_sort_extd(_ccxll, _carry, _pbuck, _iter_a, _iter_b, _leq, ccxll,)


#define ccxll_reverse_range(_iter_l, _iter_r)                                  \
                                                                               \
        ccxll_reverse_range_extd(_iter_l, _iter_r,  0)                         \

#define ccxll_reverse_range_extd(_iter_l, _iter_r, _flag_swap_iters)           \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_l)->cont != (_iter_r)->cont))  break;                 \
                                                                               \
    if (_unlikely((_iter_l)->curr.XOR == (_iter_r)->curr.XOR))  break;         \
                                                                               \
    link_t _x_in = XOR2((_iter_l)->curr.XOR, (_iter_r)->curr.XOR);             \
    link_t _x_ex = XOR2((_iter_l)->prev.XOR, (_iter_r)->next.XOR);             \
                                                                               \
    (_iter_l)->prev.node->XOR = XOR2((_iter_l)->prev.node->XOR, _x_in);        \
    (_iter_r)->next.node->XOR = XOR2((_iter_r)->next.node->XOR, _x_in);        \
                                                                               \
    (_iter_l)->curr.node->XOR = XOR2((_iter_l)->curr.node->XOR, _x_ex);        \
    (_iter_r)->curr.node->XOR = XOR2((_iter_r)->curr.node->XOR, _x_ex);        \
                                                                               \
    switch ((_flag_swap_iters))                                                \
    {                                                                          \
        case 0:                                                                \
        XOR2_SWAP((_iter_l)->prev.XOR, (_iter_l)->next.XOR);                   \
        XOR2_SWAP((_iter_r)->prev.XOR, (_iter_r)->next.XOR);                   \
        (_iter_l)->next.XOR = XOR2((_iter_l)->next.XOR, _x_ex);                \
        (_iter_r)->prev.XOR = XOR2((_iter_r)->prev.XOR, _x_ex);                \
        break;                                                                 \
                                                                               \
        case 1:  default:                                                      \
        XOR2_SWAP((_iter_l)->curr.XOR, (_iter_r)->curr.XOR);                   \
        XOR2_SWAP((_iter_l)->next.XOR, (_iter_r)->prev.XOR);                   \
    }                                                                          \
)



/* ccxll comparator */


#define ccxll_comp_leq(_iter_a, _iter_b)       (XREF((_iter_a)) <=             \
                                                XREF((_iter_b)))

#define ccxll_comp_leq_prev(_iter_a, _iter_b)  (XREF_PREV((_iter_a)) <=        \
                                                XREF_PREV((_iter_b)))

#define ccxll_comp_leq_next(_iter_a, _iter_b)  (XREF_NEXT((_iter_a)) <=        \
                                                XREF_NEXT((_iter_b)))

#define ccxll_comp_geq(_iter_a, _iter_b)       (XREF((_iter_a)) >=             \
                                                XREF((_iter_b)))

#define ccxll_comp_geq_prev(_iter_a, _iter_b)  (XREF_PREV((_iter_a)) >=        \
                                                XREF_PREV((_iter_b)))

#define ccxll_comp_geq_next(_iter_a, _iter_b)  (XREF_NEXT((_iter_a)) >=        \
                                                XREF_NEXT((_iter_b)))



/* ccxll iterators */


#define ccxll_iter_copy(_iter_dst, _iter_src)                                  \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    *(_iter_dst) = *(_iter_src)                                                \
)


#define ccxll_iter_head(_iter)                                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->prev.XOR = NULL,                                                  \
    (_iter)->curr.XOR = &((_iter)->cont->head.XOR),                            \
    (_iter)->next.XOR = &((_iter)->cont->head.stnl->XOR)                       \
)


#define ccxll_iter_tail(_iter)                                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->next.XOR = NULL,                                                  \
    (_iter)->curr.XOR = &((_iter)->cont->tail.XOR),                            \
    (_iter)->prev.XOR = &((_iter)->cont->tail.stnl->XOR)                       \
)


#define ccxll_iter_begin(_iter)                                                \
(                                                                              \
    (_iter)->prev.XOR =      &((_iter)->cont->head.XOR),                       \
    (_iter)->curr.XOR =      &((_iter)->cont->head.stnl->XOR),                 \
    (_iter)->next.XOR = XOR2(&((_iter)->cont->head.XOR),                       \
                               (_iter)->cont->head.stnl->XOR)                  \
)


#define ccxll_iter_end(_iter)                                                  \
(                                                                              \
    (_iter)->next.XOR =      &((_iter)->cont->tail.XOR),                       \
    (_iter)->curr.XOR =      &((_iter)->cont->tail.stnl->XOR),                 \
    (_iter)->prev.XOR = XOR2(&((_iter)->cont->tail.XOR),                       \
                               (_iter)->cont->tail.stnl->XOR)                  \
)


#define ccxll_iter_at_head(_iter)   ( (_iter)->curr.XOR ==                     \
                                    &((_iter)->cont->head.XOR))

#define ccxll_iter_at_tail(_iter)   ( (_iter)->curr.XOR ==                     \
                                    &((_iter)->cont->tail.XOR))

#define ccxll_iter_at_begin(_iter)  ( (_iter)->curr.XOR ==                     \
                                      (_iter)->cont->head.XOR )

#define ccxll_iter_at_end(_iter)    ( (_iter)->curr.XOR ==                     \
                                      (_iter)->cont->tail.XOR )


#define ccxll_iter_incr(_iter)                                                 \
(                                                                              \
    ccxll_iter_at_tail((_iter)) ? (NULL) :                                     \
    (                                                                          \
        (_iter)->prev.XOR = (_iter)->curr.XOR,                                 \
        (_iter)->curr.XOR = (_iter)->next.XOR,                                 \
                                                                               \
        _prefetch((_iter)->next.XOR = XOR2( (_iter)->prev.XOR,                 \
                  (_iter)->curr.node->XOR)),(_iter)->next.XOR                  \
    )                                                                          \
)


#define ccxll_iter_decr(_iter)                                                 \
(                                                                              \
    ccxll_iter_at_head((_iter)) ? (NULL) :                                     \
    (                                                                          \
        (_iter)->next.XOR = (_iter)->curr.XOR,                                 \
        (_iter)->curr.XOR = (_iter)->prev.XOR,                                 \
                                                                               \
        _prefetch((_iter)->prev.XOR = XOR2( (_iter)->next.XOR,                 \
                  (_iter)->curr.node->XOR)),(_iter)->prev.XOR                  \
    )                                                                          \
)


#define ccxll_iter_advance(_iter, _diff)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _len = (_diff);                                                        \
                                                                               \
         if (_len > 0)  while (ccxll_iter_incr((_iter)) && --_len);            \
    else if (_len < 0)  while (ccxll_iter_decr((_iter)) && ++_len);            \
)


#define ccxll_iter_distance(_iter_a, _iter_b, _pdist)                          \
                                                                               \
        cc_ll_iter_distance(_iter_a, _iter_b, _pdist, ccxll)


#ifndef CC_STRICT

#define ccxll_iter_swap(_iter_a, _iter_b)                                      \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    XOR2_SWAP((_iter_a)->prev.node, (_iter_b)->prev.node),                     \
    XOR2_SWAP((_iter_a)->curr.node, (_iter_b)->curr.node),                     \
    XOR2_SWAP((_iter_a)->next.node, (_iter_b)->next.node)                      \
)

#else

#define ccxll_iter_swap(_iter_a, _iter_b)                                      \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    XOR2_SWAP((_iter_a), (_iter_b))                                            \
)

#endif // CC_STRICT



/* ccxll traversor */


#define CCXLL_INCR(_iter)  CC_LL_INCR(_iter, ccxll)

#ifndef CC_STRICT

#define CCXLL_INCR_AUTO(_pval, _ccxll)                                         \
                                                                               \
        CCXLL_INCR_EXTD(_pval, _ccxll, (void)0)

#define CCXLL_INCR_EXTD(_pval, _ccxll, ...)                                    \
                                                                               \
        CC_LL_INCR_EXTD(_pval, _ccxll, ccxll, __VA_ARGS__)

#endif // CC_STRICT


#define CCXLL_DECR(_iter)  CC_LL_DECR(_iter, ccxll)

#ifndef CC_STRICT

#define CCXLL_DECR_AUTO(_pval, _ccxll)                                         \
                                                                               \
        CCXLL_DECR_EXTD(_pval, _ccxll, (void)0)

#define CCXLL_DECR_EXTD(_pval, _ccxll, ...)                                    \
                                                                               \
        CC_LL_DECR_EXTD(_pval, _ccxll, ccxll, __VA_ARGS__)

#endif // CC_STRICT



#endif
