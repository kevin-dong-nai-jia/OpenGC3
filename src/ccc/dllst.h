#ifndef _CCC_DLLST_H_
#define _CCC_DLLST_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>



/* dllst container node */


#define _cc_dllst_node(_cc_dllst)                                              \
                                                                               \
    struct                                                                     \
    {                                                                          \
        _cc_dllst##_element_t val;                                             \
        void *link;                                                            \
    }



/* dllst container block */


#if (CCC_DLLST_BLOCK_SIZE - 0 <= 0)
    #undef  CCC_DLLST_BLOCK_SIZE
    #define CCC_DLLST_BLOCK_SIZE 16
#endif


#define _cc_dllst_block(_cc_dllst)                                             \
                                                                               \
    struct                                                                     \
    {                                                                          \
        void *next;                                                            \
        _cc_dllst##_node_t arr[CCC_DLLST_BLOCK_SIZE];                          \
    }



/* dllst container constructor */


#define _cc_dllst_struct(_cc_dllst)                                            \
                                                                               \
    struct                                                                     \
    {                                                                          \
        void *pool;                                                            \
        _cc_dllst##_element_t *avsp, *head, *tail;                             \
                                                                               \
        int size, vcnt;                                                        \
        size_t node_size, block_size;                                          \
        ptrdiff_t val_offset, link_offset, arr_offset;                         \
    }


#define _cc_dllst_struct_init {NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0}


#define cc_dllst(_cc_dllst, _cc_dllst_element_t)                               \
                                                                               \
    typedef _cc_dllst_element_t         _cc_dllst##_element_t;                 \
    typedef _cc_dllst_node  (_cc_dllst) _cc_dllst##_node_t;                    \
    typedef _cc_dllst_block (_cc_dllst) _cc_dllst##_block_t;                   \
    typedef _cc_dllst_struct(_cc_dllst) _cc_dllst##_t;                         \
                                                                               \
    _cc_dllst##_t       _cc_dllst = _cc_dllst_struct_init;                     \
    _cc_dllst##_node_t  _cc_dllst##_node;                                      \
    _cc_dllst##_block_t _cc_dllst##_block;                                     \
                                                                               \
    _cc_dllst.head = (void*)&(_cc_dllst.tail);                                 \
    _cc_dllst.tail = (void*)&(_cc_dllst.head);                                 \
                                                                               \
    _cc_dllst.node_size  = sizeof(_cc_dllst##_node);                           \
    _cc_dllst.block_size = sizeof(_cc_dllst##_block);                          \
                                                                               \
    _cc_dllst.val_offset  = &(_cc_dllst##_node.val) -                          \
                            (_cc_dllst##_element_t*)&(_cc_dllst##_node.link);  \
    _cc_dllst.link_offset = (char*)&(_cc_dllst##_node.link) -                  \
                            (char*)&(_cc_dllst##_node.val);                    \
    _cc_dllst.arr_offset  = (char*)&(_cc_dllst##_block.arr) -                  \
                            (char*)&(_cc_dllst##_block.next);


#define cc_dllst_packed(_cc_dllst, _cc_dllst_element_t)                        \
                                                                               \
    _Pragma("pack(push,1)")                                                    \
                                                                               \
    cc_dllst(_cc_dllst, _cc_dllst_element_t);                                  \
                                                                               \
    _Pragma("pack(pop)")



/* bitwise operations */


#define _cc_xor_2_addrs(_cc_addr_a, _cc_addr_b)                                \
(                                                                              \
    (void*)((uintptr_t)(void*)(_cc_addr_a) ^                                   \
            (uintptr_t)(void*)(_cc_addr_b))                                    \
)


#define _cc_xor_3_addrs(_cc_addr_a, _cc_addr_b, _cc_addr_c)                    \
(                                                                              \
    (void*)((uintptr_t)(void*)(_cc_addr_a) ^                                   \
            (uintptr_t)(void*)(_cc_addr_b) ^                                   \
            (uintptr_t)(void*)(_cc_addr_c))                                    \
)



/* dllst container iterators */


#define _cc_dllst_iter_struct(_cc_dllst_iter)                                  \
                                                                               \
    struct                                                                     \
    {                                                                          \
        _cc_dllst_iter##_object_t  *pobj;                                      \
        _cc_dllst_iter##_element_t *prev, *curr, *next;                        \
    }


#define _cc_dllst_iter_struct_init {NULL, NULL, NULL, NULL}


#define cc_dllst_iter(_cc_dllst_iter, _cc_dllst)                               \
                                                                               \
    typedef _cc_dllst##_t                         _cc_dllst_iter##_object_t;   \
    typedef _cc_dllst##_element_t                 _cc_dllst_iter##_element_t;  \
    typedef _cc_dllst_iter_struct(_cc_dllst_iter) _cc_dllst_iter##_t;          \
                                                                               \
    _cc_dllst_iter##_t _cc_dllst_iter = _cc_dllst_iter_struct_init;            \
                                                                               \
    _cc_dllst_iter.pobj = &(_cc_dllst);


#define cc_dllst_iter_deref(_cc_dllst_iter)                                    \
(                                                                              \
    *(_cc_dllst_iter.curr + _cc_dllst_iter.pobj->val_offset)                   \
)


#define cc_dllst_iter_copy(_cc_dllst_iter_dst, _cc_dllst_iter_src)             \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    _cc_dllst_iter_dst.prev = _cc_dllst_iter_src.prev,                         \
    _cc_dllst_iter_dst.curr = _cc_dllst_iter_src.curr,                         \
    _cc_dllst_iter_dst.next = _cc_dllst_iter_src.next                          \
))


#define cc_dllst_iter_head(_cc_dllst_iter, _cc_dllst)                          \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    _cc_dllst_iter.prev = NULL,                                                \
    _cc_dllst_iter.curr = (void*)&(_cc_dllst.head),                            \
    _cc_dllst_iter.next = _cc_dllst.head                                       \
))


#define cc_dllst_iter_tail(_cc_dllst_iter, _cc_dllst)                          \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    _cc_dllst_iter.next = NULL,                                                \
    _cc_dllst_iter.curr = (void*)&(_cc_dllst.tail),                            \
    _cc_dllst_iter.prev = _cc_dllst.tail                                       \
))


#define cc_dllst_iter_begin(_cc_dllst_iter, _cc_dllst)                         \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    _cc_dllst_iter.prev = (void*)&(_cc_dllst.head),                            \
    _cc_dllst_iter.curr = _cc_dllst.head,                                      \
    _cc_dllst_iter.next = _cc_xor_2_addrs(&(_cc_dllst.head),                   \
                                          *(void**)_cc_dllst.head)             \
))


#define cc_dllst_iter_end(_cc_dllst_iter, _cc_dllst)                           \
                                                                               \
CCC_VOID_EXPR_                                                                 \
((                                                                             \
    _cc_dllst_iter.next = (void*)&(_cc_dllst.tail),                            \
    _cc_dllst_iter.curr = _cc_dllst.tail,                                      \
    _cc_dllst_iter.prev = _cc_xor_2_addrs(&(_cc_dllst.tail),                   \
                                          *(void**)_cc_dllst.tail)             \
))


#define cc_dllst_iter_incr(_cc_dllst_iter)                                     \
(                                                                              \
    (_cc_dllst_iter.next == NULL) ? (NULL) :                                   \
    (                                                                          \
        _cc_dllst_iter.prev = _cc_dllst_iter.curr,                             \
        _cc_dllst_iter.curr = _cc_dllst_iter.next,                             \
        _cc_dllst_iter.next = _cc_xor_2_addrs(_cc_dllst_iter.prev,             \
                                              *(void**)_cc_dllst_iter.curr)    \
    )                                                                          \
)


#define cc_dllst_iter_decr(_cc_dllst_iter)                                     \
(                                                                              \
    (_cc_dllst_iter.prev == NULL) ? (NULL) :                                   \
    (                                                                          \
        _cc_dllst_iter.next = _cc_dllst_iter.curr,                             \
        _cc_dllst_iter.curr = _cc_dllst_iter.prev,                             \
        _cc_dllst_iter.prev = _cc_xor_2_addrs(_cc_dllst_iter.next,             \
                                              *(void**)_cc_dllst_iter.curr)    \
    )                                                                          \
)



/* dllst container traversal */


#define cc_dllst_trav(_cc_dllst, _cc_dllst_iter)                               \
                                                                               \
    cc_dllst_iter_head(_cc_dllst_iter, _cc_dllst);                             \
                                                                               \
    while (cc_dllst_iter_incr(_cc_dllst_iter))


#define cc_dllst_trav_backward(_cc_dllst, _cc_dllst_iter)                      \
                                                                               \
    cc_dllst_iter_tail(_cc_dllst_iter, _cc_dllst);                             \
                                                                               \
    while (cc_dllst_iter_decr(_cc_dllst_iter))



/* dllst container capacity */


#define cc_dllst_size(_cc_dllst)                                               \
(                                                                              \
    _cc_dllst.size                                                             \
)


#define cc_dllst_empty(_cc_dllst)                                              \
(                                                                              \
    cc_dllst_size(_cc_dllst) == 0                                              \
)



/* dllst container element access */


#define cc_dllst_front(_cc_dllst)                                              \
(                                                                              \
    *(_cc_dllst.head + _cc_dllst.val_offset)                                   \
)


#define cc_dllst_back(_cc_dllst)                                               \
(                                                                              \
    *(_cc_dllst.tail + _cc_dllst.val_offset)                                   \
)



/* dllst container node operations */


#define _cc_dllst_node_alloc(_cc_dllst_node_ptr, _cc_dllst)                    \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    if (_cc_dllst.avsp == NULL)                                                \
    {                                                                          \
        if (_cc_dllst.vcnt == 0)                                               \
        {                                                                      \
            void *pool_dup = _cc_dllst.pool;                                   \
            _cc_dllst.pool = malloc(_cc_dllst.block_size);                     \
                                                                               \
            *(void**)_cc_dllst.pool = pool_dup;                                \
            _cc_dllst.vcnt = CCC_DLLST_BLOCK_SIZE;                             \
        }                                                                      \
                                                                               \
        _cc_dllst_node_ptr = ((char*)_cc_dllst.pool + _cc_dllst.arr_offset +   \
                              _cc_dllst.node_size * --_cc_dllst.vcnt);         \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        _cc_dllst_node_ptr = _cc_dllst.avsp + _cc_dllst.val_offset;            \
                                                                               \
        _cc_dllst.avsp = *(void**)_cc_dllst.avsp;                              \
    }                                                                          \
})


#define _cc_dllst_node_clear(_cc_dllst_node_link_ptr, _cc_dllst)               \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    *(void**)_cc_dllst_node_link_ptr = _cc_dllst.avsp;                         \
    _cc_dllst.avsp = _cc_dllst_node_link_ptr;                                  \
})


#define _cc_dllst_all_nodes_free(_cc_dllst)                                    \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    while (_cc_dllst.pool != NULL)                                             \
    {                                                                          \
        void *pool_dup = _cc_dllst.pool;                                       \
        _cc_dllst.pool = *(void**)_cc_dllst.pool;                              \
        free(pool_dup);                                                        \
    }                                                                          \
})



/* dllst container modifiers */


#define cc_dllst_push_front(_cc_dllst, _cc_dllst_push_front_value)             \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    void *node; void **link;                                                   \
    _cc_dllst##_element_t *val;                                                \
                                                                               \
    _cc_dllst_node_alloc(node, _cc_dllst);                                     \
                                                                               \
     val = node;                                                               \
    *val = _cc_dllst_push_front_value;                                         \
                                                                               \
     link = (void*)((char*)node + _cc_dllst.link_offset);                      \
    *link = _cc_xor_2_addrs(&(_cc_dllst.head), _cc_dllst.head);                \
                                                                               \
    *(void**)_cc_dllst.head = _cc_xor_3_addrs(&(_cc_dllst.head), link,         \
                                              *(void**)_cc_dllst.head);        \
                                                                               \
    _cc_dllst.head = (void*)link;                                              \
                                                                               \
    _cc_dllst.size++;                                                          \
})


#define cc_dllst_push_back(_cc_dllst, _cc_dllst_push_back_value)               \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    void *node; void **link;                                                   \
    _cc_dllst##_element_t *val;                                                \
                                                                               \
    _cc_dllst_node_alloc(node, _cc_dllst);                                     \
                                                                               \
     val = node;                                                               \
    *val = _cc_dllst_push_back_value;                                          \
                                                                               \
     link = (void*)((char*)node + _cc_dllst.link_offset);                      \
    *link = _cc_xor_2_addrs(&(_cc_dllst.tail), _cc_dllst.tail);                \
                                                                               \
    *(void**)_cc_dllst.tail = _cc_xor_3_addrs(&(_cc_dllst.tail), link,         \
                                              *(void**)_cc_dllst.tail);        \
                                                                               \
    _cc_dllst.tail = (void*)link;                                              \
                                                                               \
    _cc_dllst.size++;                                                          \
})


#define cc_dllst_pop_front(_cc_dllst)                                          \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    if (!(cc_dllst_empty(_cc_dllst)))                                          \
    {                                                                          \
        void *head = _cc_dllst.head;                                           \
                                                                               \
        _cc_dllst.head = _cc_xor_2_addrs(&(_cc_dllst.head),                    \
                                         *(void**)_cc_dllst.head);             \
                                                                               \
        *(void**)_cc_dllst.head = _cc_xor_3_addrs(&(_cc_dllst.head), head,     \
                                                  *(void**)_cc_dllst.head);    \
                                                                               \
        _cc_dllst_node_clear(head, _cc_dllst);                                 \
                                                                               \
        _cc_dllst.size--;                                                      \
    }                                                                          \
})


#define cc_dllst_pop_back(_cc_dllst)                                           \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    if (!(cc_dllst_empty(_cc_dllst)))                                          \
    {                                                                          \
        void *tail = _cc_dllst.tail;                                           \
                                                                               \
        _cc_dllst.tail = _cc_xor_2_addrs(&(_cc_dllst.tail),                    \
                                         *(void**)_cc_dllst.tail);             \
                                                                               \
        *(void**)_cc_dllst.tail = _cc_xor_3_addrs(&(_cc_dllst.tail), tail,     \
                                                  *(void**)_cc_dllst.tail);    \
                                                                               \
        _cc_dllst_node_clear(tail, _cc_dllst);                                 \
                                                                               \
        _cc_dllst.size--;                                                      \
    }                                                                          \
})


#define cc_dllst_insert(_cc_dllst_iter, _cc_dllst_insert_value)                \
    /* TODO */


#define cc_dllst_erase(_cc_dllst_iter)                                         \
    /* TODO */


#define cc_dllst_swap(_cc_dllst_a, _cc_dllst_b)                                \
    /* TODO */


#define cc_dllst_resize(_cc_dllst, _cc_dllst_padding_value)                    \
    /* TODO */


#define cc_dllst_clear(_cc_dllst)                                              \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    do cc_dllst_pop_back(_cc_dllst);                                           \
    while (!(cc_dllst_empty(_cc_dllst)));                                      \
})



/* dllst container operations */


#define cc_dllst_transfer_range(_cc_dllst, _cc_dllst_iter_position,            \
                                _cc_dllst_iter_fisrt, _cc_dllst_iter_last)     \
    /* TODO */


#define cc_dllst_sort(_cc_dllst)                                               \
    /* TODO */



/* dllst container destructor */


#define cc_dllst_free(_cc_dllst)                                               \
                                                                               \
CCC_STATEMENT_                                                                 \
({                                                                             \
    _cc_dllst_all_nodes_free(_cc_dllst);                                       \
                                                                               \
    _cc_dllst.size = 0;                                                        \
    _cc_dllst.vcnt = 0;                                                        \
    _cc_dllst.head = (void*)&(_cc_dllst.tail);                                 \
    _cc_dllst.tail = (void*)&(_cc_dllst.head);                                 \
})



#endif
