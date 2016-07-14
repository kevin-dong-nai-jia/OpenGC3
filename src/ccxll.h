#ifndef _CCXLL_H_
#define _CCXLL_H_

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <omp.h>

#include "misc.h"
#include "node.h"


/* ccxll create */


#define ccxll(elem_t)       ccxll_extd(elem_t, _PADDED_)

#define ccxll_pckd(elem_t)  ccxll_extd(elem_t, _PACKED_)

#define ccxll_extd(elem_t, _PACK_)                                             \
                                                                               \
    struct  {                                                                  \
        elem_t *head;                                                          \
        elem_t *tail;                                                          \
        elem_t *avsp;                                                          \
        ptrdiff_t val_offset, xor_offset;                                      \
        long long size, ncnt, vcnt, blkstart, blkratio, blkthrsh;              \
                                                                               \
        struct  {                                                              \
            link_t next;                                                       \
                                                                               \
            PRAGMA##_PACK_##BGN                                                \
            struct  {                                                          \
                elem_t val;                                                    \
                link_t xor;                                                    \
                                                                               \
            }   *pnode, nodes[1];                                              \
            PRAGMA##_PACK_##END                                                \
                                                                               \
        }   *pool, block;                                                      \
    }


#define ccxll_iter(elem_t)       ccxll_iter_extd(elem_t, _PADDED_)

#define ccxll_iter_pckd(elem_t)  ccxll_iter_extd(elem_t, _PACKED_)

#define ccxll_iter_extd(elem_t, _PACK_)                                        \
                                                                               \
    struct  {                                                                  \
        elem_t *prev;                                                          \
        elem_t *curr;                                                          \
        elem_t *next;                                                          \
        ccxll_extd(elem_t, _PACK_) *pccxll;                                    \
    }



/* ccxll initialize */


#define ccxll_init(_ccxll)  ccxll_init_extd(_ccxll, 16, 2, 65536)

#define ccxll_init_extd(_ccxll, _start, _ratio, _thrsh)                        \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    _ccxll_reset_links((_ccxll)),                                              \
                                                                               \
    (_ccxll).blkstart = ((_start) >= 1) ? (_start) : 1,                        \
    (_ccxll).blkratio = ((_ratio) >= 1) ? (_ratio) : 1,                        \
    (_ccxll).blkthrsh = ((_thrsh) >= (_ccxll).blkstart) ? (_thrsh) :           \
                                                          (_ccxll).blkstart,   \
                                                                               \
    (_ccxll).xor_offset = ((char*)&((_ccxll).block.nodes[0].xor) -             \
                           (char*)&((_ccxll).block.nodes[0].val)),             \
                                                                               \
    ((_ccxll).xor_offset % (sizeof((_ccxll).block.nodes[0].val))) ?            \
    (fputs("FATAL ERROR: Misalignment", stderr), exit(EXIT_FAILURE), 0) :      \
    ((_ccxll).val_offset = (_ccxll).xor_offset * (-1) /                        \
                           (sizeof((_ccxll).block.nodes[0].val)))              \
)


#define _ccxll_reset_links(_ccxll)                                             \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccxll).head = (link_t)&((_ccxll).tail),                                  \
    (_ccxll).tail = (link_t)&((_ccxll).head),                                  \
    (_ccxll).avsp = NULL,                                                      \
    (_ccxll).pool = NULL,                                                      \
    (_ccxll).size = (_ccxll).ncnt = (_ccxll).vcnt = 0                          \
)


#define ccxll_iter_init(_iter, _ccxll)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).prev = (_iter).curr = (_iter).next = NULL,                         \
    (_iter).pccxll = (void*)&(_ccxll)                                          \
)



/* ccxll destroy */


#define ccxll_free(_ccxll)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _nodes_free((_ccxll));                                                     \
    _ccxll_reset_links((_ccxll));                                              \
)



/* exclusive or */


#define XOR_2(_addr_a, _addr_b)                                                \
(                                                                              \
    (link_t)((uintptr_t)(void*)(_addr_a) ^ (uintptr_t)(void*)(_addr_b))        \
)


#define XOR_3(_addr_a, _addr_b, _addr_c)                                       \
(                                                                              \
    (link_t)((uintptr_t)(void*)(_addr_a) ^ (uintptr_t)(void*)(_addr_b) ^       \
             (uintptr_t)(void*)(_addr_c))                                      \
)


/* ccxll access */


#define ccxll_front(_ccxll)  (*((_ccxll).head + (_ccxll).val_offset))

#define ccxll_back(_ccxll)   (*((_ccxll).tail + (_ccxll).val_offset))



/* ccxll capacity */


#define ccxll_size(_ccxll)   ((_ccxll).size)

#define ccxll_empty(_ccxll)  (ccxll_size((_ccxll)) == 0)



/* ccxll modifiers */


#define  ccxll_push_front(_ccxll, _val)  _ccxll_push(_ccxll, _val, head)

#define  ccxll_push_back(_ccxll, _val)   _ccxll_push(_ccxll, _val, tail)

#define _ccxll_push(_ccxll, _val, _ends_)                                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _node_alloc((_ccxll).block.pnode, (_ccxll));                               \
                                                                               \
    (_ccxll).block.pnode->val = (_val);                                        \
    (_ccxll).block.pnode->xor = XOR_2(&((_ccxll)._ends_), (_ccxll)._ends_);    \
                                                                               \
    *(link_t*)(_ccxll)._ends_ = XOR_3(&((_ccxll)._ends_),                      \
                                      *(link_t*)(_ccxll)._ends_,               \
                                      &((_ccxll).block.pnode->xor));           \
    (_ccxll)._ends_ = (link_t)&((_ccxll).block.pnode->xor);                    \
                                                                               \
    (_ccxll).size++;                                                           \
)


#define  ccxll_pop_front(_ccxll)  _ccxll_pop(_ccxll, head)

#define  ccxll_pop_back(_ccxll)   _ccxll_pop(_ccxll, tail)

#define _ccxll_pop(_ccxll, _ends_)                                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_empty((_ccxll)))  break;                                         \
                                                                               \
    link_t pxor =   (_ccxll)._ends_ ;                                          \
    link_t psen = &((_ccxll)._ends_);                                          \
                                                                               \
    (_ccxll)._ends_           = XOR_2(psen, *(link_t*)(_ccxll)._ends_);        \
    *(link_t*)(_ccxll)._ends_ = XOR_3(psen, *(link_t*)(_ccxll)._ends_, pxor);  \
                                                                               \
    _node_clear(pxor, (_ccxll));                                               \
                                                                               \
    (_ccxll).size--;                                                           \
)


#define ccxll_insert(_iter, _val)                                              \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_iter_at_head(_iter))  break;                                     \
                                                                               \
    link_t pxor;                                                               \
                                                                               \
    _node_alloc((*(_iter).pccxll).block.pnode, *(_iter).pccxll);               \
    (_iter).pccxll->block.pnode->val = (_val);                                 \
                                                                               \
    (_iter).next = (_iter).curr;                                               \
    (_iter).curr = pxor = &((_iter).pccxll->block.pnode->xor);                 \
                                                                               \
    *(link_t*)(_iter).curr = XOR_2((_iter).prev, (_iter).next);                \
    *(link_t*)(_iter).prev = XOR_3(*(link_t*)(_iter).prev, (_iter).next, pxor);\
    *(link_t*)(_iter).next = XOR_3(*(link_t*)(_iter).next, (_iter).prev, pxor);\
                                                                               \
    (_iter).pccxll->size++;                                                    \
)


#define ccxll_erase(_iter)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_iter_at_head(_iter) || ccxll_iter_at_tail(_iter))  break;        \
                                                                               \
    link_t pxor = (_iter).curr;                                                \
                                                                               \
    *(link_t*)(_iter).prev = XOR_3(*(link_t*)(_iter).prev, (_iter).next, pxor);\
    *(link_t*)(_iter).next = XOR_3(*(link_t*)(_iter).next, (_iter).prev, pxor);\
                                                                               \
    (_iter).curr = (_iter).next;                                               \
    (_iter).next = XOR_2(*(link_t*)(_iter).curr, (_iter).prev);                \
                                                                               \
    _node_clear(pxor, *(_iter).pccxll);                                        \
                                                                               \
    (_iter).pccxll->size--;                                                    \
)


#define ccxll_swap(_ccxll_a, _ccxll_b)                                /* TODO */


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
    link_t p_prev = (_iter_p).prev;                                            \
    link_t p_curr = (_iter_p).curr;                                            \
    link_t l_prev = (_iter_l).prev;                                            \
    link_t l_curr = (_iter_l).curr;                                            \
    link_t r_prev = (_iter_r).prev;                                            \
    link_t r_curr = (_iter_r).curr;                                            \
                                                                               \
    if (l_curr == r_curr || p_curr == l_curr || p_curr == r_curr)  break;      \
                                                                               \
    *(link_t*)p_prev = XOR_3(p_curr, l_curr, *(link_t*)p_prev);                \
    *(link_t*)p_curr = XOR_3(p_prev, r_prev, *(link_t*)p_curr);                \
    *(link_t*)l_prev = XOR_3(l_curr, r_curr, *(link_t*)l_prev);                \
    *(link_t*)l_curr = XOR_3(l_prev, p_prev, *(link_t*)l_curr);                \
    *(link_t*)r_prev = XOR_3(r_curr, p_curr, *(link_t*)r_prev);                \
    *(link_t*)r_curr = XOR_3(r_prev, l_prev, *(link_t*)r_curr);                \
                                                                               \
    (_iter_p).next = ((_iter_p).next == l_curr) ? (r_curr) : ((_iter_p).next); \
    (_iter_l).next = ((_iter_l).next == r_curr) ? (p_curr) : ((_iter_l).next); \
    (_iter_r).next = ((_iter_r).next == p_curr) ? (l_curr) : ((_iter_r).next); \
                                                                               \
    (_iter_p).prev = XOR_2((_iter_p).next, *(link_t*)p_curr);                  \
    (_iter_l).prev = XOR_2((_iter_l).next, *(link_t*)l_curr);                  \
    (_iter_r).prev = XOR_2((_iter_r).next, *(link_t*)r_curr);                  \
)


#define ccxll_merge_range(_iter_l, _iter_m, _iter_r, _iter_x)                  \
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


#define ccxll_sort(_ccxll, _ptr4_iter_x)                                       \
        ccxll_sort_extd(_ccxll, _ptr4_iter_x,  1, XLEQ)

#define ccxll_sort_extd(_ccxll, _ptr4_iter_x, _g, _leq)                        \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_empty(_ccxll))  break;                                           \
                                                                               \
    for (int c_mg = 0, gap = (_g); c_mg != 1 && !(c_mg = 0); gap <<= 1)        \
    {                                                                          \
        ccxll_iter_begin((_ptr4_iter_x)[0], (_ccxll));                         \
        ccxll_iter_begin((_ptr4_iter_x)[1], (_ccxll));                         \
        ccxll_iter_begin((_ptr4_iter_x)[2], (_ccxll));                         \
                                                                               \
        while (!(ccxll_iter_at_tail((_ptr4_iter_x)[1])) && ++c_mg)             \
        {                                                                      \
            ccxll_iter_advance((_ptr4_iter_x)[1], gap);                        \
            ccxll_iter_copy   ((_ptr4_iter_x)[2], (_ptr4_iter_x)[1]);          \
            ccxll_iter_advance((_ptr4_iter_x)[2], gap);                        \
                                                                               \
            ccxll_merge_range_extd((_ptr4_iter_x)[0], (_ptr4_iter_x)[1],       \
                                   (_ptr4_iter_x)[2], (_ptr4_iter_x)[3], _leq);\
        }                                                                      \
    }                                                                          \
)


#define _ccxll_sort_two_sub(_ccxll, _ptrn_ccxll, _ptrn4_iter_x, _r, _leq)      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int size_half_sub[(_r)];                                                   \
                                                                               \
    for (int c_lf = 0; c_lf < (_r); c_lf++)                                    \
        size_half_sub[c_lf] = (_ptrn_ccxll)[c_lf].size;                        \
                                                                               \
    for (int c_lf = 0, c_rg = (_r); ((c_lf < ((-1) * (_ccxll).size - 1)) &&    \
                             ((_ptrn_ccxll)[c_rg].size != 0)); c_lf++, c_rg++) \
    {                                                                          \
        (_ccxll).size += 1;                                                    \
        (_ptrn_ccxll)[c_lf].size += (_ptrn_ccxll)[c_rg].size;                  \
        (_ptrn_ccxll)[c_rg].size  = 0;                                         \
                                                                               \
        ccxll_iter_tail ((_ptrn4_iter_x)[c_lf][0], (_ptrn_ccxll)[c_lf]);       \
        ccxll_iter_begin((_ptrn4_iter_x)[c_lf][1], (_ptrn_ccxll)[c_rg]);       \
        ccxll_iter_tail ((_ptrn4_iter_x)[c_lf][2], (_ptrn_ccxll)[c_rg]);       \
        ccxll_move_range((_ptrn4_iter_x)[c_lf][0], (_ptrn4_iter_x)[c_lf][1],   \
                         (_ptrn4_iter_x)[c_lf][2]);                            \
    }                                                                          \
                                                                               \
    _Pragma("omp parallel for")                                                \
    for (int c_lf = 0; c_lf < (_r); c_lf++)                                    \
        ccxll_sort_extd((_ptrn_ccxll)[c_lf], (_ptrn4_iter_x)[c_lf],            \
                        size_half_sub[c_lf], _leq);                            \
)


#define ccxll_sort_openmp(_ccxll, _ptrn_ccxll_x, _ptrn4_iter_x, _n)            \
        ccxll_sort_openmp_extd(_ccxll, _ptrn_ccxll_x, _ptrn4_iter_x, _n, XLEQ)

#define ccxll_sort_openmp_extd(_ccxll, _ptrn_ccxll_x, _ptrn4_iter_x, _n, _leq) \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int size_all = (_ccxll).size;                                              \
    int size_sub = (size_all / (_n)) + (size_all % (_n) != 0);                 \
                                                                               \
    for (int c_th = 0; c_th < (_n); c_th++)                                    \
    {                                                                          \
        bool suff = ((_ccxll).size >= size_sub);                               \
                                                                               \
        (_ptrn_ccxll_x)[c_th].size = suff ? size_sub : (_ccxll).size;          \
        (_ccxll).size              = suff ? ((_ccxll).size - size_sub) : 0;    \
                                                                               \
        ccxll_iter_tail   ((_ptrn4_iter_x)[c_th][0], (_ptrn_ccxll_x)[c_th]);   \
        ccxll_iter_begin  ((_ptrn4_iter_x)[c_th][1], (_ccxll));                \
        ccxll_iter_begin  ((_ptrn4_iter_x)[c_th][2], (_ccxll));                \
        ccxll_iter_advance((_ptrn4_iter_x)[c_th][2], size_sub);                \
        ccxll_move_range  ((_ptrn4_iter_x)[c_th][0], (_ptrn4_iter_x)[c_th][1], \
                           (_ptrn4_iter_x)[c_th][2]);                          \
    }                                                                          \
                                                                               \
    (_ccxll).size = (-1) * (_n);                                               \
                                                                               \
    _Pragma("omp parallel for")                                                \
    for (int c_th = 0; c_th < (_n); c_th++)                                    \
        ccxll_sort_extd((_ptrn_ccxll_x)[c_th], (_ptrn4_iter_x)[c_th], 1, _leq);\
                                                                               \
    for (int c_th = (_n); (c_th = ((c_th + (c_th > 1)) / 2)); )                \
        _ccxll_sort_two_sub((_ccxll), (_ptrn_ccxll_x),                         \
                                      (_ptrn4_iter_x), c_th, _leq);            \
                                                                               \
    ccxll_iter_tail ((_ptrn4_iter_x)[0][0], (_ccxll));                         \
    ccxll_iter_begin((_ptrn4_iter_x)[0][1], (_ptrn_ccxll_x)[0]);               \
    ccxll_iter_tail ((_ptrn4_iter_x)[0][2], (_ptrn_ccxll_x)[0]);               \
    ccxll_move_range((_ptrn4_iter_x)[0][0], (_ptrn4_iter_x)[0][1],             \
                     (_ptrn4_iter_x)[0][2]);                                   \
                                                                               \
    (_ccxll).size = size_all;                                                  \
)


#define ccxll_reverse_range(_iter_l, _iter_r)                                  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    *(link_t*)(_iter_l).next = XOR_3(*(link_t*)(_iter_l).next,                 \
                                     (_iter_l).curr, (_iter_r).curr);          \
    *(link_t*)(_iter_r).prev = XOR_3(*(link_t*)(_iter_r).prev,                 \
                                     (_iter_r).curr, (_iter_l).curr);          \
                                                                               \
    *(link_t*)(_iter_l).curr = XOR_3(*(link_t*)(_iter_l).curr,                 \
                                     (_iter_l).next, (_iter_r).prev);          \
    *(link_t*)(_iter_r).curr = XOR_3(*(link_t*)(_iter_r).curr,                 \
                                     (_iter_r).prev, (_iter_l).next);          \
                                                                               \
    (_iter_l).next = XOR_2((_iter_l).prev, *(link_t*)(_iter_l).curr);          \
    (_iter_r).next = XOR_2((_iter_r).prev, *(link_t*)(_iter_r).curr);          \
)



/* ccxll iterators */


#define ccxll_iter_dref(_iter)  (*((_iter).curr + (_iter).pccxll->val_offset))


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
    (_iter).prev = NULL,                                                       \
    (_iter).curr = (link_t)&((_ccxll).head),                                   \
    (_iter).next = (_ccxll).head                                               \
)


#define ccxll_iter_tail(_iter, _ccxll)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).next = NULL,                                                       \
    (_iter).curr = (link_t)&((_ccxll).tail),                                   \
    (_iter).prev = (_ccxll).tail                                               \
)


#define ccxll_iter_begin(_iter, _ccxll)                                        \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).prev = (link_t)&((_ccxll).head),                                   \
    (_iter).curr = (_ccxll).head,                                              \
    (_iter).next = XOR_2(&((_ccxll).head), *(link_t*)(_ccxll).head)            \
)


#define ccxll_iter_end(_iter, _ccxll)                                          \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).next = (link_t)&((_ccxll).tail),                                   \
    (_iter).curr = (_ccxll).tail,                                              \
    (_iter).prev = XOR_2(&((_ccxll).tail), *(link_t*)(_ccxll).tail)            \
)


#define ccxll_iter_at_head(_iter)           ((_iter).prev == NULL)

#define ccxll_iter_at_tail(_iter)           ((_iter).next == NULL)

#define ccxll_iter_at_begin(_iter, _ccxll)  ((_iter).curr == (_ccxll).head)

#define ccxll_iter_at_end(_iter, _ccxll)    ((_iter).curr == (_ccxll).tail)


#define ccxll_iter_incr(_iter)                                                 \
(                                                                              \
    (ccxll_iter_at_tail(_iter)) ? (void*)(NULL) :                              \
    (                                                                          \
        (_iter).prev = (_iter).curr,                                           \
        (_iter).curr = (_iter).next,                                           \
        (_iter).next = XOR_2((_iter).prev, *(link_t*)(_iter).curr)             \
    )                                                                          \
)


#define ccxll_iter_decr(_iter)                                                 \
(                                                                              \
    (ccxll_iter_at_head(_iter)) ? (void*)(NULL) :                              \
    (                                                                          \
        (_iter).next = (_iter).curr,                                           \
        (_iter).curr = (_iter).prev,                                           \
        (_iter).prev = XOR_2((_iter).next, *(link_t*)(_iter).curr)             \
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


#define CCXLL_TRAV(_ccxll, _iter)  CCXLL_FORWARD_TRAVERSAL(_ccxll, _iter)

#define CCXLL_FORWARD_TRAVERSAL(_ccxll, _iter)                                 \
                                                                               \
    for (ccxll_iter_head((_iter), (_ccxll)); ccxll_iter_incr((_iter)); )


#define CCXLL_BACKWARD_TRAVERSAL(_ccxll, _iter)                                \
                                                                               \
    for (ccxll_iter_tail((_iter), (_ccxll)); ccxll_iter_decr((_iter)); )



/* default comparators */


#define XLEQ  CCXLL_DEFAULT_LEQ_COMPARATOR

#define CCXLL_DEFAULT_LEQ_COMPARATOR(_iter_a, _iter_b)                         \
(                                                                              \
    ccxll_iter_dref((_iter_a)) <= ccxll_iter_dref((_iter_b))                   \
)



#endif
