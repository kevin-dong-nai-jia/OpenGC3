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



#ifndef _CCC_DLLST_H_
#define _CCC_DLLST_H_


#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>



/* syntax wrapper */


#define STATEMENT_(...) do {__VA_ARGS__} while (0)

#define VOID_EXPR_(...) ((__VA_ARGS__), ((void)0))



/* dllst constants */


#if (CC_DLLST_START - 0 <= 0)

    #undef  CC_DLLST_START
    #define CC_DLLST_START 16

#endif


#if (CC_DLLST_RATIO - 0 <= 0)

    #undef  CC_DLLST_RATIO
    #define CC_DLLST_RATIO 2

#endif


#if (CC_DLLST_THRSH - 0 <  CC_DLLST_START)

    #undef  CC_DLLST_THRSH
    #define CC_DLLST_THRSH 65536

#endif



/* dllst create */


#define link void*

#define cc_dllst(elem_t)                                                       \
                                                                               \
    struct                                                                     \
    {                                                                          \
        elem_t *head;                                                          \
        elem_t *tail;                                                          \
        elem_t *avsp;                                                          \
                                                                               \
        size_t    size, ncnt, vcnt;                                            \
        ptrdiff_t val_offset, xor_offset;                                      \
                                                                               \
        struct                                                                 \
        {                                                                      \
            link next;                                                         \
                                                                               \
            struct                                                             \
            {                                                                  \
                elem_t val;                                                    \
                link   xor;                                                    \
                                                                               \
            }   nodes[CC_DLLST_START], *pnode;                                 \
                                                                               \
        }   block, *pool;                                                      \
    }


#define cc_dllst_init(_dllst)                                                  \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_dllst).head = (link)&((_dllst).tail),                                    \
    (_dllst).tail = (link)&((_dllst).head),                                    \
    (_dllst).avsp = NULL,                                                      \
    (_dllst).pool = &((_dllst).block),                                         \
                                                                               \
    (_dllst).size = 0,                                                         \
    (_dllst).ncnt = CC_DLLST_START,                                            \
    (_dllst).vcnt = CC_DLLST_START,                                            \
                                                                               \
    (((_dllst).xor_offset = ((char*)&((_dllst).block.nodes[0].xor) -           \
                             (char*)&((_dllst).block.nodes[0].val))) %         \
     sizeof((_dllst).block.nodes[0].val)) ?                                    \
    (puts("FATAL ERROR: Misalignment Issue."), exit(EXIT_FAILURE), 0) :        \
    ((_dllst).val_offset = (_dllst).xor_offset * (-1) /                        \
                           sizeof((_dllst).block.nodes[0].val))                \
)



/* dllst iterator create */


#define cc_dllst_iter(elem_t)                                                  \
                                                                               \
    struct                                                                     \
    {                                                                          \
        elem_t *prev;                                                          \
        elem_t *curr;                                                          \
        elem_t *next;                                                          \
                                                                               \
        cc_dllst(elem_t) *pdllst;                                              \
                                                                               \
        struct                                                                 \
        {                                                                      \
            elem_t *prev;                                                      \
            elem_t *curr;                                                      \
            elem_t *next;                                                      \
                                                                               \
            cc_dllst(elem_t) *pdllst;                                          \
                                                                               \
        }   *piter;                                                            \
    }


#define cc_dllst_iter_init(_iter, _dllst)                                      \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).prev = NULL,                                                       \
    (_iter).curr = NULL,                                                       \
    (_iter).next = NULL,                                                       \
                                                                               \
    (_iter).pdllst = (void*)&(_dllst),                                         \
    (_iter).piter  = NULL                                                      \
)



/* bitwise operations */


#define XOR_2(_addr_a, _addr_b)                                                \
(                                                                              \
    (link)((uintptr_t)(link)(_addr_a) ^ (uintptr_t)(link)(_addr_b))            \
)


#define XOR_3(_addr_a, _addr_b, _addr_c)                                       \
(                                                                              \
    (link)((uintptr_t)(link)(_addr_a) ^                                        \
           (uintptr_t)(link)(_addr_b) ^ (uintptr_t)(link)(_addr_c))            \
)



/* dllst iterator operations */


#define cc_dllst_iter_dref(_iter)                                              \
(                                                                              \
    *((_iter).curr + (_iter).pdllst->val_offset)                               \
)


#define cc_dllst_iter_copy(_iter_dst, _iter_src)                               \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter_dst).prev = (_iter_src).prev,                                       \
    (_iter_dst).curr = (_iter_src).curr,                                       \
    (_iter_dst).next = (_iter_src).next                                        \
)


#define cc_dllst_iter_head(_iter, _dllst)                                      \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).prev = NULL,                                                       \
    (_iter).curr = (link)&((_dllst).head),                                     \
    (_iter).next = (_dllst).head                                               \
)


#define cc_dllst_iter_tail(_iter, _dllst)                                      \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).next = NULL,                                                       \
    (_iter).curr = (link)&((_dllst).tail),                                     \
    (_iter).prev = (_dllst).tail                                               \
)


#define cc_dllst_iter_begin(_iter, _dllst)                                     \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).prev = (link)&((_dllst).head),                                     \
    (_iter).curr = (_dllst).head,                                              \
    (_iter).next = XOR_2(&((_dllst).head), *(link*)(_dllst).head)              \
)


#define cc_dllst_iter_end(_iter, _dllst)                                       \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter).next = (link)&((_dllst).tail),                                     \
    (_iter).curr = (_dllst).tail,                                              \
    (_iter).prev = XOR_2(&((_dllst).tail), *(link*)(_dllst).tail)              \
)


#define cc_dllst_iter_incr(_iter)                                              \
(                                                                              \
    ((_iter).next != NULL) ?                                                   \
    (                                                                          \
        (_iter).prev = (_iter).curr,                                           \
        (_iter).curr = (_iter).next,                                           \
        (_iter).next = XOR_2((_iter).prev, *(link*)(_iter).curr)               \
                                                                               \
    ) : (void*)(NULL)                                                          \
)


#define cc_dllst_iter_decr(_iter)                                              \
(                                                                              \
    ((_iter).prev != NULL) ?                                                   \
    (                                                                          \
        (_iter).next = (_iter).curr,                                           \
        (_iter).curr = (_iter).prev,                                           \
        (_iter).prev = XOR_2((_iter).next, *(link*)(_iter).curr)               \
                                                                               \
    ) : (void*)(NULL)                                                          \
)


#define cc_dllst_iter_advance(_iter, _diff)                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int diff = (_diff);                                                        \
                                                                               \
    if (diff > 0)                                                              \
        while (cc_dllst_iter_incr((_iter)) && --diff);                         \
                                                                               \
    if (diff < 0)                                                              \
        while (cc_dllst_iter_decr((_iter)) && ++diff);                         \
)



/* dllst access */


#define cc_dllst_front(_dllst)                                                 \
(                                                                              \
    *((_dllst).head + (_dllst).val_offset)                                     \
)


#define cc_dllst_back(_dllst)                                                  \
(                                                                              \
    *((_dllst).tail + (_dllst).val_offset)                                     \
)



/* dllst capacity */


#define cc_dllst_size(_dllst)                                                  \
(                                                                              \
    (_dllst).size                                                              \
)


#define cc_dllst_empty(_dllst)                                                 \
(                                                                              \
    cc_dllst_size((_dllst)) == 0                                               \
)



/* dllst node operations */


#define _cc_dllst_node_alloc(_pnode, _dllst)                                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_dllst).avsp == NULL)                                                 \
    {                                                                          \
        if ((_dllst).vcnt == 0)                                                \
        {                                                                      \
            link pool_dup = (_dllst).pool;                                     \
                                                                               \
            if ((_dllst).ncnt < CC_DLLST_THRSH)                                \
                (_dllst).vcnt = ((_dllst).ncnt *= CC_DLLST_RATIO);             \
            else                                                               \
                (_dllst).vcnt = ((_dllst).ncnt  = CC_DLLST_THRSH);             \
                                                                               \
            (_dllst).pool = malloc(sizeof((_dllst).block) +                    \
                                   sizeof((_dllst).block.nodes[0]) *           \
                                   ((_dllst).ncnt - CC_DLLST_START));          \
                                                                               \
            *(link*)(_dllst).pool = pool_dup;                                  \
        }                                                                      \
                                                                               \
        (_pnode) = &((_dllst).pool->nodes[--(_dllst).vcnt]);                   \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        (_pnode) = (void*)((_dllst).avsp + (_dllst).val_offset);               \
        (_dllst).avsp = *(link*)(_dllst).avsp;                                 \
    }                                                                          \
)


#define _cc_dllst_node_clear(_pxor, _dllst)                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    *(link*)(_pxor) = (_dllst).avsp;                                           \
    (_dllst).avsp = (_pxor);                                                   \
)


#define _cc_dllst_blocks_free(_dllst)                                          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while ((_dllst).pool != &((_dllst).block))                                 \
    {                                                                          \
        link pool_dup = (_dllst).pool;                                         \
        (_dllst).pool = *(link*)(_dllst).pool;                                 \
        free(pool_dup);                                                        \
    }                                                                          \
)



/* dllst modifiers */


#define cc_dllst_push_front(_dllst, _value)                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _cc_dllst_node_alloc((_dllst).block.pnode, (_dllst));                      \
                                                                               \
    (_dllst).block.pnode->val = (_value);                                      \
    (_dllst).block.pnode->xor = XOR_2(&((_dllst).head), (_dllst).head);        \
                                                                               \
    *(link*)                                                                   \
    (_dllst).head = XOR_3(&((_dllst).head),                                    \
                          *(link*)(_dllst).head,                               \
                          &((_dllst).block.pnode->xor));                       \
    (_dllst).head = (link)&((_dllst).block.pnode->xor);                        \
                                                                               \
    (_dllst).size++;                                                           \
)


#define cc_dllst_push_back(_dllst, _value)                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _cc_dllst_node_alloc((_dllst).block.pnode, (_dllst));                      \
                                                                               \
    (_dllst).block.pnode->val = (_value);                                      \
    (_dllst).block.pnode->xor = XOR_2(&((_dllst).tail), (_dllst).tail);        \
                                                                               \
    *(link*)                                                                   \
    (_dllst).tail = XOR_3(&((_dllst).tail),                                    \
                          *(link*)(_dllst).tail,                               \
                          &((_dllst).block.pnode->xor));                       \
    (_dllst).tail = (link)&((_dllst).block.pnode->xor);                        \
                                                                               \
    (_dllst).size++;                                                           \
)


#define cc_dllst_pop_front(_dllst)                                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (!(cc_dllst_empty((_dllst))))                                           \
    {                                                                          \
        link head  =   (_dllst).head;                                          \
        link phead = &((_dllst).head);                                         \
                                                                               \
        (_dllst).head = XOR_2(phead, *(link*)(_dllst).head);                   \
        *(link*)                                                               \
        (_dllst).head = XOR_3(phead, *(link*)(_dllst).head, head);             \
                                                                               \
        _cc_dllst_node_clear(head, (_dllst));                                  \
                                                                               \
        (_dllst).size--;                                                       \
    }                                                                          \
)


#define cc_dllst_pop_back(_dllst)                                              \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (!(cc_dllst_empty((_dllst))))                                           \
    {                                                                          \
        link tail  =   (_dllst).tail;                                          \
        link ptail = &((_dllst).tail);                                         \
                                                                               \
        (_dllst).tail = XOR_2(ptail, *(link*)(_dllst).tail);                   \
        *(link*)                                                               \
        (_dllst).tail = XOR_3(ptail, *(link*)(_dllst).tail, tail);             \
                                                                               \
        _cc_dllst_node_clear(tail, (_dllst));                                  \
                                                                               \
        (_dllst).size--;                                                       \
    }                                                                          \
)


#define cc_dllst_insert(_iter, _value)                                         \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_iter).prev != NULL)                                                  \
    {                                                                          \
        link pxor;                                                             \
                                                                               \
        _cc_dllst_node_alloc((*(_iter).pdllst).block.pnode, *(_iter).pdllst);  \
                                                                               \
        (_iter).pdllst->block.pnode->val = (_value);                           \
                                                                               \
        (_iter).next = (_iter).curr;                                           \
        (_iter).curr = pxor = &((_iter).pdllst->block.pnode->xor);             \
                                                                               \
        *(link*)                                                               \
        (_iter).curr = XOR_2((_iter).prev, (_iter).next);                      \
        *(link*)                                                               \
        (_iter).prev = XOR_3(*(link*)(_iter).prev, (_iter).next, pxor);        \
        *(link*)                                                               \
        (_iter).next = XOR_3(*(link*)(_iter).next, (_iter).prev, pxor);        \
                                                                               \
        (_iter).pdllst->size++;                                                \
    }                                                                          \
)


#define cc_dllst_erase(_iter)                                                  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_iter).prev != NULL && (_iter).next != NULL)                          \
    {                                                                          \
        link pxor = (_iter).curr;                                              \
        (_iter).curr = (_iter).next;                                           \
                                                                               \
        *(link*)                                                               \
        (_iter).prev = XOR_3(*(link*)(_iter).prev, (_iter).next, pxor);        \
        *(link*)                                                               \
        (_iter).next = XOR_3(*(link*)(_iter).next, (_iter).prev, pxor);        \
                                                                               \
        (_iter).next = XOR_2(*(link*)(_iter).curr, (_iter).prev);              \
                                                                               \
        _cc_dllst_node_clear(pxor, *(_iter).pdllst);                           \
                                                                               \
        (_iter).pdllst->size--;                                                \
    }                                                                          \
)


#define cc_dllst_swap(_dllst_a, _dllst_b)                             /* TODO */


#define cc_dllst_resize(_dllst, _value)                               /* TODO */


#define cc_dllst_clear(_dllst)                                                 \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while (!(cc_dllst_empty((_dllst))))                                        \
        cc_dllst_pop_back((_dllst));                                           \
)



/* dllst operations */


#define cc_dllst_move_range(_iter_p, _iter_f, _iter_l)                         \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    link p_prev = (_iter_p).prev;                                              \
    link p_curr = (_iter_p).curr;                                              \
    link f_prev = (_iter_f).prev;                                              \
    link f_curr = (_iter_f).curr;                                              \
    link l_prev = (_iter_l).prev;                                              \
    link l_curr = (_iter_l).curr;                                              \
                                                                               \
    link *pp_next = (link*)&((_iter_p).next);                                  \
    link *pf_next = (link*)&((_iter_f).next);                                  \
    link *pl_next = (link*)&((_iter_l).next);                                  \
                                                                               \
    *(link*)p_prev = XOR_3(p_curr, f_curr, *(link*)p_prev);                    \
    *(link*)p_curr = XOR_3(p_prev, l_prev, *(link*)p_curr);                    \
    *(link*)f_prev = XOR_3(f_curr, l_curr, *(link*)f_prev);                    \
    *(link*)f_curr = XOR_3(f_prev, p_prev, *(link*)f_curr);                    \
    *(link*)l_prev = XOR_3(l_curr, p_curr, *(link*)l_prev);                    \
    *(link*)l_curr = XOR_3(l_prev, f_prev, *(link*)l_curr);                    \
                                                                               \
    if (*pp_next == f_curr)                                                    \
        *pp_next =  l_curr;                                                    \
    if (*pf_next == l_curr)                                                    \
        *pf_next =  p_curr;                                                    \
    if (*pl_next == p_curr)                                                    \
        *pl_next =  f_curr;                                                    \
                                                                               \
    (_iter_p).prev = XOR_2(*pp_next, *(link*)p_curr);                          \
    (_iter_f).prev = XOR_2(*pf_next, *(link*)f_curr);                          \
    (_iter_l).prev = XOR_2(*pl_next, *(link*)l_curr);                          \
)


#define cc_dllst_sort(_dllst)                                         /* TODO */



/* dllst traversal */


#define cc_dllst_trav(_dllst, _iter)                                           \
                                                                               \
    for (cc_dllst_iter_head((_iter), (_dllst)); cc_dllst_iter_incr((_iter)); )


#define cc_dllst_trav_back(_dllst, _iter)                                      \
                                                                               \
    for (cc_dllst_iter_tail((_iter), (_dllst)); cc_dllst_iter_decr((_iter)); )



/* dllst destroy */


#define cc_dllst_free(_dllst)                                                  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _cc_dllst_blocks_free((_dllst));                                           \
)



#endif
