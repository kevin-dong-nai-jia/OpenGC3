#ifndef CC_XLL_H
#define CC_XLL_H

#include "pool.h"
#include "misc.h"
#include "snym.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


/* synonym */

#define XOR lnk[0]

#define CCXLL       ADDID(CCXLL)
#define CCXLL_CONT  ADDID(CCXLL_CONT)
#define CCXLL_NODE  ADDID(CCXLL_NODE)
#define CCXLL_BLCK  ADDID(CCXLL_BLCK)
#define CCXLL_ITER  ADDID(CCXLL_ITER)
#define CCXLL_HDTL  ADDID(CCXLL_HDTL)
#define CCXLL_ADJC  ADDID(CCXLL_ADJC)


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


typedef void* link_t;

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
        int size,  used,  vcnt;                   /* size and node record */   \
        int start, ratio, thrsh;                  /* block increment info */   \
                                                                               \
        struct CCXLL_NODE                                                      \
        {   link_t lnk[1];                                                     \
            elem_t val;                           /* val with an xor link */   \
        }   *avsp, *pnode;                        /* available space list */   \
                                                                               \
        union  CCXLL_HDTL                                                      \
        {   link_t lnk[1];                                                     \
            struct CCXLL_NODE *stnl;              /* points to same addr. */   \
        }   head, tail;                           /* two pseudo sentinels */   \
                                                                               \
        struct CCXLL_BLCK                                                      \
        {   struct CCXLL_BLCK *next;              /* points to next block */   \
            PRAGMA_##_ALIGN_##_BGN                /* packed pragma starts */   \
            struct CCXLL_NODE nodes[1];           /* node structure array */   \
            PRAGMA_##_ALIGN_##_END                /* the pragma ends here */   \
        }   *pool, *pblock;                       /* points to 1-st block */   \
                                                                               \
        struct CCXLL_ITER                                                      \
        {   union CCXLL_ADJC                                                   \
            {   link_t lnk[1];                                                 \
                struct CCXLL_NODE *node;                                       \
            }   prev, curr, next;                 /* adjacent ptr to node */   \
            struct CCXLL_CONT *ccxll;             /* points to ccxll body */   \
        }   (*itarr)[_n_iter], *_iter, **_it;                                  \
                                                                               \
        struct CCXLL_CONT **_co;                  /* internal use _it _co */   \
                                                                               \
        unsigned char _it_base, _it_limit;                                     \
        unsigned char _co_base, _co_limit;                                     \
    }



/* ccxll initialize */


#define ccxll_init(_ccxll)                                                     \
                                                                               \
        ccxll_init_extd(_ccxll, 1 << 4, 1 << 1, 1 << 16)

#define ccxll_init_extd(_ccxll, _start, _ratio, _thrsh)                        \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_ccxll) = NULL;                                                           \
                                                                               \
    _ccxll_init_extd((_ccxll), (_start), (_ratio), (_thrsh), 1);               \
                                                                               \
    _itarr_alloc((_ccxll), ccxll);                                             \
    _ccxll_iter_init((_ccxll)->_iter, (_ccxll), 1);                            \
)


#define _ccxll_init(_ccxll_dst, _ccxll_src, _alloc)                            \
                                                                               \
        _ccxll_init_extd(_ccxll_dst, -1,     -1,     -1, _alloc)

#define _ccxll_init_extd(_ccxll, _start, _ratio, _thrsh, _alloc)               \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_alloc))                                                              \
        _cont_alloc((_ccxll));                                                 \
                                                                               \
    _ccxll_init_core((_ccxll));                                                \
    _ccxll_init_info((_ccxll), (_start), (_ratio), (_thrsh));                  \
)


#define _ccxll_init_core(_ccxll)                                               \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    _ccxll_init_seed((_ccxll)),                                                \
                                                                               \
    (_ccxll)->used = (_ccxll)->vcnt   = 0,                                     \
    (_ccxll)->avsp = (_ccxll)->pnode  = NULL,                                  \
    (_ccxll)->pool = (_ccxll)->pblock = NULL,                                  \
                                                                               \
    (_ccxll)->itarr = NULL,                                                    \
    (_ccxll)->_iter = NULL,                                                    \
    (_ccxll)->_it   = NULL,                                                    \
    (_ccxll)->_co   = NULL,                                                    \
    (_ccxll)->_it_base = (_ccxll)->_it_limit = 0,                              \
    (_ccxll)->_co_base = (_ccxll)->_co_limit = 0                               \
)


#define _ccxll_init_seed(_ccxll)                                               \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccxll)->size = 0,                                                        \
    (_ccxll)->head.XOR = &((_ccxll)->tail),                                    \
    (_ccxll)->tail.XOR = &((_ccxll)->head)                                     \
)


#define _ccxll_init_info(_ccxll, _start, _ratio, _thrsh)                       \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccxll)->start = ((_start) > 0) ? (_start) : 1,                           \
    (_ccxll)->ratio = ((_ratio) > 0) ? (_ratio) : 1,                           \
    (_ccxll)->thrsh = ((_thrsh) > (_ccxll)->start) ? (_thrsh) : (_ccxll)->start\
)


#define ccxll_iter_init(_iter, _ccxll)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->prev.XOR = NULL,                                                  \
    (_iter)->curr.XOR = NULL,                                                  \
    (_iter)->next.XOR = NULL,                                                  \
    (_iter)->ccxll = (_ccxll)                                                  \
)


#define _ccxll_iter_init(_iter, _ccxll, _alloc)                                \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_alloc))                                                              \
        _iter_alloc((_iter));                                                  \
                                                                               \
    ccxll_iter_init((_iter), (_ccxll));                                        \
)



/* ccxll destroy */


#define ccxll_free(_ccxll)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_free((_ccxll));                                                        \
    _co_free((_ccxll));                                                        \
                                                                               \
    _iter_free ((_ccxll)->_iter);                                              \
    _itarr_free((_ccxll));                                                     \
    _block_free((_ccxll));                                                     \
    _cont_free ((_ccxll));                                                     \
)



/* exclusive or */


#define XOR2(_addr_a, _addr_b)                                                 \
(                                                                              \
    (void*)((uintptr_t)(void*)(_addr_a) ^                                      \
            (uintptr_t)(void*)(_addr_b))                                       \
)


#define XOR3(_addr_a, _addr_b, _addr_c)                                        \
(                                                                              \
    (void*)((uintptr_t)(void*)(_addr_a) ^                                      \
            (uintptr_t)(void*)(_addr_b) ^                                      \
            (uintptr_t)(void*)(_addr_c))                                       \
)



/* ccxll access */


#define ccxll_front(_ccxll)  ((_ccxll)->head.stnl->val)

#define ccxll_back(_ccxll)   ((_ccxll)->tail.stnl->val)



/* ccxll capacity */


#define ccxll_size(_ccxll)   ((_ccxll)->size)

#define ccxll_empty(_ccxll)  ((ccxll_size((_ccxll))) == 0)



/* ccxll modifiers */


#define  ccxll_push_front(_ccxll, _val)  _ccxll_push(_ccxll, _val, head)

#define  ccxll_push_back(_ccxll, _val)   _ccxll_push(_ccxll, _val, tail)

#define _ccxll_push(_ccxll, _val, _hdtl_)                                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _node_alloc((_ccxll)->pnode, (_ccxll));                                    \
                                                                               \
    (_ccxll)->pnode->val       = (_val);                                       \
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
    _node_alloc((_iter)->ccxll->pnode, (_iter)->ccxll);                        \
                                                                               \
    (_iter)->ccxll->pnode->val = (_val);                                       \
                                                                               \
    (_iter)->next.node = (_iter)->curr.node;                                   \
    (_iter)->curr.node = (_iter)->ccxll->pnode;                                \
                                                                               \
    (_iter)->curr.node->XOR = XOR2((_iter)->prev.XOR, (_iter)->next.XOR);      \
                                                                               \
    (_iter)->prev.node->XOR = XOR3((_iter)->prev.node->XOR,                    \
                                   (_iter)->next.XOR,                          \
                                 &((_iter)->ccxll->pnode->XOR));               \
    (_iter)->next.node->XOR = XOR3((_iter)->next.node->XOR,                    \
                                   (_iter)->prev.XOR,                          \
                                 &((_iter)->ccxll->pnode->XOR));               \
    (_iter)->ccxll->size++;                                                    \
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
    _node_clear((_iter)->curr.node, (_iter)->ccxll);                           \
                                                                               \
    (_iter)->curr.XOR =      (_iter)->next.XOR;                                \
    (_iter)->next.XOR = XOR2((_iter)->curr.node->XOR, (_iter)->prev.XOR);      \
                                                                               \
    (_iter)->ccxll->size--;                                                    \
)


#define ccxll_swap(_ccxll_a, _ccxll_b)                                         \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    void *_bup = (_ccxll_a);                                                   \
                                                                               \
    (_ccxll_a) = (_ccxll_b);                                                   \
    (_ccxll_b) = _bup;                                                         \
)


#define ccxll_resize(_ccxll, _items, _val)                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _size = ccxll_size((_ccxll)) - (_items);                               \
                                                                               \
    if (_size > 0)       while(_size--)  ccxll_pop_back ((_ccxll));            \
    else if (_size < 0)  while(_size++)  ccxll_push_back((_ccxll), (_val));    \
)


#define ccxll_clear(_ccxll)                                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while (!(ccxll_empty((_ccxll))))  {  ccxll_pop_back ((_ccxll));  }         \
)



/* ccxll operations */


#define ccxll_move(_iter_p, _iter_i)                                           \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_i)->curr.XOR == (_iter_p)->prev.XOR))  break;         \
                                                                               \
    if (_unlikely(ccxll_iter_at_head((_iter_p)) ||                             \
                  ccxll_iter_at_head((_iter_i)) ||                             \
                  ccxll_iter_at_tail((_iter_i))))  break;                      \
                                                                               \
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
                                                                               \
    (_iter_i)->ccxll->size--;                                                  \
    (_iter_i)->ccxll = (_iter_p)->ccxll;                                       \
    (_iter_i)->ccxll->size++;                                                  \
)


#define ccxll_move_range(_iter_p, _iter_l, _iter_r)                            \
                                                                               \
        ccxll_move_range_extd(_iter_p, _iter_l, _iter_r,    -1)

#define ccxll_move_range_extd(_iter_p, _iter_l, _iter_r, _dist)                \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_l)->ccxll != (_iter_r)->ccxll))  break;               \
                                                                               \
    if (_unlikely((_iter_p)->ccxll != (_iter_l)->ccxll))                       \
    {                                                                          \
        int _dist_m = (_dist);                                                 \
                                                                               \
        if (_dist_m < 0)                                                       \
            ccxll_iter_distance((_iter_l), (_iter_r), &_dist_m);               \
                                                                               \
        (_iter_p)->ccxll->size += _dist_m;                                     \
        (_iter_l)->ccxll->size -= _dist_m;                                     \
        (_iter_l)->ccxll = (_iter_p)->ccxll;                                   \
    }                                                                          \
                                                                               \
    if (_unlikely((_iter_l)->curr.XOR == (_iter_r)->curr.XOR))  break;         \
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


#define ccxll_merge(_ccxll_d, _ccxll_s)                                        \
                                                                               \
        ccxll_merge_extd(_ccxll_d, _ccxll_s, XLEQ)

#define ccxll_merge_extd(_ccxll_d, _ccxll_s, _leq)                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
     if (_unlikely(ccxll_empty((_ccxll_s))))  break;                           \
                                                                               \
    _it_alloc((_ccxll_d), 1, _base_p, ccxll);                                  \
    _it_alloc((_ccxll_s), 2, _base_q, ccxll);                                  \
                                                                               \
    _ccxll_merge_extd((_ccxll_d), _it_((_ccxll_d), _base_p, 0),                \
                                  _it_((_ccxll_s), _base_q, 0),                \
                                  _it_((_ccxll_s), _base_q, 1), _leq);         \
                                                                               \
    _it_clear((_ccxll_d), 1);                                                  \
    _it_clear((_ccxll_s), 2);                                                  \
)

#define _ccxll_merge_extd(_ccxll_d, _iter_l, _iter_m, _iter_r, _leq)           \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    ccxll_iter_tail ((_iter_l));                                               \
    ccxll_iter_begin((_iter_m));                                               \
    ccxll_iter_tail ((_iter_r));                                               \
                                                                               \
    ccxll_move_range_extd ((_iter_l), (_iter_m), (_iter_r),                    \
                           ccxll_size((_iter_m)->ccxll));                      \
                                                                               \
    ccxll_iter_begin((_iter_l));                                               \
    ccxll_iter_init ((_iter_r), (_ccxll_d));                                   \
    ccxll_iter_tail ((_iter_r));                                               \
                                                                               \
    ccxll_merge_range_extd((_iter_l), (_iter_m), (_iter_r), _leq);             \
)


#define ccxll_merge_range(_iter_l, _iter_m, _iter_r)                           \
                                                                               \
        ccxll_merge_range_extd(_iter_l, _iter_m, _iter_r, XLEQ)

#define ccxll_merge_range_extd(_iter_l, _iter_m, _iter_r, _leq)                \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_alloc((_iter_l)->ccxll, 1, _base_m, ccxll);                            \
                                                                               \
    _ccxll_merge_range_extd((_iter_l), (_iter_m), (_iter_r),                   \
                            _it_((_iter_l)->ccxll, _base_m, 0), _leq);         \
                                                                               \
    _it_clear((_iter_l)->ccxll, 1);                                            \
)

#define _ccxll_merge_range_extd(_iter_l, _iter_m, _iter_r, _iter_x, _leq)      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_l)->ccxll != (_iter_m)->ccxll ||                      \
                  (_iter_m)->ccxll != (_iter_r)->ccxll))  break;               \
                                                                               \
    ccxll_iter_copy((_iter_x), (_iter_m));                                     \
                                                                               \
    while (1)                                                                  \
    {                                                                          \
        while ((_iter_l)->curr.XOR != (_iter_m)->curr.XOR &&                   \
                _leq((_iter_l), (_iter_m)))                                    \
            ccxll_iter_incr((_iter_l));                                        \
                                                                               \
        if (_unlikely((_iter_l)->curr.XOR == (_iter_m)->curr.XOR))             \
        {                                                                      \
            ccxll_iter_copy((_iter_l), (_iter_r));                             \
            ccxll_iter_copy((_iter_m), (_iter_r));  break;                     \
        }                                                                      \
                                                                               \
        ccxll_iter_incr((_iter_x));                                            \
                                                                               \
        while ((_iter_x)->curr.XOR != (_iter_r)->curr.XOR &&                   \
               !_leq((_iter_l), (_iter_x)))                                    \
            ccxll_iter_incr((_iter_x));                                        \
                                                                               \
        ccxll_move_range((_iter_l), (_iter_m), (_iter_x));                     \
        ccxll_iter_copy ((_iter_m), (_iter_x));                                \
                                                                               \
        if (_unlikely((_iter_x)->curr.XOR == (_iter_r)->curr.XOR))             \
        {                                                                      \
            ccxll_iter_copy((_iter_l), (_iter_x));                             \
            ccxll_iter_copy((_iter_r), (_iter_x));  break;                     \
        }                                                                      \
    }                                                                          \
)


#define ccxll_sort(_ccxll)                                                     \
                                                                               \
        ccxll_sort_extd(_ccxll, XLEQ)

#define ccxll_sort_extd(_ccxll, _leq)                                          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _buck = log2(ccxll_size((_ccxll))) + 1;                                \
                                                                               \
    _co_alloc((_ccxll),     1, _base_c, ccxll);                                \
    _co_alloc((_ccxll), _buck, _base_b, ccxll);                                \
    _it_alloc((_ccxll),     2, _base_i, ccxll);                                \
                                                                               \
    _ccxll_sort_extd( (_ccxll),                                                \
                       _co_((_ccxll), _base_c, 0),                             \
                     &(_co_((_ccxll), _base_b, 0)),                            \
                       _it_((_ccxll), _base_i, 0),                             \
                       _it_((_ccxll), _base_i, 1), _leq);                      \
                                                                               \
    _co_clear((_ccxll), _buck + 1);                                            \
    _it_clear((_ccxll),  2);                                                   \
)

#define _ccxll_sort_extd(_ccxll, _carry, _p64bucket, _iter_a, _iter_b, _leq)   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely(ccxll_size((_ccxll)) <= 1))  break;                          \
                                                                               \
    int _fill = 0, _curr;                                                      \
                                                                               \
    do                                                                         \
    {                                                                          \
        ccxll_iter_init ((_iter_a), (_carry));                                 \
        ccxll_iter_init ((_iter_b), (_ccxll));                                 \
        ccxll_iter_begin((_iter_a));                                           \
        ccxll_iter_begin((_iter_b));                                           \
                                                                               \
        ccxll_move((_iter_a), (_iter_b));                                      \
                                                                               \
        for (_curr = 0; _curr != _fill &&                                      \
                       !(ccxll_empty(*((_p64bucket) + _curr))); _curr++)       \
        {                                                                      \
            ccxll_merge_extd(*((_p64bucket) + _curr), (_carry), _leq);         \
            ccxll_swap      (*((_p64bucket) + _curr), (_carry));               \
        }                                                                      \
        ccxll_swap(*((_p64bucket) + _curr), (_carry));                         \
                                                                               \
        if (_unlikely(_curr == _fill))  _fill++;                               \
    }                                                                          \
    while (!(ccxll_empty((_ccxll))));                                          \
                                                                               \
    for (_curr = 0; _curr < _fill; _curr++)                                    \
        ccxll_merge_extd((_ccxll), *((_p64bucket) + _curr), _leq);             \
)


#define ccxll_reverse_range(_iter_l, _iter_r)                                  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_iter_l)->ccxll != (_iter_r)->ccxll)  break;                          \
                                                                               \
    link_t _l_c = (_iter_l)->curr.XOR;                                         \
    link_t _r_c = (_iter_r)->curr.XOR;                                         \
    link_t _l_p = (_iter_l)->prev.XOR;                                         \
    link_t _r_p = (_iter_r)->prev.XOR;                                         \
    link_t _l_n = (_iter_l)->next.XOR;                                         \
                                                                               \
    (_iter_l)->next.node->XOR = XOR3((_iter_l)->next.node->XOR, _l_c, _r_c);   \
    (_iter_r)->prev.node->XOR = XOR3((_iter_r)->prev.node->XOR, _r_c, _l_c);   \
                                                                               \
    (_iter_l)->curr.node->XOR = XOR3((_iter_l)->curr.node->XOR, _l_n, _r_p);   \
    (_iter_r)->curr.node->XOR = XOR3((_iter_r)->curr.node->XOR, _l_n, _r_p);   \
                                                                               \
    (_iter_l)->next.XOR = XOR2(_l_p, _l_c);                                    \
    (_iter_r)->next.XOR = XOR2(_r_p, _r_c);                                    \
)



/* ccxll comparator */


#define CCXLL_LEQ_COMPAR(_iter_a, _iter_b)  (XREF((_iter_a)) <= XREF((_iter_b)))



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
    (_iter)->curr.XOR = &((_iter)->ccxll->head.XOR),                           \
    (_iter)->next.XOR = &((_iter)->ccxll->head.stnl->XOR)                      \
)


#define ccxll_iter_tail(_iter)                                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->next.XOR = NULL,                                                  \
    (_iter)->curr.XOR = &((_iter)->ccxll->tail.XOR),                           \
    (_iter)->prev.XOR = &((_iter)->ccxll->tail.stnl->XOR)                      \
)


#define ccxll_iter_begin(_iter)                                                \
(                                                                              \
    (_iter)->prev.XOR =      &((_iter)->ccxll->head.XOR),                      \
    (_iter)->curr.XOR =      &((_iter)->ccxll->head.stnl->XOR),                \
    (_iter)->next.XOR = XOR2(&((_iter)->ccxll->head.XOR),                      \
                               (_iter)->ccxll->head.stnl->XOR)                 \
)


#define ccxll_iter_end(_iter)                                                  \
(                                                                              \
    (_iter)->next.XOR =      &((_iter)->ccxll->tail.XOR),                      \
    (_iter)->curr.XOR =      &((_iter)->ccxll->tail.stnl->XOR),                \
    (_iter)->prev.XOR = XOR2(&((_iter)->ccxll->tail.XOR),                      \
                               (_iter)->ccxll->tail.stnl->XOR)                 \
)


#define ccxll_iter_at_head(_iter)   ((_iter)->prev.XOR == NULL)

#define ccxll_iter_at_tail(_iter)   ((_iter)->next.XOR == NULL)

#define ccxll_iter_at_begin(_iter)  ((_iter)->curr.XOR ==                      \
                                     (_iter)->ccxll->head.XOR)

#define ccxll_iter_at_end(_iter)    ((_iter)->curr.XOR ==                      \
                                     (_iter)->ccxll->tail.XOR)


#define ccxll_iter_incr(_iter)                                                 \
(                                                                              \
    (ccxll_iter_at_tail(_iter)) ? (NULL) :                                     \
    (                                                                          \
        (_iter)->prev.XOR =      (_iter)->curr.XOR,                            \
        (_iter)->curr.XOR =      (_iter)->next.XOR,                            \
        (_iter)->next.XOR = XOR2((_iter)->prev.XOR, (_iter)->curr.node->XOR)   \
    )                                                                          \
)


#define ccxll_iter_decr(_iter)                                                 \
(                                                                              \
    (ccxll_iter_at_head(_iter)) ? (NULL) :                                     \
    (                                                                          \
        (_iter)->next.XOR =      (_iter)->curr.XOR,                            \
        (_iter)->curr.XOR =      (_iter)->prev.XOR,                            \
        (_iter)->prev.XOR = XOR2((_iter)->next.XOR, (_iter)->curr.node->XOR)   \
    )                                                                          \
)


#define ccxll_iter_advance(_iter, _diff)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _len = (_diff);                                                        \
                                                                               \
    if (_len > 0)       {  while (ccxll_iter_incr((_iter)) && --_len);  }      \
    else if (_len < 0)  {  while (ccxll_iter_decr((_iter)) && ++_len);  }      \
)


#define ccxll_iter_distance(_iter_a, _iter_b, _pdist)                          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_alloc((_iter_a)->ccxll, 1, _base_d, ccxll);                            \
                                                                               \
    ccxll_iter_copy(_it_((_iter_a)->ccxll, _base_d, 0), (_iter_a));            \
                                                                               \
    STATEMENT_                                                                 \
    (                                                                          \
        (*(_pdist)) = 0;                                                       \
        if ((_iter_a)->ccxll != (_iter_b)->ccxll)  break;                      \
                                                                               \
        while ((_iter_a)->curr.XOR != (_iter_b)->curr.XOR && ++(*(_pdist)))    \
               if (!(ccxll_iter_incr((_iter_a))))  break;                      \
                                                                               \
        if ((_iter_a)->curr.XOR == (_iter_b)->curr.XOR)  break;                \
        else  (*(_pdist)) = 0;                                                 \
                                                                               \
        ccxll_iter_copy((_iter_a), _it_((_iter_a)->ccxll, _base_d, 0));        \
                                                                               \
        while ((_iter_a)->curr.XOR != (_iter_b)->curr.XOR && --(*(_pdist)))    \
               if (!(ccxll_iter_decr((_iter_a))))  break;                      \
                                                                               \
        if ((_iter_a)->curr.XOR == (_iter_b)->curr.XOR)  break;                \
        else  (*(_pdist)) = 0;                                                 \
    );                                                                         \
                                                                               \
    ccxll_iter_copy((_iter_a), _it_((_iter_a)->ccxll, _base_d, 0));            \
                                                                               \
    _it_clear((_iter_a)->ccxll, 1);                                            \
)



/* ccxll traversor */


#define CCXLL_INCR(_iter)                                                      \
                                                                               \
    for (ccxll_iter_head((_iter)); ccxll_iter_incr((_iter)); )

#ifndef CC_STRICT

#define CCXLL_INCR_AUTO(_pval, _ccxll)                                         \
                                                                               \
    ccxll_iter_head((_ccxll)->_iter);                                          \
                                                                               \
    for (__typeof__((_ccxll)->pnode->val) *_pval;                              \
         (ccxll_iter_incr((_ccxll)->_iter)) &&                                 \
         ((_pval) = &XREF((_ccxll)->_iter), 1); )

#endif // CC_STRICT


#define CCXLL_DECR(_iter)                                                      \
                                                                               \
    for (ccxll_iter_tail((_iter)); ccxll_iter_decr((_iter)); )

#ifndef CC_STRICT

#define CCXLL_DECR_AUTO(_pval, _ccxll)                                         \
                                                                               \
                                                                               \
    ccxll_iter_tail((_ccxll)->_iter);                                          \
                                                                               \
    for (__typeof__((_ccxll)->pnode->val) *_pval;                              \
         (ccxll_iter_decr((_ccxll)->_iter)) &&                                 \
         ((_pval) = &XREF((_ccxll)->_iter), 1); )

#endif // CC_STRICT



/* ccxll extensions */


#define ccxll_append(_ccxll_dst, _ccxll_src)                                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_alloc((_ccxll_src), 1, _base_a, ccxll);                                \
                                                                               \
    CCXLL_INCR(_it_((_ccxll_src), _base_a, 0))                                 \
        ccxll_push_back((_ccxll_dst), XREF(_it_((_ccxll_src), _base_a, 0)));   \
                                                                               \
    _it_clear((_ccxll_src), 1);                                                \
)



#endif
