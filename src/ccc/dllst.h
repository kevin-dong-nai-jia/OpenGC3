#ifndef _CCC_DLLST_H_
#define _CCC_DLLST_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "message.h"



/* dllst container node */


#define _cc_dllst_node(_cc_dllst_object)                                   \
                                                                           \
    struct                                                                 \
    {                                                                      \
        _cc_dllst_object##_element_t val;                                  \
        void *link;                                                        \
    }



/* dllst container block */


#ifdef CCC_BLOCK_SIZE
    #define _cc_dllst_block_node_num CCC_BLOCK_SIZE
#else
    #define CCC_BLOCK_SIZE_DEFAULT   16
    #define _cc_dllst_block_node_num CCC_BLOCK_SIZE_DEFAULT
#endif


#define _cc_dllst_block(_cc_dllst_object)                                  \
                                                                           \
    struct                                                                 \
    {                                                                      \
        void *next;                                                        \
        _cc_dllst_object##_node_t arr[_cc_dllst_block_node_num];           \
    }



/* dllst container constructor */


#define _cc_dllst_struct(_cc_dllst_object)                                 \
                                                                           \
    struct                                                                 \
    {                                                                      \
        void *pool;                                                        \
        _cc_dllst_object##_element_t *avsp, *head, *tail;                  \
                                                                           \
        int size, vcnt;                                                    \
        size_t node_size, block_size;                                      \
        ptrdiff_t val_offset, link_offset, arr_offset;                     \
    }


#define _cc_dllst_struct_init {NULL, NULL, NULL, NULL,                     \
                               0, 0, 0, 0, 0, 0, 0}


#define cc_dllst(_cc_dllst_object, _cc_dllst_element_t)                    \
                                                                           \
    typedef _cc_dllst_element_t _cc_dllst_object##_element_t;              \
    typedef _cc_dllst_node  (_cc_dllst_object) _cc_dllst_object##_node_t;  \
    typedef _cc_dllst_block (_cc_dllst_object) _cc_dllst_object##_block_t; \
    typedef _cc_dllst_struct(_cc_dllst_object) _cc_dllst_object##_t;       \
                                                                           \
    _cc_dllst_object##_t       _cc_dllst_object = _cc_dllst_struct_init;   \
    _cc_dllst_object##_node_t  _cc_dllst_object##_node;                    \
    _cc_dllst_object##_block_t _cc_dllst_object##_block;                   \
                                                                           \
    _cc_dllst_object.node_size  = sizeof(_cc_dllst_object##_node);         \
    _cc_dllst_object.block_size = sizeof(_cc_dllst_object##_block);        \
                                                                           \
    _cc_dllst_object.val_offset  = &(_cc_dllst_object##_node.val) -        \
                                   (_cc_dllst_object##_element_t*)         \
                                   &(_cc_dllst_object##_node.link);        \
    _cc_dllst_object.link_offset = (char*)&(_cc_dllst_object##_node.link) -\
                                   (char*)&(_cc_dllst_object##_node.val);  \
    _cc_dllst_object.arr_offset  = (char*)&(_cc_dllst_object##_block.arr) -\
                                   (char*)&(_cc_dllst_object##_block.next);


#define cc_dllst_packed(_cc_dllst_object, _cc_dllst_element_t)             \
                                                                           \
    _Pragma("pack(push,1)")                                                \
                                                                           \
    cc_dllst(_cc_dllst_object, _cc_dllst_element_t);                       \
                                                                           \
    _Pragma("pack(pop)")



/* dllst container iterators */


#define _cc_dllst_iter_struct(_cc_dllst_object)                            \
                                                                           \
    struct                                                                 \
    {                                                                      \
        _cc_dllst_object##_t *pobj;                                        \
        _cc_dllst_object##_element_t *prev, *curr, *next;                  \
    }


#define _cc_dllst_iter_struct_init {NULL, NULL, NULL, NULL}


#define cc_dllst_iter(_cc_dllst_iter, _cc_dllst_object)                    \
                                                                           \
    typedef _cc_dllst_object##_element_t _cc_dllst_iter##_element_t;       \
    typedef _cc_dllst_iter_struct(_cc_dllst_object) _cc_dllst_iter##_t;    \
                                                                           \
    _cc_dllst_iter##_t _cc_dllst_iter = _cc_dllst_iter_struct_init;        \
                                                                           \
    _cc_dllst_iter.pobj = &(_cc_dllst_object);


#define cc_dllst_iter_copy(_cc_dllst_iter_dst, _cc_dllst_iter_src)         \
(                                                                          \
    _cc_dllst_iter_dst.prev = _cc_dllst_iter_src.prev,                     \
    _cc_dllst_iter_dst.curr = _cc_dllst_iter_src.curr,                     \
    _cc_dllst_iter_dst.next = _cc_dllst_iter_src.next, (void)0             \
)


#define cc_dllst_iter_valid(_cc_dllst_iter)                                \
(                                                                          \
    _cc_dllst_iter.prev != NULL && _cc_dllst_iter.next != NULL             \
)


#define cc_dllst_iter_dereference(_cc_dllst_iter)                          \
(                                                                          \
    *(_cc_dllst_iter.curr + _cc_dllst_iter.pobj->val_offset)               \
)


#define cc_dllst_iter_deref(_cc_dllst_iter)                                \
(                                                                          \
    cc_dllst_iter_dereference(_cc_dllst_iter)                              \
)


#define _cc_xor_2_addrs(_cc_addr_a, _cc_addr_b)                            \
(                                                                          \
    (void*)((uintptr_t)(void*)(_cc_addr_a) ^                               \
            (uintptr_t)(void*)(_cc_addr_b))                                \
)


#define _cc_xor_3_addrs(_cc_addr_a, _cc_addr_b, _cc_addr_c)                \
(                                                                          \
    (void*)((uintptr_t)(void*)(_cc_addr_a) ^                               \
            (uintptr_t)(void*)(_cc_addr_b) ^                               \
            (uintptr_t)(void*)(_cc_addr_c))                                \
)


#define cc_dllst_iter_begin(_cc_dllst_iter, _cc_dllst_object)              \
(                                                                          \
    (_cc_dllst_object.head != NULL) ?                                      \
    (                                                                      \
        _cc_dllst_iter.prev = (void*)&(_cc_dllst_object.head),             \
        _cc_dllst_iter.curr = _cc_dllst_object.head,                       \
        _cc_dllst_iter.next = _cc_xor_2_addrs(&(_cc_dllst_object.head),    \
                                       *(void**)_cc_dllst_object.head)     \
    ) : (__cc_warning_dllst_is_empty(_cc_dllst_object))                    \
)


#define cc_dllst_iter_end(_cc_dllst_iter, _cc_dllst_object)                \
(                                                                          \
    (_cc_dllst_object.tail != NULL) ?                                      \
    (                                                                      \
        _cc_dllst_iter.next = (void*)&(_cc_dllst_object.tail),             \
        _cc_dllst_iter.curr = _cc_dllst_object.tail,                       \
        _cc_dllst_iter.prev = _cc_xor_2_addrs(&(_cc_dllst_object.tail),    \
                                       *(void**)_cc_dllst_object.tail)     \
    ) : (__cc_warning_dllst_is_empty(_cc_dllst_object))                    \
)


#define cc_dllst_iter_incr(_cc_dllst_iter)                                 \
(                                                                          \
    (cc_dllst_iter_valid(_cc_dllst_iter)) ?                                \
    (                                                                      \
        _cc_dllst_iter.prev = _cc_dllst_iter.curr,                         \
        _cc_dllst_iter.curr = _cc_dllst_iter.next,                         \
        _cc_dllst_iter.next =                                              \
        (                                                                  \
            (_cc_dllst_iter.curr == (void*)&(_cc_dllst_iter.pobj->tail)) ? \
            (NULL) : (_cc_xor_2_addrs(_cc_dllst_iter.prev,                 \
                             *(void**)_cc_dllst_iter.curr))                \
        )                                                                  \
    ) : (__cc_warning_dllst_iter_is_invalid(_cc_dllst_iter))               \
)


#define cc_dllst_iter_decr(_cc_dllst_iter)                                 \
(                                                                          \
    (cc_dllst_iter_valid(_cc_dllst_iter)) ?                                \
    (                                                                      \
        _cc_dllst_iter.next = _cc_dllst_iter.curr,                         \
        _cc_dllst_iter.curr = _cc_dllst_iter.prev,                         \
        _cc_dllst_iter.prev =                                              \
        (                                                                  \
            (_cc_dllst_iter.curr == (void*)&(_cc_dllst_iter.pobj->head)) ? \
            (NULL) : (_cc_xor_2_addrs(_cc_dllst_iter.next,                 \
                             *(void**)_cc_dllst_iter.curr))                \
        )                                                                  \
    ) : (__cc_warning_dllst_iter_is_invalid(_cc_dllst_iter))               \
)



/* dllst container traversal */


#define cc_dllst_trav(_cc_dllst_object, _cc_dllst_iter)                    \
                                                                           \
for                                                                        \
(                                                                          \
    cc_dllst_iter_begin(_cc_dllst_iter, _cc_dllst_object);                 \
    cc_dllst_iter_valid(_cc_dllst_iter);                                   \
    cc_dllst_iter_incr (_cc_dllst_iter)                                    \
)


#define cc_dllst_trav_rev(_cc_dllst_object, _cc_dllst_iter)                \
                                                                           \
for                                                                        \
(                                                                          \
    cc_dllst_iter_end  (_cc_dllst_iter, _cc_dllst_object);                 \
    cc_dllst_iter_valid(_cc_dllst_iter);                                   \
    cc_dllst_iter_decr (_cc_dllst_iter)                                    \
)



/* dllst container capacity */


#define cc_dllst_size(_cc_dllst_object)                                    \
(                                                                          \
    _cc_dllst_object.size                                                  \
)


#define cc_dllst_empty(_cc_dllst_object)                                   \
(                                                                          \
    cc_dllst_size(_cc_dllst_object) == 0                                   \
)



/* dllst container element access */


#define cc_dllst_front(_cc_dllst_object)                                   \
(                                                                          \
    *(_cc_dllst_object.head + _cc_dllst_object.val_offset)                 \
)


#define cc_dllst_back(_cc_dllst_object)                                    \
(                                                                          \
    *(_cc_dllst_object.tail + _cc_dllst_object.val_offset)                 \
)



/* dllst container node operation */


#define _cc_dllst_node_alloc(_cc_dllst_node_ptr, _cc_dllst_object)         \
{                                                                          \
    if (_cc_dllst_object.avsp == NULL)                                     \
    {                                                                      \
        if (_cc_dllst_object.vcnt == 0)                                    \
        {                                                                  \
            void *pool_dup = _cc_dllst_object.pool;                        \
            _cc_dllst_object.pool = malloc(_cc_dllst_object.block_size);   \
                                                                           \
            if (_cc_dllst_object.pool == NULL)                             \
                (void)__cc_error_dllst_failed_to_alloc(_cc_dllst_object);  \
                                                                           \
            *(void**)_cc_dllst_object.pool = pool_dup;                     \
            _cc_dllst_object.vcnt = _cc_dllst_block_node_num;              \
        }                                                                  \
                                                                           \
        _cc_dllst_node_ptr = (void*)                                       \
            ((char*)_cc_dllst_object.pool +                                \
             _cc_dllst_object.arr_offset +                                 \
             _cc_dllst_object.node_size * (--(_cc_dllst_object.vcnt)));    \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        _cc_dllst_node_ptr =                                               \
            _cc_dllst_object.avsp + _cc_dllst_object.val_offset;           \
                                                                           \
        _cc_dllst_object.avsp = *(void**)_cc_dllst_object.avsp;            \
    }                                                                      \
}


#define _cc_dllst_node_clear(_cc_dllst_node_link_ptr, _cc_dllst_object)    \
{                                                                          \
    *(void**)_cc_dllst_node_link_ptr = _cc_dllst_object.avsp;              \
    _cc_dllst_object.avsp = _cc_dllst_node_link_ptr;                       \
}


#define _cc_dllst_all_nodes_free(_cc_dllst_object)                         \
{                                                                          \
    while (_cc_dllst_object.pool != NULL)                                  \
    {                                                                      \
        void *pool_dup = _cc_dllst_object.pool;                            \
        _cc_dllst_object.pool = *(void**)_cc_dllst_object.pool;            \
        free(pool_dup);                                                    \
    }                                                                      \
}



/* dllst container modifiers */


#define cc_dllst_push_front(_cc_dllst_object, _cc_dllst_push_front_value)  \
{                                                                          \
    void *node, **link;                                                    \
    _cc_dllst_object##_element_t *val;                                     \
                                                                           \
    _cc_dllst_node_alloc(node, _cc_dllst_object);                          \
                                                                           \
    *(val = node) = _cc_dllst_push_front_value;                            \
    link = (void*)((char*)node + _cc_dllst_object.link_offset);            \
                                                                           \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
    {                                                                      \
        *link = _cc_xor_2_addrs(&(_cc_dllst_object.head),                  \
                                &(_cc_dllst_object.tail));                 \
                                                                           \
        _cc_dllst_object.tail = (void*)link;                               \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        *link = _cc_xor_2_addrs(&(_cc_dllst_object.head),                  \
                                  _cc_dllst_object.head);                  \
                                                                           \
        *(void**)_cc_dllst_object.head = _cc_xor_3_addrs(link,             \
            &(_cc_dllst_object.head), *(void**)_cc_dllst_object.head);     \
    }                                                                      \
                                                                           \
    _cc_dllst_object.head = (void*)link;                                   \
    _cc_dllst_object.size++;                                               \
}


#define cc_dllst_push_back(_cc_dllst_object, _cc_dllst_push_back_value)    \
{                                                                          \
    void *node, **link;                                                    \
    _cc_dllst_object##_element_t *val;                                     \
                                                                           \
    _cc_dllst_node_alloc(node, _cc_dllst_object);                          \
                                                                           \
    *(val = node) = _cc_dllst_push_back_value;                             \
    link = (void*)((char*)node + _cc_dllst_object.link_offset);            \
                                                                           \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
    {                                                                      \
        *link = _cc_xor_2_addrs(&(_cc_dllst_object.head),                  \
                                &(_cc_dllst_object.tail));                 \
                                                                           \
        _cc_dllst_object.head = (void*)link;                               \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        *link = _cc_xor_2_addrs(&(_cc_dllst_object.tail),                  \
                                  _cc_dllst_object.tail);                  \
                                                                           \
        *(void**)_cc_dllst_object.tail = _cc_xor_3_addrs(link,             \
            &(_cc_dllst_object.tail), *(void**)_cc_dllst_object.tail);     \
    }                                                                      \
                                                                           \
    _cc_dllst_object.tail = (void*)link;                                   \
    _cc_dllst_object.size++;                                               \
}


#define cc_dllst_pop_front(_cc_dllst_object)                               \
{                                                                          \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
    {                                                                      \
        (void)__cc_warning_dllst_is_empty(_cc_dllst_object);               \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        void *head = _cc_dllst_object.head;                                \
                                                                           \
        if (cc_dllst_size(_cc_dllst_object) == 1)                          \
        {                                                                  \
            _cc_dllst_object.head = NULL;                                  \
            _cc_dllst_object.tail = NULL;                                  \
        }                                                                  \
        else                                                               \
        {                                                                  \
            _cc_dllst_object.head = _cc_xor_2_addrs(                       \
                &(_cc_dllst_object.head), *(void**)_cc_dllst_object.head); \
                                                                           \
            *(void**)_cc_dllst_object.head = _cc_xor_3_addrs(head,         \
                &(_cc_dllst_object.head), *(void**)_cc_dllst_object.head); \
        }                                                                  \
                                                                           \
        _cc_dllst_node_clear(head, _cc_dllst_object);                      \
                                                                           \
        _cc_dllst_object.size--;                                           \
    }                                                                      \
}


#define cc_dllst_pop_back(_cc_dllst_object)                                \
{                                                                          \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
    {                                                                      \
        (void)__cc_warning_dllst_is_empty(_cc_dllst_object);               \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        void *tail = _cc_dllst_object.tail;                                \
                                                                           \
        if (cc_dllst_size(_cc_dllst_object) == 1)                          \
        {                                                                  \
            _cc_dllst_object.head = NULL;                                  \
            _cc_dllst_object.tail = NULL;                                  \
        }                                                                  \
        else                                                               \
        {                                                                  \
            _cc_dllst_object.tail = _cc_xor_2_addrs(                       \
                &(_cc_dllst_object.tail), *(void**)_cc_dllst_object.tail); \
                                                                           \
            *(void**)_cc_dllst_object.tail = _cc_xor_3_addrs(tail,         \
                &(_cc_dllst_object.tail), *(void**)_cc_dllst_object.tail); \
        }                                                                  \
                                                                           \
        _cc_dllst_node_clear(tail, _cc_dllst_object);                      \
                                                                           \
        _cc_dllst_object.size--;                                           \
    }                                                                      \
}


#define cc_dllst_insert(_cc_dllst_iter, _cc_dllst_insert_value)            \
    /* TODO */


#define cc_dllst_erase(_cc_dllst_iter)                                     \
    /* TODO */


#define cc_dllst_swap(_cc_dllst_object_a, _cc_dllst_object_b)              \
    /* TODO */


#define cc_dllst_resize(_cc_dllst_object, _cc_dllst_padding_value)         \
    /* TODO */


#define cc_dllst_clear(_cc_dllst_object)                                   \
{                                                                          \
    do cc_dllst_pop_back(_cc_dllst_object)                                 \
    while (!(cc_dllst_empty(_cc_dllst_object)));                           \
}



/* dllst container destructor */


#define cc_dllst_free(_cc_dllst_object)                                    \
{                                                                          \
    _cc_dllst_all_nodes_free(_cc_dllst_object);                            \
                                                                           \
    _cc_dllst_object.size = 0;                                             \
    _cc_dllst_object.vcnt = 0;                                             \
    _cc_dllst_object.head = NULL;                                          \
    _cc_dllst_object.tail = NULL;                                          \
}



#endif
