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
    #define CCC_DLLST_LIMIT 65536

#endif



/* dllst create */


typedef void* _link_t;

#define cc_dllst(_elem_t)                                                      \
                                                                               \
    struct                                                                     \
    {                                                                          \
        _elem_t *head;                                                         \
        _elem_t *tail;                                                         \
        _elem_t *avsp;                                                         \
                                                                               \
        struct                                                                 \
        {                                                                      \
            _link_t next;                                                      \
                                                                               \
            struct                                                             \
            {                                                                  \
                _elem_t val;                                                   \
                _link_t xorl;                                                  \
            }   nodes[CCC_DLLST_START], *pnode;                                \
                                                                               \
        }   block, *pool;                                                      \
                                                                               \
        size_t    size, ncnt, vcnt;                                            \
        ptrdiff_t val_offset, link_offset;                                     \
    }


#define cc_dllst_init(_cc_dllst)                                               \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    (_cc_dllst).head = (_link_t)&((_cc_dllst).tail),                           \
    (_cc_dllst).tail = (_link_t)&((_cc_dllst).head),                           \
    (_cc_dllst).avsp = NULL,                                                   \
    (_cc_dllst).pool = &((_cc_dllst).block),                                   \
                                                                               \
    (_cc_dllst).size = 0,                                                      \
    (_cc_dllst).ncnt = CCC_DLLST_START,                                        \
    (_cc_dllst).vcnt = CCC_DLLST_START,                                        \
    (_cc_dllst).link_offset = (char*)&((_cc_dllst).block.nodes[0].xorl) -      \
                              (char*)&((_cc_dllst).block.nodes[0].val),        \
    (_cc_dllst).val_offset  = (_cc_dllst).link_offset * (-1) /                 \
                              sizeof((_cc_dllst).block.nodes[0].val)           \
))



/* dllst iterator create */


#define cc_dllst_iter(_elem_t)                                                 \
                                                                               \
    struct                                                                     \
    {                                                                          \
        _elem_t *prev;                                                         \
        _elem_t *curr;                                                         \
        _elem_t *next;                                                         \
                                                                               \
        cc_dllst(_elem_t) *pobj;                                               \
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


#define _cc_xor_2(_cc_addr_a, _cc_addr_b)                                      \
(                                                                              \
    (_link_t)((uintptr_t)(_link_t)(_cc_addr_a) ^                               \
              (uintptr_t)(_link_t)(_cc_addr_b))                                \
)


#define _cc_xor_3(_cc_addr_a, _cc_addr_b, _cc_addr_c)                          \
(                                                                              \
    (_link_t)((uintptr_t)(_link_t)(_cc_addr_a) ^                               \
              (uintptr_t)(_link_t)(_cc_addr_b) ^                               \
              (uintptr_t)(_link_t)(_cc_addr_c))                                \
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
    (_cc_dllst_iter).curr = (_link_t)&((_cc_dllst).head),                      \
    (_cc_dllst_iter).next = (_cc_dllst).head                                   \
))


#define cc_dllst_iter_tail(_cc_dllst_iter, _cc_dllst)                          \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    (_cc_dllst_iter).next = NULL,                                              \
    (_cc_dllst_iter).curr = (_link_t)&((_cc_dllst).tail),                      \
    (_cc_dllst_iter).prev = (_cc_dllst).tail                                   \
))


#define cc_dllst_iter_begin(_cc_dllst_iter, _cc_dllst)                         \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    (_cc_dllst_iter).prev = (_link_t)&((_cc_dllst).head),                      \
    (_cc_dllst_iter).curr = (_cc_dllst).head,                                  \
    (_cc_dllst_iter).next = _cc_xor_2(&((_cc_dllst).head),                     \
                                      *(_link_t*)(_cc_dllst).head)             \
))


#define cc_dllst_iter_end(_cc_dllst_iter, _cc_dllst)                           \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    (_cc_dllst_iter).next = (_link_t)&((_cc_dllst).tail),                      \
    (_cc_dllst_iter).curr = (_cc_dllst).tail,                                  \
    (_cc_dllst_iter).prev = _cc_xor_2(&((_cc_dllst).tail),                     \
                                      *(_link_t*)(_cc_dllst).tail)             \
))


#define cc_dllst_iter_incr(_cc_dllst_iter)                                     \
(                                                                              \
    ((_cc_dllst_iter).next != NULL) ?                                          \
    (                                                                          \
        (_cc_dllst_iter).prev = (_cc_dllst_iter).curr,                         \
        (_cc_dllst_iter).curr = (_cc_dllst_iter).next,                         \
        (_cc_dllst_iter).next = _cc_xor_2((_cc_dllst_iter).prev,               \
                                          *(_link_t*)(_cc_dllst_iter).curr)    \
    ) : (void*)NULL                                                            \
)


#define cc_dllst_iter_decr(_cc_dllst_iter)                                     \
(                                                                              \
    ((_cc_dllst_iter).prev != NULL) ?                                          \
    (                                                                          \
        (_cc_dllst_iter).next = (_cc_dllst_iter).curr,                         \
        (_cc_dllst_iter).curr = (_cc_dllst_iter).prev,                         \
        (_cc_dllst_iter).prev = _cc_xor_2((_cc_dllst_iter).next,               \
                                          *(_link_t*)(_cc_dllst_iter).curr)    \
    ) : (void*)NULL                                                            \
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
            _link_t pool_dup = (_cc_dllst).pool;                               \
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
            *(_link_t*)(_cc_dllst).pool = pool_dup;                            \
        }                                                                      \
                                                                               \
        (_cc_dllst_pnode) = &((_cc_dllst).pool->nodes[--(_cc_dllst).vcnt]);    \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        (_cc_dllst_pnode) = (void*)((_cc_dllst).avsp + (_cc_dllst).val_offset);\
        (_cc_dllst).avsp = *(_link_t*)(_cc_dllst).avsp;                        \
    }                                                                          \
})


#define _cc_dllst_node_clear(_cc_dllst_node_pxorl, _cc_dllst)                  \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    *(_link_t*)(_cc_dllst_node_pxorl) = (_cc_dllst).avsp;                      \
    (_cc_dllst).avsp = (_cc_dllst_node_pxorl);                                 \
})


#define _cc_dllst_blocks_free(_cc_dllst)                                       \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    while ((_cc_dllst).pool != &((_cc_dllst).block))                           \
    {                                                                          \
        _link_t pool_dup = (_cc_dllst).pool;                                   \
        (_cc_dllst).pool = *(_link_t*)(_cc_dllst).pool;                        \
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
    (_cc_dllst).block.pnode->val  = (_cc_dllst_push_front_value);              \
    (_cc_dllst).block.pnode->xorl = _cc_xor_2(&((_cc_dllst).head),             \
                                                (_cc_dllst).head);             \
                                                                               \
    *(_link_t*)                                                                \
    (_cc_dllst).head = _cc_xor_3(&((_cc_dllst).head),                          \
                                 &((_cc_dllst).block.pnode->xorl),             \
                                 *(_link_t*)(_cc_dllst).head);                 \
    (_cc_dllst).head = (_link_t)&((_cc_dllst).block.pnode->xorl);              \
                                                                               \
    (_cc_dllst).size++;                                                        \
})


#define cc_dllst_push_back(_cc_dllst, _cc_dllst_push_back_value)               \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    _cc_dllst_node_alloc((_cc_dllst).block.pnode, (_cc_dllst));                \
                                                                               \
    (_cc_dllst).block.pnode->val  = (_cc_dllst_push_back_value);               \
    (_cc_dllst).block.pnode->xorl = _cc_xor_2(&((_cc_dllst).tail),             \
                                                (_cc_dllst).tail);             \
                                                                               \
    *(_link_t*)                                                                \
    (_cc_dllst).tail = _cc_xor_3(&((_cc_dllst).tail),                          \
                                 &((_cc_dllst).block.pnode->xorl),             \
                                 *(_link_t*)(_cc_dllst).tail);                 \
    (_cc_dllst).tail = (_link_t)&((_cc_dllst).block.pnode->xorl);              \
                                                                               \
    (_cc_dllst).size++;                                                        \
})


#define cc_dllst_pop_front(_cc_dllst)                                          \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    if (!(cc_dllst_empty((_cc_dllst))))                                        \
    {                                                                          \
        _link_t head = (_cc_dllst).head;                                       \
                                                                               \
        (_cc_dllst).head = _cc_xor_2(&((_cc_dllst).head),                      \
                                     *(_link_t*)(_cc_dllst).head);             \
        *(_link_t*)                                                            \
        (_cc_dllst).head = _cc_xor_3(&((_cc_dllst).head), head,                \
                                     *(_link_t*)(_cc_dllst).head);             \
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
        _link_t tail = (_cc_dllst).tail;                                       \
                                                                               \
        (_cc_dllst).tail = _cc_xor_2(&((_cc_dllst).tail),                      \
                                     *(_link_t*)(_cc_dllst).tail);             \
        *(_link_t*)                                                            \
        (_cc_dllst).tail = _cc_xor_3(&((_cc_dllst).tail), tail,                \
                                     *(_link_t*)(_cc_dllst).tail);             \
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
        _link_t node_pxorl;                                                    \
                                                                               \
        _cc_dllst_node_alloc((*(_cc_dllst_iter).pobj).block.pnode,             \
                              *(_cc_dllst_iter).pobj);                         \
                                                                               \
        (_cc_dllst_iter).pobj->block.pnode->val = (_cc_dllst_insert_value);    \
                                                                               \
        (_cc_dllst_iter).next = (_cc_dllst_iter).curr;                         \
        (_cc_dllst_iter).curr = node_pxorl                                     \
                              = &((_cc_dllst_iter).pobj->block.pnode->xorl);   \
                                                                               \
        *(_link_t*)                                                            \
        (_cc_dllst_iter).curr = _cc_xor_2((_cc_dllst_iter).prev,               \
                                          (_cc_dllst_iter).next);              \
        *(_link_t*)                                                            \
        (_cc_dllst_iter).prev = _cc_xor_3(*(_link_t*)(_cc_dllst_iter).prev,    \
                                          (_cc_dllst_iter).next, node_pxorl);  \
        *(_link_t*)                                                            \
        (_cc_dllst_iter).next = _cc_xor_3(*(_link_t*)(_cc_dllst_iter).next,    \
                                          (_cc_dllst_iter).prev, node_pxorl);  \
                                                                               \
        (_cc_dllst_iter).pobj->size++;                                         \
    }                                                                          \
})


#define cc_dllst_insert_range(_cc_dllst_iter_position,                         \
                              _cc_dllst_iter_fisrt, _cc_dllst_iter_last)       \
    /* TODO */


#define cc_dllst_erase(_cc_dllst_iter)                                         \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    if ((_cc_dllst_iter).prev != NULL && (_cc_dllst_iter).next != NULL)        \
    {                                                                          \
        _link_t node_pxorl = (_cc_dllst_iter).curr;                            \
                                                                               \
        *(_link_t*)                                                            \
        (_cc_dllst_iter).prev = _cc_xor_3(*(_link_t*)(_cc_dllst_iter).prev,    \
                                          (_cc_dllst_iter).next, node_pxorl);  \
        *(_link_t*)                                                            \
        (_cc_dllst_iter).next = _cc_xor_3(*(_link_t*)(_cc_dllst_iter).next,    \
                                          (_cc_dllst_iter).prev, node_pxorl);  \
                                                                               \
        (_cc_dllst_iter).curr = (_cc_dllst_iter).next;                         \
        (_cc_dllst_iter).next = _cc_xor_2((_cc_dllst_iter).prev,               \
                                          *(_link_t*)(_cc_dllst_iter).curr);   \
                                                                               \
        _cc_dllst_node_clear(node_pxorl, *(_cc_dllst_iter).pobj);              \
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
