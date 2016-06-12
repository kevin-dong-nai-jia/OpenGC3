/*                                                                            *\
   Copyright (C) 2015-2016 Kevin Dong       Distributed under The MIT License

   Permission is  hereby granted,  free of charge,  to any person obtaining a
   copy of this software and associated documentation files  (the "Software"),
   to deal  in the Software without restriction, including without limitation
   the rights to  use, copy, modify,  merge, publish,  distribute, sublicense,
   and/or  sell copies  of the Software,  and  to permit persons  to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE  IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS  FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY,  WHETHER IN AN ACTION OF CONTRACT,  TORT OR OTHERWISE,  ARISING
   FROM,  OUT OF  OR  IN CONNECTION  WITH  THE SOFTWARE  OR THE USE  OR OTHER
   DEALINGS IN THE SOFTWARE.
\*                                                                            */


#ifndef _CCXLL_H_
#define _CCXLL_H_


#include <omp.h>      /* for parallel computing */
#include <stdio.h>    /* for printing out error messages */
#include <stdlib.h>   /* for handling ccxll memory pools */
#include <stddef.h>   /* for defining ptrdiff_t */
#include <stdint.h>   /* for defining uintptr_t */
#include <stdbool.h>  /* for defining type bool */


/* type definition */

typedef void* link_t;


/* pragma (struct) */

#define PRAGMA_PADDED_BEGIN
#define PRAGMA_PADDED_END

#define PRAGMA_PACKED_BEGIN  _Pragma("pack(push, 1)")
#define PRAGMA_PACKED_END    _Pragma("pack(pop)")


/* syntax wrappers */

#define STATEMENT_(...)  do {__VA_ARGS__} while (0)
#define VOID_EXPR_(...)  ((__VA_ARGS__), ((void)0))



/* ccxll create */


#define ccxll(elem_t)       ccxll_extd(elem_t, PADDED)

#define ccxll_pckd(elem_t)  ccxll_extd(elem_t, PACKED)

#define ccxll_extd(elem_t, _pack_)                                             \
                                                                               \
    struct                                                                     \
    {                                                                          \
        elem_t *head;                                                          \
        elem_t *tail;                                                          \
        elem_t *avsp;                                                          \
                                                                               \
        int       size, ncnt, vcnt;                                            \
        ptrdiff_t val_offset, xor_offset;                                      \
        int       blkstart, blkratio, blkthrsh;                                \
                                                                               \
        struct                                                                 \
        {                                                                      \
            link_t next;                                                       \
                                                                               \
            PRAGMA_##_pack_##_BEGIN                                            \
                                                                               \
            struct                                                             \
            {                                                                  \
                elem_t val;                                                    \
                link_t xor;                                                    \
                                                                               \
            }   *pnode, nodes[1];                                              \
                                                                               \
            PRAGMA_##_pack_##_END                                              \
                                                                               \
        }   *pool, block;                                                      \
    }


#define ccxll_iter(elem_t)       ccxll_iter_extd(elem_t, PADDED)

#define ccxll_iter_pckd(elem_t)  ccxll_iter_extd(elem_t, PACKED)

#define ccxll_iter_extd(elem_t, _pack_)                                        \
                                                                               \
    struct                                                                     \
    {                                                                          \
        elem_t *prev;                                                          \
        elem_t *curr;                                                          \
        elem_t *next;                                                          \
                                                                               \
        ccxll_extd(elem_t, _pack_) *pccxll;                                    \
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
    (puts("FATAL ERROR: Misalignment Issue"), exit(EXIT_FAILURE), 0) :         \
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
    _ccxll_blocks_free((_ccxll));                                              \
    _ccxll_reset_links((_ccxll));                                              \
)



/* node operations */


#define _ccxll_node_alloc(_pnode, _ccxll)                                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_ccxll).avsp == NULL)                                                 \
    {                                                                          \
        if ((_ccxll).vcnt == 0)                                                \
        {                                                                      \
            link_t pool_dup = (_ccxll).pool;                                   \
                                                                               \
            if ((_ccxll).ncnt == 0)                                            \
                (_ccxll).vcnt = ((_ccxll).ncnt  = (_ccxll).blkstart);          \
            else                                                               \
                (_ccxll).vcnt = ((_ccxll).ncnt  < (_ccxll).blkthrsh) ?         \
                                ((_ccxll).ncnt *= (_ccxll).blkratio) :         \
                                ((_ccxll).ncnt  = (_ccxll).blkthrsh);          \
                                                                               \
            (_ccxll).pool = malloc((sizeof((_ccxll).block)) +                  \
                                   (sizeof((_ccxll).block.nodes[0])) *         \
                                   ((_ccxll).ncnt - 1));                       \
                                                                               \
            if ((_ccxll).pool == NULL)                                         \
                puts("FATAL ERROR: Failed To Allocate"), exit(EXIT_FAILURE);   \
                                                                               \
            *(link_t*)(_ccxll).pool = pool_dup;                                \
        }                                                                      \
                                                                               \
        (_pnode) = &((_ccxll).pool->nodes[--(_ccxll).vcnt]);                   \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        (_pnode) = (void*)((_ccxll).avsp + (_ccxll).val_offset);               \
        (_ccxll).avsp = *(link_t*)(_ccxll).avsp;                               \
    }                                                                          \
)


#define _ccxll_node_clear(_pxor, _ccxll)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    *(link_t*)(_pxor) = (_ccxll).avsp;                                         \
    (_ccxll).avsp = (_pxor);                                                   \
)


#define _ccxll_blocks_free(_ccxll)                                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while ((_ccxll).pool != NULL)                                              \
    {                                                                          \
        link_t pool_dup = (_ccxll).pool;                                       \
                                                                               \
        (_ccxll).pool = *(link_t*)(_ccxll).pool;                               \
        free(pool_dup);                                                        \
    }                                                                          \
)



/* link exclusive or */


#define XOR_2(_addr_a, _addr_b)                                                \
(                                                                              \
    (link_t)((uintptr_t)(link_t)(_addr_a) ^ (uintptr_t)(link_t)(_addr_b))      \
)


#define XOR_3(_addr_a, _addr_b, _addr_c)                                       \
(                                                                              \
    (link_t)((uintptr_t)(link_t)(_addr_a) ^                                    \
             (uintptr_t)(link_t)(_addr_b) ^ (uintptr_t)(link_t)(_addr_c))      \
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

#define _ccxll_push(_ccxll, _val, _name_)                                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _ccxll_node_alloc((_ccxll).block.pnode, (_ccxll));                         \
                                                                               \
    (_ccxll).block.pnode->val = (_val);                                        \
    (_ccxll).block.pnode->xor = XOR_2(&((_ccxll)._name_), (_ccxll)._name_);    \
                                                                               \
    *(link_t*)(_ccxll)._name_ = XOR_3(&((_ccxll)._name_),                      \
                                      *(link_t*)(_ccxll)._name_,               \
                                      &((_ccxll).block.pnode->xor));           \
              (_ccxll)._name_ = (link_t)&((_ccxll).block.pnode->xor);          \
                                                                               \
    (_ccxll).size++;                                                           \
)


#define  ccxll_pop_front(_ccxll)  _ccxll_pop(_ccxll, head)

#define  ccxll_pop_back(_ccxll)   _ccxll_pop(_ccxll, tail)

#define _ccxll_pop(_ccxll, _name_)                                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_empty((_ccxll)))  break;                                         \
                                                                               \
    link_t pxor =   (_ccxll)._name_ ;                                          \
    link_t psen = &((_ccxll)._name_);                                          \
                                                                               \
              (_ccxll)._name_ = XOR_2(psen, *(link_t*)(_ccxll)._name_);        \
    *(link_t*)(_ccxll)._name_ = XOR_3(psen, *(link_t*)(_ccxll)._name_, pxor);  \
                                                                               \
    _ccxll_node_clear(pxor, (_ccxll));                                         \
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
    _ccxll_node_alloc((*(_iter).pccxll).block.pnode, *(_iter).pccxll);         \
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
    _ccxll_node_clear(pxor, *(_iter).pccxll);                                  \
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
        ccxll_merge_range_extd(_iter_l, _iter_m, _iter_r, _iter_x, CDLC)

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
        ccxll_sort_extd(_ccxll, _ptr4_iter_x, CDLC, 1)

#define ccxll_sort_extd(_ccxll, _ptr4_iter_x, _leq, _gap)                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccxll_empty(_ccxll))  break;                                           \
                                                                               \
    for (int cse = 1, gap = (_gap); ((cse != 2) && (cse = 1)); gap <<= 1)      \
    {                                                                          \
        ccxll_iter_begin((_ptr4_iter_x)[0], (_ccxll));                         \
        ccxll_iter_begin((_ptr4_iter_x)[1], (_ccxll));                         \
        ccxll_iter_begin((_ptr4_iter_x)[2], (_ccxll));                         \
                                                                               \
        while (!(ccxll_iter_at_tail((_ptr4_iter_x)[1])) && cse++)              \
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
    for (int cstl = 0; cstl < (_r); cstl++)                                    \
        size_half_sub[cstl] = (_ptrn_ccxll)[cstl].size;                        \
                                                                               \
    for (int cstl = 0, cstr = (_r); ((cstl < ((-1) * (_ccxll).size - 1)) &&    \
                             ((_ptrn_ccxll)[cstr].size != 0)); cstl++, cstr++) \
    {                                                                          \
        (_ccxll).size += 1;                                                    \
        (_ptrn_ccxll)[cstl].size += (_ptrn_ccxll)[cstr].size;                  \
        (_ptrn_ccxll)[cstr].size  = 0;                                         \
                                                                               \
        ccxll_iter_tail ((_ptrn4_iter_x)[cstl][0], (_ptrn_ccxll)[cstl]);       \
        ccxll_iter_begin((_ptrn4_iter_x)[cstl][1], (_ptrn_ccxll)[cstr]);       \
        ccxll_iter_tail ((_ptrn4_iter_x)[cstl][2], (_ptrn_ccxll)[cstr]);       \
        ccxll_move_range((_ptrn4_iter_x)[cstl][0], (_ptrn4_iter_x)[cstl][1],   \
                         (_ptrn4_iter_x)[cstl][2]);                            \
    }                                                                          \
                                                                               \
    _Pragma("omp parallel for")                                                \
    for (int cstl = 0; cstl < (_r); cstl++)                                    \
        ccxll_sort_extd((_ptrn_ccxll)[cstl], (_ptrn4_iter_x)[cstl], _leq,      \
                        size_half_sub[cstl]);                                  \
)


#define ccxll_sort_openmp(_ccxll, _ptrn_ccxll_x, _ptrn4_iter_x, _n)            \
        ccxll_sort_openmp_extd(_ccxll, _ptrn_ccxll_x, _ptrn4_iter_x, _n, CDLC)

#define ccxll_sort_openmp_extd(_ccxll, _ptrn_ccxll_x, _ptrn4_iter_x, _n, _leq) \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int size_all = (_ccxll).size,                                              \
        size_sub = (size_all / (_n)) + ((size_all % (_n) == 0) ? (0) : (1));   \
                                                                               \
    for (int csp = 0; csp < (_n); csp++)                                       \
    {                                                                          \
        bool suff = ((_ccxll).size >= size_sub);                               \
                                                                               \
        (_ptrn_ccxll_x)[csp].size = suff ? size_sub : (_ccxll).size;           \
        (_ccxll).size             = suff ? ((_ccxll).size - size_sub) : 0;     \
                                                                               \
        ccxll_iter_tail   ((_ptrn4_iter_x)[csp][0], (_ptrn_ccxll_x)[csp]);     \
        ccxll_iter_begin  ((_ptrn4_iter_x)[csp][1], (_ccxll));                 \
        ccxll_iter_begin  ((_ptrn4_iter_x)[csp][2], (_ccxll));                 \
        ccxll_iter_advance((_ptrn4_iter_x)[csp][2], size_sub);                 \
        ccxll_move_range  ((_ptrn4_iter_x)[csp][0], (_ptrn4_iter_x)[csp][1],   \
                           (_ptrn4_iter_x)[csp][2]);                           \
    }                                                                          \
                                                                               \
    (_ccxll).size = (-1) * (_n);                                               \
                                                                               \
    _Pragma("omp parallel for")                                                \
    for (int csp = 0; csp < (_n); csp++)                                       \
        ccxll_sort_extd((_ptrn_ccxll_x)[csp], (_ptrn4_iter_x)[csp], _leq, 1);  \
                                                                               \
    for (int csp = (_n); (csp = ((csp + (csp > 1 ? 1 : 0)) / 2)); )            \
        _ccxll_sort_two_sub((_ccxll), (_ptrn_ccxll_x),                         \
                                      (_ptrn4_iter_x), csp, _leq);             \
                                                                               \
    ccxll_iter_tail ((_ptrn4_iter_x)[0][0], (_ccxll));                         \
    ccxll_iter_begin((_ptrn4_iter_x)[0][1], (_ptrn_ccxll_x)[0]);               \
    ccxll_iter_tail ((_ptrn4_iter_x)[0][2], (_ptrn_ccxll_x)[0]);               \
    ccxll_move_range((_ptrn4_iter_x)[0][0], (_ptrn4_iter_x)[0][1],             \
                     (_ptrn4_iter_x)[0][2]);                                   \
                                                                               \
    (_ccxll).size = size_all;                                                  \
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


#define ccxll_iter_advance(_iter, _diff)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int diff = (_diff);                                                        \
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


#define CDLC  CCXLL_DEFAULT_LEQ_COMPARATOR

#define CCXLL_DEFAULT_LEQ_COMPARATOR(_iter_a, _iter_b)                         \
(                                                                              \
    ccxll_iter_dref((_iter_a)) <= ccxll_iter_dref((_iter_b))                   \
)



#endif
