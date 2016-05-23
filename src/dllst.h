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


#ifndef _DLLST_H_
#define _DLLST_H_


#include <omp.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


/* type definition */

typedef void*  link_t;


/* pragma (struct) */

#define PRAGMA_PADDED_BEGIN
#define PRAGMA_PADDED_END

#define PRAGMA_PACKED_BEGIN  _Pragma("pack(push, 1)")
#define PRAGMA_PACKED_END    _Pragma("pack(pop)")


/* syntax wrappers */

#define STATEMENT_(...)  do {__VA_ARGS__} while (0)
#define VOID_EXPR_(...)  ((__VA_ARGS__), ((void)0))



/* dllst create */


#define dllst(elem_t)       dllst_extd(elem_t, PADDED)

#define dllst_pckd(elem_t)  dllst_extd(elem_t, PACKED)

#define dllst_extd(elem_t, _pack)                                              \
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
            PRAGMA_##_pack##_BEGIN                                             \
                                                                               \
            struct                                                             \
            {                                                                  \
                elem_t val;                                                    \
                link_t xor;                                                    \
                                                                               \
            }   *pnode, nodes[1];                                              \
                                                                               \
            PRAGMA_##_pack##_END                                               \
                                                                               \
        }   *pool, block;                                                      \
    }


#define dllst_iter(elem_t)       dllst_iter_extd(elem_t, PADDED)

#define dllst_iter_pckd(elem_t)  dllst_iter_extd(elem_t, PACKED)

#define dllst_iter_extd(elem_t, PACK)                                          \
                                                                               \
    struct                                                                     \
    {                                                                          \
        elem_t *prev;                                                          \
        elem_t *curr;                                                          \
        elem_t *next;                                                          \
                                                                               \
        dllst_extd(elem_t, PACK) *pdllst;                                      \
    }



/* dllst initialize */


#define dllst_init(_dllst)  dllst_init_extd(_dllst, 16, 2, 65536)

#define dllst_init_extd(_dllst, _start, _ratio, _thrsh)                        \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_dllst).head = (link_t)&((_dllst).tail),                                  \
    (_dllst).tail = (link_t)&((_dllst).head),                                  \
    (_dllst).avsp = NULL,                                                      \
    (_dllst).pool = NULL,                                                      \
                                                                               \
    (_dllst).size = 0,                                                         \
    (_dllst).ncnt = 0,                                                         \
    (_dllst).vcnt = 0,                                                         \
                                                                               \
    (_dllst).blkstart = ((_start) >= 1) ? (_start) : 1,                        \
    (_dllst).blkratio = ((_ratio) >= 1) ? (_ratio) : 1,                        \
    (_dllst).blkthrsh = ((_thrsh) >= (_dllst).blkstart) ?                      \
                                          (_thrsh) : (_dllst).blkstart,        \
                                                                               \
    (((_dllst).xor_offset = ((char*)&((_dllst).block.nodes[0].xor) -           \
                             (char*)&((_dllst).block.nodes[0].val))) %         \
     sizeof((_dllst).block.nodes[0].val)) ?                                    \
    (puts("FATAL ERROR: Misalignment Issue"), exit(EXIT_FAILURE), 0) :         \
    ((_dllst).val_offset = (_dllst).xor_offset * (-1) /                        \
                           sizeof((_dllst).block.nodes[0].val))                \
)


#define dllst_iter_init(_iter, _dllst)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).prev = NULL,                                                       \
    (_iter).curr = NULL,                                                       \
    (_iter).next = NULL,                                                       \
                                                                               \
    (_iter).pdllst = (void*)&(_dllst)                                          \
)



/* dllst destroy */


#define dllst_free(_dllst)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _dllst_blocks_free((_dllst));                                              \
)



/* node operations */


#define _dllst_node_alloc(_pnode, _dllst)                                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_dllst).avsp == NULL)                                                 \
    {                                                                          \
        if ((_dllst).vcnt == 0)                                                \
        {                                                                      \
            link_t pool_dup = (_dllst).pool;                                   \
                                                                               \
            if ((_dllst).ncnt == 0)                                            \
                (_dllst).vcnt = ((_dllst).ncnt  = (_dllst).blkstart);          \
            else                                                               \
                (_dllst).vcnt = ((_dllst).ncnt  < (_dllst).blkthrsh) ?         \
                                ((_dllst).ncnt *= (_dllst).blkratio) :         \
                                ((_dllst).ncnt  = (_dllst).blkthrsh);          \
                                                                               \
            (_dllst).pool = malloc(sizeof((_dllst).block) +                    \
                                   sizeof((_dllst).block.nodes[0]) *           \
                                   ((_dllst).ncnt - 1));                       \
                                                                               \
            if ((_dllst).pool == NULL)                                         \
                puts("FATAL ERROR: Failed To Allocate"), exit(EXIT_FAILURE);   \
                                                                               \
            *(link_t*)(_dllst).pool = pool_dup;                                \
        }                                                                      \
                                                                               \
        (_pnode) = &((_dllst).pool->nodes[--(_dllst).vcnt]);                   \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        (_pnode) = (void*)((_dllst).avsp + (_dllst).val_offset);               \
        (_dllst).avsp = *(link_t*)(_dllst).avsp;                               \
    }                                                                          \
)


#define _dllst_node_clear(_pxor, _dllst)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    *(link_t*)(_pxor) = (_dllst).avsp;                                         \
    (_dllst).avsp = (_pxor);                                                   \
)


#define _dllst_blocks_free(_dllst)                                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while ((_dllst).pool != NULL)                                              \
    {                                                                          \
        link_t pool_dup = (_dllst).pool;                                       \
                                                                               \
        (_dllst).pool = *(link_t*)(_dllst).pool;                               \
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



/* dllst access */


#define dllst_front(_dllst)  (*((_dllst).head + (_dllst).val_offset))

#define dllst_back(_dllst)   (*((_dllst).tail + (_dllst).val_offset))



/* dllst capacity */


#define dllst_size(_dllst)   ((_dllst).size)

#define dllst_empty(_dllst)  (dllst_size((_dllst)) == 0)



/* dllst modifiers */


#define  dllst_push_front(_dllst, _val)  _dllst_push(_dllst, _val, head)

#define  dllst_push_back(_dllst, _val)   _dllst_push(_dllst, _val, tail)

#define _dllst_push(_dllst, _val, _name_)                                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _dllst_node_alloc((_dllst).block.pnode, (_dllst));                         \
                                                                               \
    (_dllst).block.pnode->val = (_val);                                        \
    (_dllst).block.pnode->xor = XOR_2(&((_dllst)._name_), (_dllst)._name_);    \
                                                                               \
    *(link_t*)                                                                 \
    (_dllst)._name_ = XOR_3(&((_dllst)._name_),                                \
                            *(link_t*)(_dllst)._name_,                         \
                            &((_dllst).block.pnode->xor));                     \
    (_dllst)._name_ = (link_t)&((_dllst).block.pnode->xor);                    \
                                                                               \
    (_dllst).size++;                                                           \
)


#define  dllst_pop_front(_dllst)  _dllst_pop(_dllst, head)

#define  dllst_pop_back(_dllst)   _dllst_pop(_dllst, tail)

#define _dllst_pop(_dllst, _name_)                                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (!(dllst_empty((_dllst))))                                              \
    {                                                                          \
        link_t    _name_ =   (_dllst)._name_;                                  \
        link_t p##_name_ = &((_dllst)._name_);                                 \
                                                                               \
        (_dllst)._name_ = XOR_2(p##_name_, *(link_t*)(_dllst)._name_);         \
        *(link_t*)                                                             \
        (_dllst)._name_ = XOR_3(p##_name_, *(link_t*)(_dllst)._name_, _name_); \
                                                                               \
        _dllst_node_clear(_name_, (_dllst));                                   \
                                                                               \
        (_dllst).size--;                                                       \
    }                                                                          \
)


#define dllst_insert(_iter, _val)                                              \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_iter).prev != NULL)                                                  \
    {                                                                          \
        link_t pxor;                                                           \
                                                                               \
        _dllst_node_alloc((*(_iter).pdllst).block.pnode, *(_iter).pdllst);     \
                                                                               \
        (_iter).pdllst->block.pnode->val = (_val);                             \
                                                                               \
        (_iter).next = (_iter).curr;                                           \
        (_iter).curr = pxor = &((_iter).pdllst->block.pnode->xor);             \
                                                                               \
        *(link_t*)                                                             \
        (_iter).curr = XOR_2((_iter).prev, (_iter).next);                      \
        *(link_t*)                                                             \
        (_iter).prev = XOR_3(*(link_t*)(_iter).prev, (_iter).next, pxor);      \
        *(link_t*)                                                             \
        (_iter).next = XOR_3(*(link_t*)(_iter).next, (_iter).prev, pxor);      \
                                                                               \
        (_iter).pdllst->size++;                                                \
    }                                                                          \
)


#define dllst_erase(_iter)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_iter).prev != NULL && (_iter).next != NULL)                          \
    {                                                                          \
        link_t pxor = (_iter).curr;                                            \
        (_iter).curr = (_iter).next;                                           \
                                                                               \
        *(link_t*)                                                             \
        (_iter).prev = XOR_3(*(link_t*)(_iter).prev, (_iter).next, pxor);      \
        *(link_t*)                                                             \
        (_iter).next = XOR_3(*(link_t*)(_iter).next, (_iter).prev, pxor);      \
                                                                               \
        (_iter).next = XOR_2(*(link_t*)(_iter).curr, (_iter).prev);            \
                                                                               \
        _dllst_node_clear(pxor, *(_iter).pdllst);                              \
                                                                               \
        (_iter).pdllst->size--;                                                \
    }                                                                          \
)


#define dllst_swap(_dllst_a, _dllst_b)                                /* TODO */


#define dllst_resize(_dllst, _value)                                  /* TODO */


#define dllst_clear(_dllst)                                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while (!(dllst_empty((_dllst))))                                           \
        dllst_pop_back((_dllst));                                              \
)



/* dllst operations */


#define dllst_move_range(_iter_p, _iter_l, _iter_r)                            \
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


#define dllst_merge_range(_iter_l, _iter_m, _iter_r, _iter_x)                  \
        dllst_merge_range_extd(_iter_l, _iter_m, _iter_r, _iter_x, DDLC)

#define dllst_merge_range_extd(_iter_l, _iter_m, _iter_r, _iter_x, _leq)       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_iter_m).next == NULL)  break;                                        \
                                                                               \
    dllst_iter_copy((_iter_x), (_iter_m));                                     \
                                                                               \
    while (1)                                                                  \
    {                                                                          \
        while ((_iter_l).curr != (_iter_m).curr && _leq((_iter_l), (_iter_m))) \
            dllst_iter_incr((_iter_l));                                        \
                                                                               \
        if ((_iter_l).curr == (_iter_m).curr)                                  \
        {                                                                      \
            dllst_iter_copy((_iter_l), (_iter_r));                             \
            dllst_iter_copy((_iter_m), (_iter_r));  break;                     \
        }                                                                      \
                                                                               \
        dllst_iter_incr((_iter_x));                                            \
                                                                               \
        while ((_iter_x).curr != (_iter_r).curr && _leq((_iter_x), (_iter_l))) \
            dllst_iter_incr((_iter_x));                                        \
                                                                               \
        dllst_move_range((_iter_l), (_iter_m), (_iter_x));                     \
        dllst_iter_copy ((_iter_m), (_iter_x));                                \
                                                                               \
        if ((_iter_x).curr == (_iter_r).curr)                                  \
        {                                                                      \
            dllst_iter_copy((_iter_l), (_iter_x));                             \
            dllst_iter_copy((_iter_r), (_iter_x));  break;                     \
        }                                                                      \
    }                                                                          \
)


#define dllst_sort(_dllst, _ptr4_iter_x)                                       \
        dllst_sort_extd(_dllst, _ptr4_iter_x, DDLC, 1)

#define dllst_sort_extd(_dllst, _ptr4_iter_x, _leq, _gap)                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (dllst_empty(_dllst))  break;                                           \
                                                                               \
    for (int cnt = 1, gap = (_gap); ((cnt != 2) && (cnt = 1)); gap <<= 1)      \
    {                                                                          \
        dllst_iter_begin((_ptr4_iter_x)[0], (_dllst));                         \
        dllst_iter_begin((_ptr4_iter_x)[1], (_dllst));                         \
        dllst_iter_begin((_ptr4_iter_x)[2], (_dllst));                         \
                                                                               \
        while ((_ptr4_iter_x)[1].next != NULL && cnt++)                        \
        {                                                                      \
            dllst_iter_advance((_ptr4_iter_x)[1], gap);                        \
            dllst_iter_copy   ((_ptr4_iter_x)[2], (_ptr4_iter_x)[1]);          \
            dllst_iter_advance((_ptr4_iter_x)[2], gap);                        \
                                                                               \
            dllst_merge_range_extd((_ptr4_iter_x)[0], (_ptr4_iter_x)[1],       \
                                   (_ptr4_iter_x)[2], (_ptr4_iter_x)[3], _leq);\
        }                                                                      \
    }                                                                          \
)


#define _dllst_sort_two_sub(_dllst, _ptrn_dllst, _ptrn4_iter_x, _r, _leq)      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int size_half_sub[(_r)];                                                   \
                                                                               \
    for (int cstl = 0; cstl < (_r); cstl++)                                    \
        size_half_sub[cstl] = (_ptrn_dllst)[cstl].size;                        \
                                                                               \
    for (int cstl = 0, cstr = (_r); ((cstl < ((-1) * (_dllst).size - 1)) &&    \
                             ((_ptrn_dllst)[cstr].size != 0)); cstl++, cstr++) \
    {                                                                          \
        (_dllst).size += 1;                                                    \
        (_ptrn_dllst)[cstl].size += (_ptrn_dllst)[cstr].size;                  \
        (_ptrn_dllst)[cstr].size  = 0;                                         \
                                                                               \
        dllst_iter_tail ((_ptrn4_iter_x)[cstl][0], (_ptrn_dllst)[cstl]);       \
        dllst_iter_begin((_ptrn4_iter_x)[cstl][1], (_ptrn_dllst)[cstr]);       \
        dllst_iter_tail ((_ptrn4_iter_x)[cstl][2], (_ptrn_dllst)[cstr]);       \
        dllst_move_range((_ptrn4_iter_x)[cstl][0], (_ptrn4_iter_x)[cstl][1],   \
                         (_ptrn4_iter_x)[cstl][2]);                            \
    }                                                                          \
                                                                               \
    _Pragma("omp parallel for")                                                \
    for (int cstl = 0; cstl < (_r); cstl++)                                    \
        dllst_sort_extd((_ptrn_dllst)[cstl], (_ptrn4_iter_x)[cstl], _leq,      \
                        size_half_sub[cstl]);                                  \
)


#define dllst_sort_openmp(_dllst, _ptrn_dllst_x, _ptrn4_iter_x, _n)            \
        dllst_sort_openmp_extd(_dllst, _ptrn_dllst_x, _ptrn4_iter_x, _n, DDLC)

#define dllst_sort_openmp_extd(_dllst, _ptrn_dllst_x, _ptrn4_iter_x, _n, _leq) \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int size_all = (_dllst).size,                                              \
        size_sub = (size_all / (_n)) + ((size_all % (_n) == 0) ? (0) : (1));   \
                                                                               \
    for (int csp = 0; csp < (_n); csp++)                                       \
    {                                                                          \
        bool suff = ((_dllst).size >= size_sub);                               \
                                                                               \
        (_ptrn_dllst_x)[csp].size = suff ? size_sub : (_dllst).size;           \
        (_dllst).size             = suff ? ((_dllst).size - size_sub) : 0;     \
                                                                               \
        dllst_iter_tail   ((_ptrn4_iter_x)[csp][0], (_ptrn_dllst_x)[csp]);     \
        dllst_iter_begin  ((_ptrn4_iter_x)[csp][1], (_dllst));                 \
        dllst_iter_begin  ((_ptrn4_iter_x)[csp][2], (_dllst));                 \
        dllst_iter_advance((_ptrn4_iter_x)[csp][2], size_sub);                 \
        dllst_move_range  ((_ptrn4_iter_x)[csp][0], (_ptrn4_iter_x)[csp][1],   \
                           (_ptrn4_iter_x)[csp][2]);                           \
    }                                                                          \
                                                                               \
    (_dllst).size = (-1) * (_n);                                               \
                                                                               \
    _Pragma("omp parallel for")                                                \
    for (int csp = 0; csp < (_n); csp++)                                       \
        dllst_sort_extd((_ptrn_dllst_x)[csp], (_ptrn4_iter_x)[csp], _leq, 1);  \
                                                                               \
    for (int csp = (_n); (csp = ((csp + (csp > 1 ? 1 : 0)) / 2)); )            \
        _dllst_sort_two_sub((_dllst), (_ptrn_dllst_x),                         \
                                      (_ptrn4_iter_x), csp, _leq);             \
                                                                               \
    dllst_iter_tail ((_ptrn4_iter_x)[0][0], (_dllst));                         \
    dllst_iter_begin((_ptrn4_iter_x)[0][1], (_ptrn_dllst_x)[0]);               \
    dllst_iter_tail ((_ptrn4_iter_x)[0][2], (_ptrn_dllst_x)[0]);               \
    dllst_move_range((_ptrn4_iter_x)[0][0], (_ptrn4_iter_x)[0][1],             \
                     (_ptrn4_iter_x)[0][2]);                                   \
                                                                               \
    (_dllst).size = size_all;                                                  \
)



/* dllst iterators */


#define dllst_iter_dref(_iter)  (*((_iter).curr + (_iter).pdllst->val_offset))


#define dllst_iter_copy(_iter_dst, _iter_src)                                  \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter_dst).prev = (_iter_src).prev,                                       \
    (_iter_dst).curr = (_iter_src).curr,                                       \
    (_iter_dst).next = (_iter_src).next                                        \
)


#define dllst_iter_head(_iter, _dllst)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).prev = NULL,                                                       \
    (_iter).curr = (link_t)&((_dllst).head),                                   \
    (_iter).next = (_dllst).head                                               \
)


#define dllst_iter_tail(_iter, _dllst)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).next = NULL,                                                       \
    (_iter).curr = (link_t)&((_dllst).tail),                                   \
    (_iter).prev = (_dllst).tail                                               \
)


#define dllst_iter_begin(_iter, _dllst)                                        \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).prev = (link_t)&((_dllst).head),                                   \
    (_iter).curr = (_dllst).head,                                              \
    (_iter).next = XOR_2(&((_dllst).head), *(link_t*)(_dllst).head)            \
)


#define dllst_iter_end(_iter, _dllst)                                          \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).next = (link_t)&((_dllst).tail),                                   \
    (_iter).curr = (_dllst).tail,                                              \
    (_iter).prev = XOR_2(&((_dllst).tail), *(link_t*)(_dllst).tail)            \
)


#define dllst_iter_incr(_iter)                                                 \
(                                                                              \
    ((_iter).next == NULL) ? (void*)(NULL) :                                   \
    (                                                                          \
        (_iter).prev = (_iter).curr,                                           \
        (_iter).curr = (_iter).next,                                           \
        (_iter).next = XOR_2((_iter).prev, *(link_t*)(_iter).curr)             \
    )                                                                          \
)


#define dllst_iter_decr(_iter)                                                 \
(                                                                              \
    ((_iter).prev == NULL) ? (void*)(NULL) :                                   \
    (                                                                          \
        (_iter).next = (_iter).curr,                                           \
        (_iter).curr = (_iter).prev,                                           \
        (_iter).prev = XOR_2((_iter).next, *(link_t*)(_iter).curr)             \
    )                                                                          \
)


#define dllst_iter_advance(_iter, _diff)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int diff = (_diff);                                                        \
                                                                               \
    if (diff > 0)                                                              \
        while (dllst_iter_incr((_iter)) && --diff);                            \
    else if (diff < 0)                                                         \
        while (dllst_iter_decr((_iter)) && ++diff);                            \
)



/* dllst traversal */


#define DLLST_TRAVERSAL(_dllst, _iter)  DLLST_FORWARD_TRAVERSAL(_dllst, _iter)

#define DLLST_FORWARD_TRAVERSAL(_dllst, _iter)                                 \
                                                                               \
    for (dllst_iter_head((_iter), (_dllst)); dllst_iter_incr((_iter)); )


#define DLLST_BACKWARD_TRAVERSAL(_dllst, _iter)                                \
                                                                               \
    for (dllst_iter_tail((_iter), (_dllst)); dllst_iter_decr((_iter)); )



/* default comparators */


#define DDLC  DLLST_DEFAULT_LEQ_COMPARATOR

#define DLLST_DEFAULT_LEQ_COMPARATOR(_iter_a, _iter_b)                         \
(                                                                              \
    dllst_iter_dref((_iter_a)) <= dllst_iter_dref((_iter_b))                   \
)



#endif
