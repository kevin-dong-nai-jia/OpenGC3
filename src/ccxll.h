#ifndef _CCXLL_H_
#define _CCXLL_H_

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "misc.h"
#include "node.h"


/* ccxll create */


#define ccxll(elem_t)       ccxll_extd(elem_t, 0, NORMAL)

#define ccxll_pckd(elem_t)  ccxll_extd(elem_t, 0, PACKED)

#define _S_CCXLL  ADDID(_CCXLL_)
#define _S_NODE   ADDID(_CCXLL_NODE_ )
#define _U_HDTL   ADDID(_CCXLL_HDTL_ )
#define _S_BLOCK  ADDID(_CCXLL_BLOCK_)
#define _S_ITER   ADDID(_CCXLL_ITER_ )

#define ccxll_extd(elem_t, n_iter, _ALIGN_)                                    \
                                                                               \
    struct _S_CCXLL                                                            \
    {                                                                          \
        long long int  size , used , vcnt ;       /* size and node record */   \
        long long int  start, ratio, thrsh;       /* block increment info */   \
                                                                               \
        struct _S_NODE                                                         \
        {   link_t lnk;                                                        \
            elem_t val;                           /* val with an xor link */   \
        }   *avsp, *pnode;                        /* available space list */   \
                                                                               \
        union  _U_HDTL                                                         \
        {   link_t lnk;                                                        \
            struct _S_NODE *stnl;                                              \
        }   head, tail;                                                        \
                                                                               \
        struct _S_BLOCK                                                        \
        {   struct _S_BLOCK *next;                /* points to next block */   \
            PRAGMA_##_ALIGN_##_BGN                /* packed pragma starts */   \
            struct _S_NODE nodes[1];              /* node structure array */   \
            PRAGMA_##_ALIGN_##_END                /* the pragma ends here */   \
        }   *pool, *pblock;                       /* points to 1-st block */   \
                                                                               \
        struct _S_ITER                                                         \
        {   struct _S_NODE  *prev, *curr, *next;  /* adjacent ptr to node */   \
            struct _S_CCXLL *pccxll;              /* points to ccxll body */   \
        }   **_it, it[((n_iter) >= 1) ? (n_iter + 1) : 1];                     \
                                                                               \
        struct _S_CCXLL **_xl;                                                 \
    }                                                                          \



/* ccxll initialize */


#define ccxll_init(_ccxll)                                                     \
                                                                               \
        ccxll_init_extd((_ccxll), 16, 2, 65536)

#define ccxll_init_from(_ccxll_dst, _ccxll_src)                                \
                                                                               \
        ccxll_init_extd((_ccxll_dst),       (_ccxll_src).start,                \
                        (_ccxll_src).ratio, (_ccxll_src).thrsh)

#define ccxll_init_extd(_ccxll, _start, _ratio, _thrsh)                        \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    _ccxll_init_info((_ccxll), (_start), (_ratio), (_thrsh)),                  \
    _ccxll_init_core((_ccxll)),                                                \
     ccxll_iter_init((_ccxll).it[0], (_ccxll))                                 \
)


#define ccxll_full_init(_ccxll)                                                \
                                                                               \
        ccxll_full_init_extd((_ccxll), 16, 2, 65536)

#define ccxll_full_init_extd(_ccxll, _start, _ratio, _thrsh)                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    ccxll_init_extd((_ccxll), (_start), (_ratio), (_thrsh));                   \
                                                                               \
    for (int idx = 1; idx < ELEMOF_ARR((_ccxll).it); idx++)                    \
        ccxll_iter_init((_ccxll).it[idx], (_ccxll));                           \
)


#define _ccxll_init_info(_ccxll, _start, _ratio, _thrsh)                       \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccxll).start = ((_start) >= 1) ? (_start) : 1,                           \
    (_ccxll).ratio = ((_ratio) >= 1) ? (_ratio) : 1,                           \
    (_ccxll).thrsh = ((_thrsh) >= (_ccxll).start) ? (_thrsh) : (_ccxll).start  \
)


#define _ccxll_init_core(_ccxll)                                               \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccxll).head.lnk = &((_ccxll).tail),                                      \
    (_ccxll).tail.lnk = &((_ccxll).head),                                      \
    (_ccxll).avsp = (_ccxll).pnode  = NULL,                                    \
    (_ccxll).pool = (_ccxll).pblock = NULL,                                    \
    (_ccxll).size = (_ccxll).used = (_ccxll).vcnt = 0,                         \
    (_ccxll)._it = NULL, (_ccxll)._xl = NULL                                   \
)


#define ccxll_iter_init(_iter, _ccxll)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).prev = (_iter).curr = (_iter).next = NULL,                         \
    (_iter).pccxll = &(_ccxll)                                                 \
)



/* ccxll destroy */


#define ccxll_free(_ccxll)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _block_free((_ccxll));                                                     \
    _ccxll_init_core((_ccxll));                                                \
)



/* exclusive or */


#define XOR_2(_addr_a, _addr_b)                                                \
(                                                                              \
    (void*)((uintptr_t)(void*)(_addr_a) ^                                      \
            (uintptr_t)(void*)(_addr_b))                                       \
)


#define XOR_3(_addr_a, _addr_b, _addr_c)                                       \
(                                                                              \
    (void*)((uintptr_t)(void*)(_addr_a) ^                                      \
            (uintptr_t)(void*)(_addr_b) ^                                      \
            (uintptr_t)(void*)(_addr_c))                                       \
)


/* ccxll access */


#define ccxll_front(_ccxll)  ((_ccxll).head.stnl->val)

#define ccxll_back(_ccxll)   ((_ccxll).tail.stnl->val)



/* ccxll capacity */


#define ccxll_size(_ccxll)   ((_ccxll).size)

#define ccxll_empty(_ccxll)  (!(ccxll_size((_ccxll))))



/* ccxll modifiers */


#define  ccxll_push_front(_ccxll, _val)  _ccxll_push(_ccxll, _val, head)

#define  ccxll_push_back(_ccxll, _val)   _ccxll_push(_ccxll, _val, tail)

#define _ccxll_push(_ccxll, _val, _hdtl_)                                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _node_alloc((_ccxll).pnode, (_ccxll));                                     \
                                                                               \
    (_ccxll).pnode->val  = (_val);                                             \
                                                                               \
    (_ccxll).pnode->lnk       = XOR_2(&((_ccxll)._hdtl_.lnk),                  \
                                        (_ccxll)._hdtl_.lnk);                  \
                                                                               \
    (_ccxll)._hdtl_.stnl->lnk = XOR_3(&((_ccxll)._hdtl_.lnk),                  \
                                        (_ccxll)._hdtl_.stnl->lnk,             \
                                      &((_ccxll).pnode->lnk));                 \
                                                                               \
    (_ccxll)._hdtl_.lnk = &((_ccxll).pnode->lnk);                              \
                                                                               \
    (_ccxll).size++;                                                           \
)


#define  ccxll_pop_front(_ccxll)  _ccxll_pop(_ccxll, head)

#define  ccxll_pop_back(_ccxll)   _ccxll_pop(_ccxll, tail)

#define _ccxll_pop(_ccxll, _hdtl_)                                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_empty((_ccxll)))  break;                                         \
                                                                               \
    (_ccxll).pnode = (_ccxll)._hdtl_.stnl;                                     \
                                                                               \
    (_ccxll)._hdtl_.lnk       = XOR_2(&((_ccxll)._hdtl_.lnk),                  \
                                        (_ccxll)._hdtl_.stnl->lnk);            \
                                                                               \
    (_ccxll)._hdtl_.stnl->lnk = XOR_3(&((_ccxll)._hdtl_.lnk),                  \
                                        (_ccxll)._hdtl_.stnl->lnk,             \
                                      &((_ccxll).pnode->lnk));                 \
                                                                               \
    _node_clear((_ccxll).pnode, (_ccxll));                                     \
                                                                               \
    (_ccxll).size--;                                                           \
)


#define ccxll_insert(_iter, _val)                                              \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    link_t lnk;                                                                \
                                                                               \
    if (ccxll_iter_at_head(_iter))  break;                                     \
                                                                               \
    _node_alloc((_iter).pccxll->pnode, *(_iter).pccxll);                       \
                                                                               \
    (_iter).pccxll->pnode->val = (_val);                                       \
                                                                               \
    (_iter).next = (_iter).curr;                                               \
    (_iter).curr = (_iter).pccxll->pnode;                                      \
                                                                               \
    lnk = &((_iter).pccxll->pnode->lnk);                                       \
                                                                               \
    (_iter).curr->lnk = XOR_2(&((_iter).prev->lnk), &((_iter).next->lnk));     \
    (_iter).prev->lnk = XOR_3(  (_iter).prev->lnk , &((_iter).next->lnk), lnk);\
    (_iter).next->lnk = XOR_3(  (_iter).next->lnk , &((_iter).prev->lnk), lnk);\
                                                                               \
    (_iter).pccxll->size++;                                                    \
)


#define ccxll_erase(_iter)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    link_t lnk = &((_iter).curr->lnk);                                         \
                                                                               \
    if (ccxll_iter_at_head(_iter) || ccxll_iter_at_tail(_iter))  break;        \
                                                                               \
    (_iter).prev->lnk = XOR_3((_iter).prev->lnk, &((_iter).next->lnk), lnk);   \
    (_iter).next->lnk = XOR_3((_iter).next->lnk, &((_iter).prev->lnk), lnk);   \
                                                                               \
    _node_clear((_iter).curr, *(_iter).pccxll);                                \
                                                                               \
    (_iter).curr =       (_iter).next;                                         \
    (_iter).next = XOR_2((_iter).curr->lnk, &((_iter).prev->lnk));             \
                                                                               \
    (_iter).pccxll->size--;                                                    \
)


#define ccxll_swap(_ccxll_a, _ccxll_b)                                         \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    size_t sz_ct_a = (size_t)((char*)&((_ccxll_a)._it) - (char*)&(_ccxll_a));  \
    size_t sz_ct_b = (size_t)((char*)&((_ccxll_b)._it) - (char*)&(_ccxll_b));  \
    size_t sz_copy = ((sz_ct_a == sz_ct_b) ? (sz_ct_a) : (0));                 \
                                                                               \
    if (sz_copy)                                                               \
    {                                                                          \
        void *_ccxll_temp = malloc(sz_copy);                                   \
                                                                               \
        memcpy(_ccxll_temp, &(_ccxll_a), sz_copy);                             \
        memcpy(&(_ccxll_a), &(_ccxll_b), sz_copy);                             \
        memcpy(&(_ccxll_b), _ccxll_temp, sz_copy);                             \
                                                                               \
        link_t lnk_h = XOR_2(&((_ccxll_a).head.lnk), &((_ccxll_b).head.lnk));  \
        link_t lnk_t = XOR_2(&((_ccxll_a).tail.lnk), &((_ccxll_b).tail.lnk));  \
                                                                               \
        (_ccxll_a).head.stnl->lnk = XOR_2((_ccxll_a).head.stnl->lnk, lnk_h);   \
        (_ccxll_a).tail.stnl->lnk = XOR_2((_ccxll_a).tail.stnl->lnk, lnk_t);   \
        (_ccxll_b).head.stnl->lnk = XOR_2((_ccxll_b).head.stnl->lnk, lnk_h);   \
        (_ccxll_b).tail.stnl->lnk = XOR_2((_ccxll_b).tail.stnl->lnk, lnk_t);   \
                                                                               \
        free(_ccxll_temp);                                                     \
    }                                                                          \
)


#define ccxll_resize(_ccxll, _value)                                  /* TODO */


#define ccxll_clear(_ccxll)                                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while (!(ccxll_empty((_ccxll))))  {  ccxll_pop_back((_ccxll));  }          \
)



/* ccxll operations */


#define ccxll_move_range(_iter_p, _iter_l, _iter_r)                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    link_t p_prev = (_iter_p).prev, p_curr = (_iter_p).curr;                   \
    link_t l_prev = (_iter_l).prev, l_curr = (_iter_l).curr;                   \
    link_t r_prev = (_iter_r).prev, r_curr = (_iter_r).curr;                   \
                                                                               \
    if (l_curr == r_curr || p_curr == l_curr || p_curr == r_curr)  break;      \
                                                                               \
    (_iter_p).prev->lnk = XOR_3((_iter_p).prev->lnk, p_curr, l_curr);          \
    (_iter_p).curr->lnk = XOR_3((_iter_p).curr->lnk, p_prev, r_prev);          \
    (_iter_l).prev->lnk = XOR_3((_iter_l).prev->lnk, l_curr, r_curr);          \
    (_iter_l).curr->lnk = XOR_3((_iter_l).curr->lnk, l_prev, p_prev);          \
    (_iter_r).prev->lnk = XOR_3((_iter_r).prev->lnk, r_curr, p_curr);          \
    (_iter_r).curr->lnk = XOR_3((_iter_r).curr->lnk, r_prev, l_prev);          \
                                                                               \
    (_iter_p).next = ((_iter_p).next == l_curr) ? (r_curr) : ((_iter_p).next); \
    (_iter_l).next = ((_iter_l).next == r_curr) ? (p_curr) : ((_iter_l).next); \
    (_iter_r).next = ((_iter_r).next == p_curr) ? (l_curr) : ((_iter_r).next); \
                                                                               \
    (_iter_p).prev = XOR_2((_iter_p).curr->lnk, (_iter_p).next);               \
    (_iter_l).prev = XOR_2((_iter_l).curr->lnk, (_iter_l).next);               \
    (_iter_r).prev = XOR_2((_iter_r).curr->lnk, (_iter_r).next);               \
)


#define ccxll_merge_range(_iter_l, _iter_m, _iter_r, _iter_x)                  \
                                                                               \
        ccxll_merge_range_extd(_iter_l, _iter_m, _iter_r, _iter_x, XLEQ)

#define ccxll_merge_range_extd(_iter_l, _iter_m, _iter_r, _iter_x, _leq)       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_iter_at_tail(_iter_m))  break;                                   \
                                                                               \
    ccxll_iter_copy((_iter_x), (_iter_m));                                     \
                                                                               \
    while (1)                                                                  \
    {                                                                          \
        while ((_iter_l).curr != (_iter_m).curr && _leq((_iter_l), (_iter_m))) \
            ccxll_iter_incr((_iter_l));                                        \
                                                                               \
        if ((_iter_l).curr == (_iter_m).curr)                                  \
        {                                                                      \
            ccxll_iter_copy((_iter_l), (_iter_r));                             \
            ccxll_iter_copy((_iter_m), (_iter_r));  break;                     \
        }                                                                      \
                                                                               \
        ccxll_iter_incr((_iter_x));                                            \
                                                                               \
        while ((_iter_x).curr != (_iter_r).curr && _leq((_iter_x), (_iter_l))) \
            ccxll_iter_incr((_iter_x));                                        \
                                                                               \
        ccxll_move_range((_iter_l), (_iter_m), (_iter_x));                     \
        ccxll_iter_copy ((_iter_m), (_iter_x));                                \
                                                                               \
        if ((_iter_x).curr == (_iter_r).curr)                                  \
        {                                                                      \
            ccxll_iter_copy((_iter_l), (_iter_x));                             \
            ccxll_iter_copy((_iter_r), (_iter_x));  break;                     \
        }                                                                      \
    }                                                                          \
)


#define ccxll_sort(_ccxll)  ccxll_sort_extd(_ccxll, 1, XLEQ)

#define ccxll_sort_extd(_ccxll, _g, _leq)                                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_empty(_ccxll) || (_ccxll)._it != NULL)  break;                   \
                                                                               \
    (_ccxll)._it = malloc(4 * sizeof(*(_ccxll)._it));                          \
                                                                               \
    for (int c_it = 0; c_it < 4; c_it++)                                       \
        (_ccxll)._it[c_it] = malloc(sizeof(**(_ccxll)._it)),                   \
        ccxll_iter_init(*(_ccxll)._it[c_it], (_ccxll));                        \
                                                                               \
    for (int c_mg = 0, gap = (_g); c_mg != 1 && !(c_mg = 0); gap <<= 1)        \
    {                                                                          \
        ccxll_iter_begin(*(_ccxll)._it[0], (_ccxll));                          \
        ccxll_iter_begin(*(_ccxll)._it[1], (_ccxll));                          \
        ccxll_iter_begin(*(_ccxll)._it[2], (_ccxll));                          \
                                                                               \
        while (!(ccxll_iter_at_tail(*(_ccxll)._it[1])) && ++c_mg)              \
        {                                                                      \
            ccxll_iter_advance(*(_ccxll)._it[1], gap);                         \
            ccxll_iter_copy   (*(_ccxll)._it[2], *(_ccxll)._it[1]);            \
            ccxll_iter_advance(*(_ccxll)._it[2], gap);                         \
                                                                               \
            ccxll_merge_range_extd(*(_ccxll)._it[0], *(_ccxll)._it[1],         \
                                   *(_ccxll)._it[2], *(_ccxll)._it[3], _leq);  \
        }                                                                      \
    }                                                                          \
                                                                               \
    for (int c_it = 0; c_it < 4; c_it++)                                       \
        free((_ccxll)._it[c_it]);                                              \
    free((_ccxll)._it);                                                        \
                                                                               \
    (_ccxll)._it = NULL;                                                       \
)


#define ccxll_reverse_range(_iter_l, _iter_r)                                  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_iter_l).next->lnk = XOR_3(&((_iter_l).curr->lnk),                        \
                                &((_iter_r).curr->lnk), (_iter_l).next->lnk);  \
    (_iter_r).prev->lnk = XOR_3(&((_iter_r).curr->lnk),                        \
                                &((_iter_l).curr->lnk), (_iter_r).prev->lnk);  \
                                                                               \
    (_iter_l).curr->lnk = XOR_3(&((_iter_l).next->lnk),                        \
                                &((_iter_r).prev->lnk), (_iter_l).curr->lnk);  \
    (_iter_r).curr->lnk = XOR_3(&((_iter_r).prev->lnk),                        \
                                &((_iter_l).next->lnk), (_iter_r).curr->lnk);  \
                                                                               \
    (_iter_l).next      = XOR_2(&((_iter_l).prev->lnk),                        \
                                &((_iter_l).curr->lnk));                       \
    (_iter_r).next      = XOR_2(&((_iter_r).prev->lnk),                        \
                                &((_iter_r).curr->lnk));                       \
)



/* default comparators */


#define XLEQ  CCXLL_LEQ_COMPAR

#define CCXLL_LEQ_COMPAR(_iter_a, _iter_b)                                     \
(                                                                              \
    ccxll_iter_dref((_iter_a)) <= ccxll_iter_dref((_iter_b))                   \
)



/* ccxll iterators */


#define dref       ccxll_iter_dref
#define dref_prev  ccxll_iter_dref_prev
#define dref_next  ccxll_iter_dref_next

#define ccxll_iter_dref(_iter)       ((_iter).curr->val)

#define ccxll_iter_dref_prev(_iter)  ((_iter).prev->val)

#define ccxll_iter_dref_next(_iter)  ((_iter).next->val)


#define ccxll_iter_copy(_iter_dst, _iter_src)                                  \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter_dst).prev = (_iter_src).prev,                                       \
    (_iter_dst).curr = (_iter_src).curr,                                       \
    (_iter_dst).next = (_iter_src).next                                        \
)


#define ccxll_iter_head(_iter, _ccxll)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).prev =  NULL,                                                      \
    (_iter).curr = (void*)&((_ccxll).head.lnk),                                \
    (_iter).next =          (_ccxll).head.lnk                                  \
)


#define ccxll_iter_tail(_iter, _ccxll)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).next =  NULL,                                                      \
    (_iter).curr = (void*)&((_ccxll).tail.lnk),                                \
    (_iter).prev =          (_ccxll).tail.lnk                                  \
)


#define ccxll_iter_begin(_iter, _ccxll)                                        \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).prev = (void*)&((_ccxll).head.lnk),                                \
    (_iter).curr =          (_ccxll).head.stnl,                                \
    (_iter).next =  XOR_2(&((_ccxll).head.lnk), (_ccxll).head.stnl->lnk)       \
)


#define ccxll_iter_end(_iter, _ccxll)                                          \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).next = (void*)&((_ccxll).tail.lnk),                                \
    (_iter).curr =          (_ccxll).tail.stnl,                                \
    (_iter).prev =  XOR_2(&((_ccxll).tail.lnk), (_ccxll).tail.stnl->lnk)       \
)


#define ccxll_iter_at_head(_iter)           ((_iter).prev == NULL)

#define ccxll_iter_at_tail(_iter)           ((_iter).next == NULL)

#define ccxll_iter_at_begin(_iter, _ccxll)  ((_iter).curr == (_ccxll).head.stnl)

#define ccxll_iter_at_end(_iter, _ccxll)    ((_iter).curr == (_ccxll).tail.stnl)


#define ccxll_iter_incr(_iter)                                                 \
(                                                                              \
    (ccxll_iter_at_tail(_iter)) ? (void*)(NULL) :                              \
    (                                                                          \
        (_iter).prev =       (_iter).curr,                                     \
        (_iter).curr =       (_iter).next,                                     \
        (_iter).next = XOR_2((_iter).prev, (_iter).curr->lnk)                  \
    )                                                                          \
)


#define ccxll_iter_decr(_iter)                                                 \
(                                                                              \
    (ccxll_iter_at_head(_iter)) ? (void*)(NULL) :                              \
    (                                                                          \
        (_iter).next =       (_iter).curr,                                     \
        (_iter).curr =       (_iter).prev,                                     \
        (_iter).prev = XOR_2((_iter).next, (_iter).curr->lnk)                  \
    )                                                                          \
)


#define ccxll_iter_advance(_iter, _d)                                          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int diff = (_d);                                                           \
                                                                               \
    if (diff > 0)       {  while (ccxll_iter_incr((_iter)) && --diff);  }      \
    else if (diff < 0)  {  while (ccxll_iter_decr((_iter)) && ++diff);  }      \
)



/* ccxll traversal */


#define CCXLL_TRAV(_ccxll)                                                     \
                                                                               \
        CCXLL_FORWARD_TRAVERSAL((_ccxll), (_ccxll).it[0])

#define CCXLL_FORWARD_TRAVERSAL(_ccxll, _iter)                                 \
                                                                               \
    for (ccxll_iter_head((_iter), (_ccxll)); ccxll_iter_incr((_iter)); )


#define CCXLL_BACKWARD_TRAVERSAL(_ccxll, _iter)                                \
                                                                               \
    for (ccxll_iter_tail((_iter), (_ccxll)); ccxll_iter_decr((_iter)); )



/* ccxll extensions */


#define ccxll_copy(_ccxll_dst, _ccxll_src)                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    ccxll_free((_ccxll_dst));                                                  \
                                                                               \
    CCXLL_TRAV((_ccxll_src))                                                   \
        ccxll_push_back((_ccxll_dst), ccxll_iter_dref((_ccxll_src).it[0]));    \
)


#define ccxll_rearrange(_ccxll)                                                \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_ccxll)._xl != NULL)  break;                                          \
                                                                               \
    (_ccxll)._xl    = malloc(sizeof( *(_ccxll)._xl));                          \
    (_ccxll)._xl[0] = malloc(sizeof(**(_ccxll)._xl));                          \
    ccxll_init_from(*(_ccxll)._xl[0], (_ccxll));                               \
                                                                               \
    ccxll_copy(*(_ccxll)._xl[0], (_ccxll));                                    \
    ccxll_swap(*(_ccxll)._xl[0], (_ccxll));                                    \
    ccxll_free(*(_ccxll)._xl[0]);                                              \
                                                                               \
    free((_ccxll)._xl[0]);                                                     \
    free((_ccxll)._xl   );                                                     \
                                                                               \
    (_ccxll)._xl = NULL;                                                       \
)



#endif
