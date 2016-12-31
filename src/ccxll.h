#ifndef CC_XLL_H
#define CC_XLL_H

#include "pool.h"
#include "misc.h"
#include "snym.h"

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
        typedef ccxll_struct_extd(elem_t, _n_iter, _ALIGN_) *CC_T_CCXLL;       \
                                                                               \
        CC_T_CCXLL


#define ccxll_struct(elem_t)                                                   \
                                                                               \
        ccxll_struct_extd(elem_t, 1, NORMAL)

#define ccxll_struct_pckd(elem_t)                                              \
                                                                               \
        ccxll_struct_extd(elem_t, 1, PACKED)

#define ccxll_struct_extd(elem_t, _n_iter, _ALIGN_)                            \
                                                                               \
    struct CC_S_CCXLL_BODY                                                     \
    {                                                                          \
        int size,  used,  vcnt;                   /* size and node record */   \
        int start, ratio, thrsh;                  /* block increment info */   \
                                                                               \
        struct CC_S_CCXLL_NODE                                                 \
        {   link_t lnk;                                                        \
            elem_t val;                           /* val with an xor link */   \
        }   *avsp, *pnode;                        /* available space list */   \
                                                                               \
        union  CC_U_CCXLL_HDTL                                                 \
        {   link_t lnk;                                                        \
            struct CC_S_CCXLL_NODE *stnl;         /* points to same addr. */   \
        }   head, tail;                           /* two pseudo sentinels */   \
                                                                               \
        struct CC_S_CCXLL_BLOCK                                                \
        {   struct CC_S_CCXLL_BLOCK *next;        /* points to next block */   \
            PRAGMA_##_ALIGN_##_BGN                /* packed pragma starts */   \
            struct CC_S_CCXLL_NODE nodes[1];      /* node structure array */   \
            PRAGMA_##_ALIGN_##_END                /* the pragma ends here */   \
        }   *pool, *pblock;                       /* points to 1-st block */   \
                                                                               \
        struct CC_S_CCXLL_ITER                                                 \
        {   union CC_U_CCXLL_ITER                                              \
            {   link_t lnk;                                                    \
                struct CC_S_CCXLL_NODE *node;                                  \
            }   prev, curr, next;                 /* adjacent ptr to node */   \
            struct CC_S_CCXLL_BODY *ccxll;        /* points to ccxll body */   \
        }   **_it, (*itarr)[_n_iter];                                          \
                                                                               \
        struct CC_S_CCXLL_BODY **_xl;             /* internal use _it/_xl */   \
                                                                               \
        unsigned char _it_base, _it_limit;                                     \
        unsigned char _xl_base, _xl_limit;                                     \
    }



/* ccxll initialize */


#define ccxll_init(_ccxll)                                                     \
                                                                               \
        ccxll_init_extd(_ccxll, 1 << 4, 1 << 1, 1 << 16)

#define ccxll_init_from(_ccxll_dst, _ccxll_src)                                \
                                                                               \
        ccxll_init_extd((_ccxll_dst),        (_ccxll_src)->start,              \
                        (_ccxll_src)->ratio, (_ccxll_src)->thrsh)

#define ccxll_init_extd(_ccxll, _start, _ratio, _thrsh)                        \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_ccxll) = NULL;                                                           \
                                                                               \
    _ccxll_init_extd((_ccxll), (_start), (_ratio), (_thrsh));                  \
    _itarr_alloc((_ccxll));                                                    \
)


#define _ccxll_init_from(_ccxll_dst, _ccxll_src)                               \
                                                                               \
        _ccxll_init_extd((_ccxll_dst),        (_ccxll_src)->start,             \
                         (_ccxll_src)->ratio, (_ccxll_src)->thrsh)

#define _ccxll_init_extd(_ccxll, _start, _ratio, _thrsh)                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _ccxll_alloc((_ccxll));                                                    \
                                                                               \
    _ccxll_init_core((_ccxll));                                                \
    _ccxll_init_info((_ccxll), (_start), (_ratio), (_thrsh));                  \
)


#define _ccxll_init_seed(_ccxll)                                               \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccxll)->size = 0,                                                        \
    (_ccxll)->head.lnk = &((_ccxll)->tail),                                    \
    (_ccxll)->tail.lnk = &((_ccxll)->head)                                     \
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
    (_ccxll)->_it   = NULL,                                                    \
    (_ccxll)->_xl   = NULL,                                                    \
    (_ccxll)->_it_base = (_ccxll)->_it_limit = 0,                              \
    (_ccxll)->_xl_base = (_ccxll)->_xl_limit = 0                               \
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
    (_iter)->prev.lnk = NULL,                                                  \
    (_iter)->curr.lnk = NULL,                                                  \
    (_iter)->next.lnk = NULL,                                                  \
    (_iter)->ccxll = (_ccxll)                                                  \
)


#define _ccxll_iter_init(_iter, _ccxll)                                        \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _iter_alloc((_iter));                                                      \
                                                                               \
    ccxll_iter_init((_iter), (_ccxll));                                        \
)



/* ccxll destroy */


#define ccxll_free(_ccxll)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_free((_ccxll));                                                        \
    _xl_free((_ccxll));                                                        \
                                                                               \
    _block_free((_ccxll));                                                     \
    _itarr_free((_ccxll));                                                     \
    _ccxll_free((_ccxll));                                                     \
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
    (_ccxll)->pnode->val = (_val);                                             \
                                                                               \
    (_ccxll)->pnode->lnk       = XOR2(&((_ccxll)->_hdtl_.lnk),                 \
                                        (_ccxll)->_hdtl_.lnk);                 \
                                                                               \
    (_ccxll)->_hdtl_.stnl->lnk = XOR3(&((_ccxll)->_hdtl_.lnk),                 \
                                        (_ccxll)->_hdtl_.stnl->lnk,            \
                                      &((_ccxll)->pnode->lnk));                \
                                                                               \
    (_ccxll)->_hdtl_.lnk = &((_ccxll)->pnode->lnk);                            \
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
    (_ccxll)->_hdtl_.lnk       = XOR2(&((_ccxll)->_hdtl_.lnk),                 \
                                        (_ccxll)->_hdtl_.stnl->lnk);           \
                                                                               \
    (_ccxll)->_hdtl_.stnl->lnk = XOR3(&((_ccxll)->_hdtl_.lnk),                 \
                                        (_ccxll)->_hdtl_.stnl->lnk,            \
                                      &((_ccxll)->pnode->lnk));                \
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
    (_iter)->curr.node->lnk = XOR2((_iter)->prev.lnk, (_iter)->next.lnk);      \
                                                                               \
    (_iter)->prev.node->lnk = XOR3((_iter)->prev.node->lnk,                    \
                                   (_iter)->next.lnk,                          \
                                 &((_iter)->ccxll->pnode->lnk));               \
    (_iter)->next.node->lnk = XOR3((_iter)->next.node->lnk,                    \
                                   (_iter)->prev.lnk,                          \
                                 &((_iter)->ccxll->pnode->lnk));               \
    (_iter)->ccxll->size++;                                                    \
)


#define ccxll_erase(_iter)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_iter_at_head((_iter)) || ccxll_iter_at_tail((_iter)))  break;    \
                                                                               \
    (_iter)->prev.node->lnk = XOR3((_iter)->prev.node->lnk,                    \
                                   (_iter)->next.lnk, (_iter)->curr.lnk);      \
    (_iter)->next.node->lnk = XOR3((_iter)->next.node->lnk,                    \
                                   (_iter)->prev.lnk, (_iter)->curr.lnk);      \
                                                                               \
    _node_clear((_iter)->curr.node, (_iter)->ccxll);                           \
                                                                               \
    (_iter)->curr.lnk =      (_iter)->next.lnk;                                \
    (_iter)->next.lnk = XOR2((_iter)->curr.node->lnk, (_iter)->prev.lnk);      \
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


#define _ccxll_swap(_ccxll_u, _ccxll_l)                                        \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    void    *_bup = (_ccxll_l);                                                \
    void *_xl_bup = (_ccxll_l)->_xl;                                           \
    unsigned char _xl_base_bup  = (_ccxll_l)->_xl_base;                        \
    unsigned char _xl_limit_bup = (_ccxll_l)->_xl_limit;                       \
                                                                               \
    (_ccxll_l)->_xl       = (_ccxll_u)->_xl;                                   \
    (_ccxll_l)->_xl_base  = (_ccxll_u)->_xl_base;                              \
    (_ccxll_l)->_xl_limit = (_ccxll_u)->_xl_limit;                             \
    (_ccxll_l)            = (_ccxll_u);                                        \
                                                                               \
    (_ccxll_u)->_xl       = _xl_bup;                                           \
    (_ccxll_u)->_xl_base  = _xl_base_bup;                                      \
    (_ccxll_u)->_xl_limit = _xl_limit_bup;                                     \
    (_ccxll_u)            = _bup;                                              \
)


#define ccxll_resize(_ccxll, _items, _val)                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _R = ccxll_size((_ccxll)) - (_items);                                  \
                                                                               \
    if (_R > 0)       {  while(_R--)  ccxll_pop_back ((_ccxll));          }    \
    else if (_R < 0)  {  while(_R++)  ccxll_push_back((_ccxll), (_val));  }    \
)


#define ccxll_clear(_ccxll)                                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while (!(ccxll_empty((_ccxll))))  ccxll_pop_back((_ccxll));                \
)



/* ccxll operations */


#define ccxll_move(_iter_p, _iter_i)                                           \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_iter_i)->curr.lnk == (_iter_p)->prev.lnk)  break;                    \
                                                                               \
    if (ccxll_iter_at_head((_iter_p)) ||                                       \
        ccxll_iter_at_head((_iter_i)) || ccxll_iter_at_tail((_iter_i)))  break;\
                                                                               \
    (_iter_i)->prev.node->lnk = XOR3((_iter_i)->prev.node->lnk,                \
                                     (_iter_i)->next.lnk, (_iter_i)->curr.lnk);\
    (_iter_i)->next.node->lnk = XOR3((_iter_i)->next.node->lnk,                \
                                     (_iter_i)->prev.lnk, (_iter_i)->curr.lnk);\
                                                                               \
    (_iter_i)->prev.lnk = (_iter_p)->prev.lnk;                                 \
    (_iter_i)->next.lnk = (_iter_p)->curr.lnk;                                 \
                                                                               \
    (_iter_i)->curr.node->lnk = XOR2((_iter_i)->prev.lnk, (_iter_i)->next.lnk);\
                                                                               \
    (_iter_i)->prev.node->lnk = XOR3((_iter_i)->prev.node->lnk,                \
                                     (_iter_i)->next.lnk, (_iter_i)->curr.lnk);\
    (_iter_i)->next.node->lnk = XOR3((_iter_i)->next.node->lnk,                \
                                     (_iter_i)->prev.lnk, (_iter_i)->curr.lnk);\
                                                                               \
    (_iter_p)->prev.lnk =      (_iter_i)->curr.lnk;                            \
    (_iter_p)->next.lnk = XOR2((_iter_p)->prev.lnk, (_iter_p)->curr.node->lnk);\
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
    if ((_iter_l)->ccxll != (_iter_r)->ccxll)  break;                          \
                                                                               \
    if ((_iter_p)->ccxll != (_iter_l)->ccxll)                                  \
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
    if ((_iter_l)->curr.lnk == (_iter_r)->curr.lnk)  break;                    \
                                                                               \
    link_t _p_c = (_iter_p)->curr.lnk;                                         \
    link_t _l_c = (_iter_l)->curr.lnk;                                         \
    link_t _r_c = (_iter_r)->curr.lnk;                                         \
    link_t _p_p = (_iter_p)->prev.lnk;                                         \
    link_t _l_p = (_iter_l)->prev.lnk;                                         \
    link_t _r_p = (_iter_r)->prev.lnk;                                         \
                                                                               \
    (_iter_p)->prev.node->lnk = XOR3((_iter_p)->prev.node->lnk, _p_c, _l_c);   \
    (_iter_l)->prev.node->lnk = XOR3((_iter_l)->prev.node->lnk, _l_c, _r_c);   \
    (_iter_r)->prev.node->lnk = XOR3((_iter_r)->prev.node->lnk, _r_c, _p_c);   \
                                                                               \
    (_iter_p)->curr.node->lnk = XOR3((_iter_p)->curr.node->lnk, _p_p, _r_p);   \
    (_iter_l)->curr.node->lnk = XOR3((_iter_l)->curr.node->lnk, _l_p, _p_p);   \
    (_iter_r)->curr.node->lnk = XOR3((_iter_r)->curr.node->lnk, _r_p, _l_p);   \
                                                                               \
    if ((_iter_p)->next.lnk == _l_c)  (_iter_p)->next.lnk = _r_c;              \
    if ((_iter_l)->next.lnk == _r_c)  (_iter_l)->next.lnk = _p_c;              \
    if ((_iter_r)->next.lnk == _p_c)  (_iter_r)->next.lnk = _l_c;              \
                                                                               \
    (_iter_p)->prev.lnk = XOR2((_iter_p)->curr.node->lnk, (_iter_p)->next.lnk);\
    (_iter_l)->prev.lnk = XOR2((_iter_l)->curr.node->lnk, (_iter_l)->next.lnk);\
    (_iter_r)->prev.lnk = XOR2((_iter_r)->curr.node->lnk, (_iter_r)->next.lnk);\
)


#define ccxll_merge(_ccxll_d, _ccxll_s)                                        \
                                                                               \
        ccxll_merge_extd(_ccxll_d, _ccxll_s, XLEQ)

#define ccxll_merge_extd(_ccxll_d, _ccxll_s, _leq)                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
     if (ccxll_empty((_ccxll_s)))  break;                                      \
                                                                               \
    _it_alloc((_ccxll_d), 1, _base_p);                                         \
    _it_alloc((_ccxll_s), 2, _base_q);                                         \
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
    _it_alloc((_iter_l)->ccxll, 1, _base_m);                                   \
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
    if ((_iter_l)->ccxll != (_iter_m)->ccxll ||                                \
        (_iter_m)->ccxll != (_iter_r)->ccxll)  break;                          \
                                                                               \
    ccxll_iter_copy((_iter_x), (_iter_m));                                     \
                                                                               \
    while (1)                                                                  \
    {                                                                          \
        while ((_iter_l)->curr.lnk != (_iter_m)->curr.lnk &&                   \
                _leq((_iter_l), (_iter_m)))                                    \
            ccxll_iter_incr((_iter_l));                                        \
                                                                               \
        if ((_iter_l)->curr.lnk == (_iter_m)->curr.lnk)                        \
        {                                                                      \
            ccxll_iter_copy((_iter_l), (_iter_r));                             \
            ccxll_iter_copy((_iter_m), (_iter_r));  break;                     \
        }                                                                      \
                                                                               \
        ccxll_iter_incr((_iter_x));                                            \
                                                                               \
        while ((_iter_x)->curr.lnk != (_iter_r)->curr.lnk &&                   \
               !_leq((_iter_l), (_iter_x)))                                    \
            ccxll_iter_incr((_iter_x));                                        \
                                                                               \
        ccxll_move_range((_iter_l), (_iter_m), (_iter_x));                     \
        ccxll_iter_copy ((_iter_m), (_iter_x));                                \
                                                                               \
        if ((_iter_x)->curr.lnk == (_iter_r)->curr.lnk)                        \
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
    _xl_alloc((_ccxll),  1, _base_c);                                          \
    _xl_alloc((_ccxll), 64, _base_b);                                          \
    _it_alloc((_ccxll),  2, _base_i);                                          \
                                                                               \
    _ccxll_sort_extd( (_ccxll),                                                \
                       _xl_((_ccxll), _base_c, 0),                             \
                     &(_xl_((_ccxll), _base_b, 0)),                            \
                       _it_((_ccxll), _base_i, 0),                             \
                       _it_((_ccxll), _base_i, 1), _leq);                      \
                                                                               \
    _xl_clear((_ccxll), 65);                                                   \
    _it_clear((_ccxll),  2);                                                   \
)

#define _ccxll_sort_extd(_ccxll, _carry, _p64bucket, _iter_a, _iter_b, _leq)   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_size((_ccxll)) <= 1)  break;                                     \
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
        if (_curr == _fill)  _fill++;                                          \
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
    link_t _l_c = (_iter_l)->curr.lnk;                                         \
    link_t _r_c = (_iter_r)->curr.lnk;                                         \
    link_t _l_p = (_iter_l)->prev.lnk;                                         \
    link_t _r_p = (_iter_r)->prev.lnk;                                         \
    link_t _l_n = (_iter_l)->next.lnk;                                         \
                                                                               \
    (_iter_l)->next.node->lnk = XOR3((_iter_l)->next.node->lnk, _l_c, _r_c);   \
    (_iter_r)->prev.node->lnk = XOR3((_iter_r)->prev.node->lnk, _r_c, _l_c);   \
                                                                               \
    (_iter_l)->curr.node->lnk = XOR3((_iter_l)->curr.node->lnk, _l_n, _r_p);   \
    (_iter_r)->curr.node->lnk = XOR3((_iter_r)->curr.node->lnk, _l_n, _r_p);   \
                                                                               \
    (_iter_l)->next.lnk = XOR2(_l_p, _l_c);                                    \
    (_iter_r)->next.lnk = XOR2(_r_p, _r_c);                                    \
)



/* default comparators */


#define CCXLL_LEQ_COMPAR(_iter_a, _iter_b)  (DREF((_iter_a)) <= DREF((_iter_b)))


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
    (_iter)->prev.lnk = NULL,                                                  \
    (_iter)->curr.lnk = &((_iter)->ccxll->head.lnk),                           \
    (_iter)->next.lnk = &((_iter)->ccxll->head.stnl->lnk)                      \
)


#define ccxll_iter_tail(_iter)                                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->next.lnk = NULL,                                                  \
    (_iter)->curr.lnk = &((_iter)->ccxll->tail.lnk),                           \
    (_iter)->prev.lnk = &((_iter)->ccxll->tail.stnl->lnk)                      \
)


#define ccxll_iter_begin(_iter)                                                \
(                                                                              \
    (_iter)->prev.lnk =      &((_iter)->ccxll->head.lnk),                      \
    (_iter)->curr.lnk =      &((_iter)->ccxll->head.stnl->lnk),                \
    (_iter)->next.lnk = XOR2(&((_iter)->ccxll->head.lnk),                      \
                               (_iter)->ccxll->head.stnl->lnk)                 \
)


#define ccxll_iter_end(_iter)                                                  \
(                                                                              \
    (_iter)->next.lnk =      &((_iter)->ccxll->tail.lnk),                      \
    (_iter)->curr.lnk =      &((_iter)->ccxll->tail.stnl->lnk),                \
    (_iter)->prev.lnk = XOR2(&((_iter)->ccxll->tail.lnk),                      \
                               (_iter)->ccxll->tail.stnl->lnk)                 \
)


#define ccxll_iter_at_head(_iter)   ((_iter)->prev.lnk == NULL)

#define ccxll_iter_at_tail(_iter)   ((_iter)->next.lnk == NULL)

#define ccxll_iter_at_begin(_iter)  ((_iter)->curr.lnk ==                      \
                                     (_iter)->ccxll->head.lnk)

#define ccxll_iter_at_end(_iter)    ((_iter)->curr.lnk ==                      \
                                     (_iter)->ccxll->tail.lnk)


#define ccxll_iter_incr(_iter)                                                 \
(                                                                              \
    (ccxll_iter_at_tail(_iter)) ? (NULL) :                                     \
    (                                                                          \
        (_iter)->prev.lnk =      (_iter)->curr.lnk,                            \
        (_iter)->curr.lnk =      (_iter)->next.lnk,                            \
        (_iter)->next.lnk = XOR2((_iter)->prev.lnk, (_iter)->curr.node->lnk)   \
    )                                                                          \
)


#define ccxll_iter_decr(_iter)                                                 \
(                                                                              \
    (ccxll_iter_at_head(_iter)) ? (NULL) :                                     \
    (                                                                          \
        (_iter)->next.lnk =      (_iter)->curr.lnk,                            \
        (_iter)->curr.lnk =      (_iter)->prev.lnk,                            \
        (_iter)->prev.lnk = XOR2((_iter)->next.lnk, (_iter)->curr.node->lnk)   \
    )                                                                          \
)


#define ccxll_iter_advance(_iter, _diff)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _D = (_diff);                                                          \
                                                                               \
    if (_D > 0)       {  while (ccxll_iter_incr((_iter)) && --_D);  }          \
    else if (_D < 0)  {  while (ccxll_iter_decr((_iter)) && ++_D);  }          \
)


#define ccxll_iter_distance(_iter_a, _iter_b, _pdist)                          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_alloc((_iter_a)->ccxll, 1, _base_d);                                   \
                                                                               \
    ccxll_iter_copy(_it_((_iter_a)->ccxll, _base_d, 0), (_iter_a));            \
                                                                               \
    STATEMENT_                                                                 \
    (                                                                          \
        (*(_pdist)) = 0;                                                       \
        if ((_iter_a)->ccxll != (_iter_b)->ccxll)  break;                      \
                                                                               \
        while ((_iter_a)->curr.lnk != (_iter_b)->curr.lnk && ++(*(_pdist)))    \
               if (!(ccxll_iter_incr((_iter_a))))  break;                      \
                                                                               \
        if ((_iter_a)->curr.lnk == (_iter_b)->curr.lnk)  break;                \
        else  (*(_pdist)) = 0;                                                 \
                                                                               \
        ccxll_iter_copy((_iter_a), _it_((_iter_a)->ccxll, _base_d, 0));        \
                                                                               \
        while ((_iter_a)->curr.lnk != (_iter_b)->curr.lnk && --(*(_pdist)))    \
               if (!(ccxll_iter_decr((_iter_a))))  break;                      \
                                                                               \
        if ((_iter_a)->curr.lnk == (_iter_b)->curr.lnk)  break;                \
        else  (*(_pdist)) = 0;                                                 \
    );                                                                         \
                                                                               \
    ccxll_iter_copy((_iter_a), _it_((_iter_a)->ccxll, _base_d, 0));            \
                                                                               \
    _it_clear((_iter_a)->ccxll, 1);                                            \
)



/* ccxll traversal */


#define CCXLL_INCR(_iter)                                                      \
                                                                               \
    for (ccxll_iter_head((_iter)); ccxll_iter_incr((_iter)); )

#ifndef CCC_STRICT

#define CCXLL_INCR_DREF(_pval, _iter)                                          \
                                                                               \
    ccxll_iter_head((_iter));                                                  \
                                                                               \
    for (__typeof__((_iter)->curr.node->val) *_pval;                           \
         (ccxll_iter_incr((_iter))) && ((_pval) = &DREF((_iter)), 1); )

#endif // CCC_STRICT


#define CCXLL_DECR(_iter)                                                      \
                                                                               \
    for (ccxll_iter_tail((_iter)); ccxll_iter_decr((_iter)); )

#ifndef CCC_STRICT

#define CCXLL_DECR_DREF(_pval, _iter)                                          \
                                                                               \
    ccxll_iter_tail((_iter));                                                  \
                                                                               \
    for (__typeof__((_iter)->curr.node->val) *_pval;                           \
         (ccxll_iter_decr((_iter))) && ((_pval) = &DREF((_iter)), 1); )

#endif // CCC_STRICT



/* ccxll extensions */


#define ccxll_append(_ccxll_dst, _ccxll_src)                                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_alloc((_ccxll_src), 1, _base_a);                                       \
                                                                               \
    CCXLL_INCR(_it_((_ccxll_src), _base_a, 0))                                 \
        ccxll_push_back((_ccxll_dst), DREF(_it_((_ccxll_src), _base_a, 0)));   \
                                                                               \
    _it_clear((_ccxll_src), 1);                                                \
)


#define ccxll_copy(_ccxll_dst, _ccxll_src)                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    ccxll_free((_ccxll_dst));                                                  \
    ccxll_init_from((_ccxll_dst), (_ccxll_src));                               \
                                                                               \
    ccxll_append((_ccxll_dst), (_ccxll_src));                                  \
)


#define ccxll_rearrange(_ccxll)                                                \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _xl_alloc((_ccxll), 1, _base_r);                                           \
                                                                               \
     ccxll_copy(_xl_(_ccxll, _base_r, 0), (_ccxll));                           \
    _ccxll_swap((_ccxll), _xl_(_ccxll, _base_r, 0));                           \
                                                                               \
    _xl_clear((_ccxll), 1);                                                    \
)



#endif
