/*                                                                            *\
   Copyright (C) 2015-2016  Kevin Dong Nai Jia      - The MIT License -

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



/* dllst constants */


#if (CCC_DLLST_START - 0 <= 0)

    #undef  CCC_DLLST_START
    #define CCC_DLLST_START 16

#endif


#if (CCC_DLLST_RATIO - 0 <= 0)

    #undef  CCC_DLLST_RATIO
    #define CCC_DLLST_RATIO 2

#endif


#if (CCC_DLLST_LIMIT - 0 <= CCC_DLLST_START)

    #undef  CCC_DLLST_LIMIT
    #define CCC_DLLST_LIMIT CCC_DLLST_START

#endif



/* dllst create */


typedef void* link_t;

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
            link_t next;                                                       \
                                                                               \
            struct                                                             \
            {                                                                  \
                elem_t val;                                                    \
                link_t xor;                                                    \
                                                                               \
            }   nodes[CCC_DLLST_START], *pnode;                                \
                                                                               \
        }   block, *pool;                                                      \
    }


#define cc_dllst_init(_cc_dllst)                                               \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    (_cc_dllst).head = (link_t)&((_cc_dllst).tail),                            \
    (_cc_dllst).tail = (link_t)&((_cc_dllst).head),                            \
    (_cc_dllst).avsp = NULL,                                                   \
    (_cc_dllst).pool = &((_cc_dllst).block),                                   \
                                                                               \
    (_cc_dllst).size = 0,                                                      \
    (_cc_dllst).ncnt = CCC_DLLST_START,                                        \
    (_cc_dllst).vcnt = CCC_DLLST_START,                                        \
    (_cc_dllst).xor_offset = (char*)&((_cc_dllst).block.nodes[0].xor) -        \
                             (char*)&((_cc_dllst).block.nodes[0].val),         \
    (_cc_dllst).val_offset = (_cc_dllst).xor_offset * (-1) /                   \
                             sizeof((_cc_dllst).block.nodes[0].val)            \
))



/* dllst iterator create */


#define cc_dllst_iter(elem_t)                                                  \
                                                                               \
    struct                                                                     \
    {                                                                          \
        elem_t *prev;                                                          \
        elem_t *curr;                                                          \
        elem_t *next;                                                          \
                                                                               \
        cc_dllst(elem_t) *pobj;                                                \
    }


#define cc_dllst_iter_init(_cc_dllst_iter, _cc_dllst)                          \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    (_cc_dllst_iter).prev = NULL,                                              \
    (_cc_dllst_iter).curr = NULL,                                              \
    (_cc_dllst_iter).next = NULL,                                              \
                                                                               \
    (_cc_dllst_iter).pobj = (void*)&(_cc_dllst)                                \
))



/* bitwise operations */


#define X2(_cc_addr_a, _cc_addr_b)                                             \
(                                                                              \
    (link_t)((uintptr_t)(link_t)(_cc_addr_a) ^                                 \
             (uintptr_t)(link_t)(_cc_addr_b))                                  \
)


#define X3(_cc_addr_a, _cc_addr_b, _cc_addr_c)                                 \
(                                                                              \
    (link_t)((uintptr_t)(link_t)(_cc_addr_a) ^                                 \
             (uintptr_t)(link_t)(_cc_addr_b) ^                                 \
             (uintptr_t)(link_t)(_cc_addr_c))                                  \
)



/* dllst iterator operations */


#define cc_dllst_iter_dref(_cc_dllst_iter)                                     \
(                                                                              \
    *((_cc_dllst_iter).curr + (_cc_dllst_iter).pobj->val_offset)               \
)


#define cc_dllst_iter_copy(_cc_dllst_iter_dst, _cc_dllst_iter_src)             \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    (_cc_dllst_iter_dst).prev = (_cc_dllst_iter_src).prev,                     \
    (_cc_dllst_iter_dst).curr = (_cc_dllst_iter_src).curr,                     \
    (_cc_dllst_iter_dst).next = (_cc_dllst_iter_src).next                      \
))


#define cc_dllst_iter_head(_cc_dllst_iter, _cc_dllst)                          \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    (_cc_dllst_iter).prev = NULL,                                              \
    (_cc_dllst_iter).curr = (link_t)&((_cc_dllst).head),                       \
    (_cc_dllst_iter).next = (_cc_dllst).head                                   \
))


#define cc_dllst_iter_tail(_cc_dllst_iter, _cc_dllst)                          \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    (_cc_dllst_iter).next = NULL,                                              \
    (_cc_dllst_iter).curr = (link_t)&((_cc_dllst).tail),                       \
    (_cc_dllst_iter).prev = (_cc_dllst).tail                                   \
))


#define cc_dllst_iter_begin(_cc_dllst_iter, _cc_dllst)                         \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    (_cc_dllst_iter).prev = (link_t)&((_cc_dllst).head),                       \
    (_cc_dllst_iter).curr = (_cc_dllst).head,                                  \
    (_cc_dllst_iter).next = X2(&((_cc_dllst).head), *(link_t*)(_cc_dllst).head)\
))


#define cc_dllst_iter_end(_cc_dllst_iter, _cc_dllst)                           \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    (_cc_dllst_iter).next = (link_t)&((_cc_dllst).tail),                       \
    (_cc_dllst_iter).curr = (_cc_dllst).tail,                                  \
    (_cc_dllst_iter).prev = X2(&((_cc_dllst).tail), *(link_t*)(_cc_dllst).tail)\
))


#define cc_dllst_iter_incr(_cc_dllst_iter)                                     \
(                                                                              \
    ((_cc_dllst_iter).next != NULL) ?                                          \
    (                                                                          \
        (_cc_dllst_iter).prev = (_cc_dllst_iter).curr,                         \
        (_cc_dllst_iter).curr = (_cc_dllst_iter).next,                         \
        (_cc_dllst_iter).next =                                                \
                   X2((_cc_dllst_iter).prev, *(link_t*)(_cc_dllst_iter).curr)  \
    ) : (NULL)                                                                 \
)


#define cc_dllst_iter_decr(_cc_dllst_iter)                                     \
(                                                                              \
    ((_cc_dllst_iter).prev != NULL) ?                                          \
    (                                                                          \
        (_cc_dllst_iter).next = (_cc_dllst_iter).curr,                         \
        (_cc_dllst_iter).curr = (_cc_dllst_iter).prev,                         \
        (_cc_dllst_iter).prev =                                                \
                   X2((_cc_dllst_iter).next, *(link_t*)(_cc_dllst_iter).curr)  \
    ) : (NULL)                                                                 \
)


#define cc_dllst_iter_advance(_cc_dllst_iter, _cc_dllst_iter_distance)         \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    int distance = (_cc_dllst_iter_distance);                                  \
                                                                               \
    if (distance > 0)                                                          \
        while (distance-- && cc_dllst_iter_incr((_cc_dllst_iter)));            \
    else if (distance < 0)                                                     \
        while (distance++ && cc_dllst_iter_decr((_cc_dllst_iter)));            \
})



/* dllst traversal */


#define cc_dllst_trav(_cc_dllst, _cc_dllst_iter)                               \
                                                                               \
    cc_dllst_iter_head((_cc_dllst_iter), (_cc_dllst));                         \
                                                                               \
    while (cc_dllst_iter_incr((_cc_dllst_iter)))


#define cc_dllst_trav_back(_cc_dllst, _cc_dllst_iter)                          \
                                                                               \
    cc_dllst_iter_tail((_cc_dllst_iter), (_cc_dllst));                         \
                                                                               \
    while (cc_dllst_iter_decr((_cc_dllst_iter)))



/* dllst access */


#define cc_dllst_front(_cc_dllst)                                              \
(                                                                              \
    *((_cc_dllst).head + (_cc_dllst).val_offset)                               \
)


#define cc_dllst_back(_cc_dllst)                                               \
(                                                                              \
    *((_cc_dllst).tail + (_cc_dllst).val_offset)                               \
)



/* dllst capacity */


#define cc_dllst_size(_cc_dllst)                                               \
(                                                                              \
    (_cc_dllst).size                                                           \
)


#define cc_dllst_empty(_cc_dllst)                                              \
(                                                                              \
    cc_dllst_size((_cc_dllst)) == 0                                            \
)



/* dllst node operations */


#define _cc_dllst_node_alloc(_cc_dllst_pnode, _cc_dllst)                       \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    if ((_cc_dllst).avsp == NULL)                                              \
    {                                                                          \
        if ((_cc_dllst).vcnt == 0)                                             \
        {                                                                      \
            link_t pool_dup = (_cc_dllst).pool;                                \
                                                                               \
            if ((_cc_dllst).ncnt < CCC_DLLST_LIMIT)                            \
                (_cc_dllst).vcnt = ((_cc_dllst).ncnt *= CCC_DLLST_RATIO);      \
            else                                                               \
                (_cc_dllst).vcnt = ((_cc_dllst).ncnt =  CCC_DLLST_LIMIT);      \
                                                                               \
            (_cc_dllst).pool = malloc(sizeof((_cc_dllst).block) +              \
                                      sizeof((_cc_dllst).block.nodes[0]) *     \
                                      ((_cc_dllst).ncnt - CCC_DLLST_START));   \
                                                                               \
            *(link_t*)(_cc_dllst).pool = pool_dup;                             \
        }                                                                      \
                                                                               \
        (_cc_dllst_pnode) = &((_cc_dllst).pool->nodes[--(_cc_dllst).vcnt]);    \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        (_cc_dllst_pnode) = (void*)((_cc_dllst).avsp + (_cc_dllst).val_offset);\
        (_cc_dllst).avsp = *(link_t*)(_cc_dllst).avsp;                         \
    }                                                                          \
})


#define _cc_dllst_node_clear(_cc_dllst_node_pxor, _cc_dllst)                   \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    *(link_t*)(_cc_dllst_node_pxor) = (_cc_dllst).avsp;                        \
    (_cc_dllst).avsp = (_cc_dllst_node_pxor);                                  \
})


#define _cc_dllst_blocks_free(_cc_dllst)                                       \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    while ((_cc_dllst).pool != &((_cc_dllst).block))                           \
    {                                                                          \
        link_t pool_dup = (_cc_dllst).pool;                                    \
        (_cc_dllst).pool = *(link_t*)(_cc_dllst).pool;                         \
        free(pool_dup);                                                        \
    }                                                                          \
})



/* dllst modifiers */


#define cc_dllst_push_front(_cc_dllst, _cc_dllst_push_front_value)             \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    _cc_dllst_node_alloc((_cc_dllst).block.pnode, (_cc_dllst));                \
                                                                               \
    (_cc_dllst).block.pnode->val = (_cc_dllst_push_front_value);               \
    (_cc_dllst).block.pnode->xor = X2(&((_cc_dllst).head), (_cc_dllst).head);  \
                                                                               \
    *(link_t*)                                                                 \
    (_cc_dllst).head = X3(&((_cc_dllst).head),                                 \
                          *(link_t*)(_cc_dllst).head,                          \
                          &((_cc_dllst).block.pnode->xor));                    \
    (_cc_dllst).head = (link_t)&((_cc_dllst).block.pnode->xor);                \
                                                                               \
    (_cc_dllst).size++;                                                        \
})


#define cc_dllst_push_back(_cc_dllst, _cc_dllst_push_back_value)               \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    _cc_dllst_node_alloc((_cc_dllst).block.pnode, (_cc_dllst));                \
                                                                               \
    (_cc_dllst).block.pnode->val = (_cc_dllst_push_back_value);                \
    (_cc_dllst).block.pnode->xor = X2(&((_cc_dllst).tail), (_cc_dllst).tail);  \
                                                                               \
    *(link_t*)                                                                 \
    (_cc_dllst).tail = X3(&((_cc_dllst).tail),                                 \
                          *(link_t*)(_cc_dllst).tail,                          \
                          &((_cc_dllst).block.pnode->xor));                    \
    (_cc_dllst).tail = (link_t)&((_cc_dllst).block.pnode->xor);                \
                                                                               \
    (_cc_dllst).size++;                                                        \
})


#define cc_dllst_pop_front(_cc_dllst)                                          \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    if (!(cc_dllst_empty((_cc_dllst))))                                        \
    {                                                                          \
        link_t head  =   (_cc_dllst).head;                                     \
        link_t phead = &((_cc_dllst).head);                                    \
                                                                               \
        (_cc_dllst).head = X2(phead, *(link_t*)(_cc_dllst).head);              \
        *(link_t*)                                                             \
        (_cc_dllst).head = X3(phead, *(link_t*)(_cc_dllst).head, head);        \
                                                                               \
        _cc_dllst_node_clear(head, (_cc_dllst));                               \
                                                                               \
        (_cc_dllst).size--;                                                    \
    }                                                                          \
})


#define cc_dllst_pop_back(_cc_dllst)                                           \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    if (!(cc_dllst_empty((_cc_dllst))))                                        \
    {                                                                          \
        link_t tail  =   (_cc_dllst).tail;                                     \
        link_t ptail = &((_cc_dllst).tail);                                    \
                                                                               \
        (_cc_dllst).tail = X2(ptail, *(link_t*)(_cc_dllst).tail);              \
        *(link_t*)                                                             \
        (_cc_dllst).tail = X3(ptail, *(link_t*)(_cc_dllst).tail, tail);        \
                                                                               \
        _cc_dllst_node_clear(tail, (_cc_dllst));                               \
                                                                               \
        (_cc_dllst).size--;                                                    \
    }                                                                          \
})


#define cc_dllst_insert(_cc_dllst_iter, _cc_dllst_insert_value)                \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    if ((_cc_dllst_iter).prev != NULL)                                         \
    {                                                                          \
        link_t node_pxor;                                                      \
                                                                               \
        _cc_dllst_node_alloc((*(_cc_dllst_iter).pobj).block.pnode,             \
                              *(_cc_dllst_iter).pobj);                         \
                                                                               \
        (_cc_dllst_iter).pobj->block.pnode->val = (_cc_dllst_insert_value);    \
                                                                               \
        (_cc_dllst_iter).next = (_cc_dllst_iter).curr;                         \
        (_cc_dllst_iter).curr = node_pxor                                      \
                              = &((_cc_dllst_iter).pobj->block.pnode->xor);    \
                                                                               \
        *(link_t*)                                                             \
        (_cc_dllst_iter).curr = X2((_cc_dllst_iter).prev,                      \
                                   (_cc_dllst_iter).next);                     \
        *(link_t*)                                                             \
        (_cc_dllst_iter).prev = X3(*(link_t*)(_cc_dllst_iter).prev,            \
                                   (_cc_dllst_iter).next, node_pxor);          \
        *(link_t*)                                                             \
        (_cc_dllst_iter).next = X3(*(link_t*)(_cc_dllst_iter).next,            \
                                   (_cc_dllst_iter).prev, node_pxor);          \
                                                                               \
        (_cc_dllst_iter).pobj->size++;                                         \
    }                                                                          \
})


#define cc_dllst_erase(_cc_dllst_iter)                                         \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    if ((_cc_dllst_iter).prev != NULL && (_cc_dllst_iter).next != NULL)        \
    {                                                                          \
        link_t node_pxor = (_cc_dllst_iter).curr;                              \
        (_cc_dllst_iter).curr = (_cc_dllst_iter).next;                         \
                                                                               \
        *(link_t*)                                                             \
        (_cc_dllst_iter).prev = X3(*(link_t*)(_cc_dllst_iter).prev,            \
                                   (_cc_dllst_iter).next, node_pxor);          \
        *(link_t*)                                                             \
        (_cc_dllst_iter).next = X3(*(link_t*)(_cc_dllst_iter).next,            \
                                   (_cc_dllst_iter).prev, node_pxor);          \
                                                                               \
        (_cc_dllst_iter).next = X2(*(link_t*)(_cc_dllst_iter).curr,            \
                                   (_cc_dllst_iter).prev);                     \
                                                                               \
        _cc_dllst_node_clear(node_pxor, *(_cc_dllst_iter).pobj);               \
                                                                               \
        (_cc_dllst_iter).pobj->size--;                                         \
    }                                                                          \
})


#define cc_dllst_swap(_cc_dllst_a, _cc_dllst_b)                                \
    /* TODO */


#define cc_dllst_resize(_cc_dllst, _cc_dllst_padding_value)                    \
    /* TODO */


#define cc_dllst_clear(_cc_dllst)                                              \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    do cc_dllst_pop_back((_cc_dllst));                                         \
    while (!(cc_dllst_empty((_cc_dllst))));                                    \
})



/* dllst operations */


#define cc_dllst_splice_range(_cc_dllst_iter_p,                                \
                              _cc_dllst_iter_f, _cc_dllst_iter_l)              \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    link_t p_prev = (_cc_dllst_iter_p).prev;                                   \
    link_t p_curr = (_cc_dllst_iter_p).curr;                                   \
    link_t f_prev = (_cc_dllst_iter_f).prev;                                   \
    link_t f_curr = (_cc_dllst_iter_f).curr;                                   \
    link_t l_prev = (_cc_dllst_iter_l).prev;                                   \
    link_t l_curr = (_cc_dllst_iter_l).curr;                                   \
                                                                               \
    link_t *pp_next = (link_t*)&((_cc_dllst_iter_p).next);                     \
    link_t *pf_next = (link_t*)&((_cc_dllst_iter_f).next);                     \
    link_t *pl_next = (link_t*)&((_cc_dllst_iter_l).next);                     \
                                                                               \
    *(link_t*)p_prev = X3(p_curr, f_curr, *(link_t*)p_prev);                   \
    *(link_t*)p_curr = X3(p_prev, l_prev, *(link_t*)p_curr);                   \
    *(link_t*)f_prev = X3(f_curr, l_curr, *(link_t*)f_prev);                   \
    *(link_t*)f_curr = X3(f_prev, p_prev, *(link_t*)f_curr);                   \
    *(link_t*)l_prev = X3(l_curr, p_curr, *(link_t*)l_prev);                   \
    *(link_t*)l_curr = X3(l_prev, f_prev, *(link_t*)l_curr);                   \
                                                                               \
    if (*pp_next == f_curr)                                                    \
        *pp_next =  l_curr;                                                    \
    if (*pf_next == l_curr)                                                    \
        *pf_next =  p_curr;                                                    \
    if (*pl_next == p_curr)                                                    \
        *pl_next =  f_curr;                                                    \
                                                                               \
    (_cc_dllst_iter_p).prev = X2(*pp_next, *(link_t*)p_curr);                  \
    (_cc_dllst_iter_f).prev = X2(*pf_next, *(link_t*)f_curr);                  \
    (_cc_dllst_iter_l).prev = X2(*pl_next, *(link_t*)l_curr);                  \
})


#define cc_dllst_sort(_cc_dllst)                                               \
    /* TODO */



/* dllst destroy */


#define cc_dllst_free(_cc_dllst)                                               \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    _cc_dllst_blocks_free((_cc_dllst));                                        \
                                                                               \
    cc_dllst_init((_cc_dllst));                                                \
})



#endif
