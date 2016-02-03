#ifndef _CCC_DLLST_H_
#define _CCC_DLLST_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "debug.h"



/* dllst container node */


#define _cc_dllst_node(_cc_dllst_object)                                   \
                                                                           \
    struct                                                                 \
    {                                                                      \
        _cc_dllst_object##_elem_type val;                                  \
        void *link;                                                        \
    }



/* dllst container block */


#define CCC_BLOCK_ELEM_DEFAULT 16

#ifdef CCC_BLOCK_ELEM
    #define _cc_dllst_block_elem CCC_BLOCK_ELEM
#else
    #define _cc_dllst_block_elem CCC_BLOCK_ELEM_DEFAULT
#endif


#define _cc_dllst_block(_cc_dllst_object)                                  \
                                                                           \
    struct                                                                 \
    {                                                                      \
        void *next;                                                        \
        _cc_dllst_node(_cc_dllst_object) arr[_cc_dllst_block_elem];        \
    }



/* dllst container constructor */


#define _cc_dllst_struct(_cc_dllst_object)                                 \
                                                                           \
    struct                                                                 \
    {                                                                      \
        int size, vcnt;                                                    \
        void *pool, *pool_dup;                                             \
        _cc_dllst_object##_elem_type *avsp, *head, *tail;                  \
                                                                           \
        size_t node_size, block_size;                                      \
        ptrdiff_t val_offset, link_offset, arr_offset;                     \
    }


#define _cc_dllst_struct_init {0, 0, NULL, NULL, NULL,                     \
                               NULL, NULL, 0, 0, 0, 0, 0}


#define cc_dllst(_cc_dllst_object, _cc_dllst_elem_type)                    \
                                                                           \
    typedef _cc_dllst_elem_type _cc_dllst_object##_elem_type;              \
                                                                           \
    _cc_dllst_struct(_cc_dllst_object)                                     \
        _cc_dllst_object = _cc_dllst_struct_init;                          \
                                                                           \
    _cc_dllst_node(_cc_dllst_object) _cc_dllst_object##_node;              \
                                                                           \
    _cc_dllst_object.node_size = sizeof(_cc_dllst_object##_node);          \
                                                                           \
    _cc_dllst_object.val_offset = &(_cc_dllst_object##_node.val) -         \
                                  (_cc_dllst_object##_elem_type*)          \
                                      &(_cc_dllst_object##_node.link);     \
                                                                           \
    _cc_dllst_object.link_offset = (char*)&(_cc_dllst_object##_node.link) -\
                                   (char*)&(_cc_dllst_object##_node.val);  \
                                                                           \
    _cc_dllst_block(_cc_dllst_object) _cc_dllst_object##_block;            \
                                                                           \
    _cc_dllst_object.block_size = sizeof(_cc_dllst_object##_block);        \
                                                                           \
    _cc_dllst_object.arr_offset = (char*)&(_cc_dllst_object##_block.arr) - \
                                  (char*)&(_cc_dllst_object##_block.next);


#define cc_dllst_packed(_cc_dllst_object, _cc_dllst_elem_type)             \
                                                                           \
    _Pragma("pack(push,1)")                                                \
                                                                           \
    cc_dllst(_cc_dllst_object, _cc_dllst_elem_type);                       \
                                                                           \
    _Pragma("pack(pop)")



/* dllst container iterators */


#define _cc_dllst_iter_struct(_cc_dllst_object)                            \
                                                                           \
    struct                                                                 \
    {                                                                      \
        _cc_dllst_struct(_cc_dllst_object) *pobj;                          \
        _cc_dllst_object##_elem_type **iter, *info[5];                     \
    }


#define _cc_dllst_iter_struct_init {NULL, NULL,                            \
                                    {NULL, NULL, NULL, NULL, NULL}}


#define cc_dllst_iter(_cc_dllst_iter,  _cc_dllst_object)                   \
                                                                           \
    _cc_dllst_iter_struct(_cc_dllst_object)                                \
        _cc_dllst_iter = _cc_dllst_iter_struct_init;                       \
                                                                           \
    _cc_dllst_iter.iter = &(_cc_dllst_iter.info[2]);                       \
                                                                           \
    _cc_dllst_iter.pobj = (void*)&(_cc_dllst_object);


#define cc_dllst_packed_iter(_cc_dllst_iter, _cc_dllst_object)             \
                                                                           \
    _Pragma("pack(push,1)")                                                \
                                                                           \
    cc_dllst_iter(_cc_dllst_iter, _cc_dllst_object);                       \
                                                                           \
    _Pragma("pack(pop)")


#define cc_dllst_iter_copy(_cc_dllst_iter_dst, _cc_dllst_iter_src)         \
(                                                                          \
    memcpy(_cc_dllst_iter_dst.info,                                        \
           _cc_dllst_iter_src.info,                                        \
           sizeof(_cc_dllst_iter_dst.info))                                \
)


#define cc_dllst_iter_clear(_cc_dllst_iter)                                \
(                                                                          \
    memset(_cc_dllst_iter.info, 0, sizeof(_cc_dllst_iter.info))            \
)


#define cc_dllst_iter_valid(_cc_dllst_iter)                                \
(                                                                          \
       _cc_dllst_iter.iter != NULL                                         \
    && *(_cc_dllst_iter.iter - 1) != NULL                                  \
    && *(_cc_dllst_iter.iter + 1) != NULL                                  \
)


#define cc_dllst_iter_dereference(_cc_dllst_iter)                          \
(                                                                          \
    *(*_cc_dllst_iter.iter + _cc_dllst_iter.pobj->val_offset)              \
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
    (_cc_dllst_object.head == NULL) ?                                      \
    (                                                                      \
        __cc_warning_dllst_is_empty(_cc_dllst_object)                      \
    ) :                                                                    \
    (                                                                      \
        _cc_dllst_iter.info[0] = (void*)&(_cc_dllst_object.head),          \
        _cc_dllst_iter.info[1] = (void*)&(_cc_dllst_object.head),          \
        _cc_dllst_iter.info[2] = _cc_dllst_object.head,                    \
        _cc_dllst_iter.info[3] = _cc_xor_2_addrs(&(_cc_dllst_object.head), \
                                           *(void**)_cc_dllst_object.head),\
        _cc_dllst_iter.info[4] = (void*)&(_cc_dllst_object.tail)           \
    )                                                                      \
)


#define cc_dllst_iter_end(_cc_dllst_iter, _cc_dllst_object)                \
(                                                                          \
    (_cc_dllst_object.tail == NULL) ?                                      \
    (                                                                      \
        __cc_warning_dllst_is_empty(_cc_dllst_object)                      \
    ) :                                                                    \
    (                                                                      \
        _cc_dllst_iter.info[4] = (void*)&(_cc_dllst_object.tail),          \
        _cc_dllst_iter.info[3] = (void*)&(_cc_dllst_object.tail),          \
        _cc_dllst_iter.info[2] = _cc_dllst_object.tail,                    \
        _cc_dllst_iter.info[1] = _cc_xor_2_addrs(&(_cc_dllst_object.tail), \
                                           *(void**)_cc_dllst_object.tail),\
        _cc_dllst_iter.info[0] = (void*)&(_cc_dllst_object.head)           \
    )                                                                      \
)


#define cc_dllst_iter_incr(_cc_dllst_iter)                                 \
(                                                                          \
    (!(cc_dllst_iter_valid(_cc_dllst_iter))) ?                             \
    (                                                                      \
        __cc_warning_dllst_iter_is_invalid(_cc_dllst_iter)                 \
    ) :                                                                    \
    (                                                                      \
        _cc_dllst_iter.info[1] = _cc_dllst_iter.info[2],                   \
        _cc_dllst_iter.info[2] = _cc_dllst_iter.info[3],                   \
        _cc_dllst_iter.info[3] =                                           \
        (                                                                  \
            (_cc_dllst_iter.info[2] == _cc_dllst_iter.info[4]) ? (NULL) :  \
            (_cc_xor_2_addrs(_cc_dllst_iter.info[1],                       \
                             *(void**)_cc_dllst_iter.info[2]))             \
        )                                                                  \
    )                                                                      \
)


#define cc_dllst_iter_decr(_cc_dllst_iter)                                 \
(                                                                          \
    (!(cc_dllst_iter_valid(_cc_dllst_iter))) ?                             \
    (                                                                      \
        __cc_warning_dllst_iter_is_invalid(_cc_dllst_iter)                 \
    ) :                                                                    \
    (                                                                      \
        _cc_dllst_iter.info[3] = _cc_dllst_iter.info[2],                   \
        _cc_dllst_iter.info[2] = _cc_dllst_iter.info[1],                   \
        _cc_dllst_iter.info[1] =                                           \
        (                                                                  \
            (_cc_dllst_iter.info[2] == _cc_dllst_iter.info[0]) ? (NULL) :  \
            (_cc_xor_2_addrs(_cc_dllst_iter.info[3],                       \
                             *(void**)_cc_dllst_iter.info[2]))             \
        )                                                                  \
    )                                                                      \
)



/* dllst container traversal */


#define cc_dllst_trav(_cc_dllst_object, _cc_dllst_iter)                    \
                                                                           \
for                                                                        \
(                                                                          \
    cc_dllst_iter_begin(_cc_dllst_iter, _cc_dllst_object);                 \
    cc_dllst_iter_valid(_cc_dllst_iter);                                   \
    cc_dllst_iter_incr(_cc_dllst_iter)                                     \
)


#define cc_dllst_trav_rev(_cc_dllst_object, _cc_dllst_iter)                \
                                                                           \
for                                                                        \
(                                                                          \
    cc_dllst_iter_end(_cc_dllst_iter, _cc_dllst_object);                   \
    cc_dllst_iter_valid(_cc_dllst_iter);                                   \
    cc_dllst_iter_decr(_cc_dllst_iter)                                     \
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



/* dllst container allocation */


#define _cc_dllst_node_allocate(_cc_dllst_object)                          \
(                                                                          \
    (void)                                                                 \
    (                                                                      \
        (_cc_dllst_object.vcnt == 0) ?                                     \
        (                                                                  \
            _cc_dllst_object.vcnt = _cc_dllst_block_elem,                  \
            _cc_dllst_object.pool_dup = _cc_dllst_object.pool,             \
            _cc_dllst_object.pool = malloc(_cc_dllst_object.block_size),   \
            *(void**)_cc_dllst_object.pool = _cc_dllst_object.pool_dup     \
        ) : (NULL)                                                         \
    ) ,                                                                    \
    (void*)((char*)_cc_dllst_object.pool +                                 \
            _cc_dllst_object.arr_offset +                                  \
            _cc_dllst_object.node_size * (--(_cc_dllst_object.vcnt)))      \
)



/* dllst container modifiers */


#define cc_dllst_push_front(_cc_dllst_object, _cc_dllst_push_front_value)  \
{                                                                          \
    void *node, **link;                                                    \
    _cc_dllst_object##_elem_type *val;                                     \
                                                                           \
    if (_cc_dllst_object.avsp == NULL)                                     \
    {                                                                      \
        node = _cc_dllst_node_allocate(_cc_dllst_object);                  \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        node = _cc_dllst_object.avsp + _cc_dllst_object.val_offset;        \
        _cc_dllst_object.avsp = *(void**)_cc_dllst_object.avsp;            \
    }                                                                      \
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
                                _cc_dllst_object.head);                    \
                                                                           \
        *(void**)_cc_dllst_object.head =                                   \
            _cc_xor_3_addrs(*(void**)_cc_dllst_object.head,                \
                            &(_cc_dllst_object.head), link);               \
    }                                                                      \
                                                                           \
    _cc_dllst_object.size++;                                               \
    _cc_dllst_object.head = (void*)link;                                   \
}


#define cc_dllst_push_back(_cc_dllst_object, _cc_dllst_push_back_value)    \
{                                                                          \
    void *node, **link;                                                    \
    _cc_dllst_object##_elem_type *val;                                     \
                                                                           \
    if (_cc_dllst_object.avsp == NULL)                                     \
    {                                                                      \
        node = _cc_dllst_node_allocate(_cc_dllst_object);                  \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        node = _cc_dllst_object.avsp + _cc_dllst_object.val_offset;        \
        _cc_dllst_object.avsp = *(void**)_cc_dllst_object.avsp;            \
    }                                                                      \
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
                                _cc_dllst_object.tail);                    \
                                                                           \
        *(void**)_cc_dllst_object.tail =                                   \
            _cc_xor_3_addrs(*(void**)_cc_dllst_object.tail,                \
                            &(_cc_dllst_object.tail), link);               \
    }                                                                      \
                                                                           \
    _cc_dllst_object.size++;                                               \
    _cc_dllst_object.tail = (void*)link;                                   \
}


#define cc_dllst_pop_front(_cc_dllst_object)                               \
{                                                                          \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
    {                                                                      \
        __cc_warning_dllst_is_empty(_cc_dllst_object);                     \
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
            _cc_dllst_object.head =                                        \
                _cc_xor_2_addrs(*(void**)_cc_dllst_object.head,            \
                                &(_cc_dllst_object.head));                 \
                                                                           \
            *(void**)_cc_dllst_object.head =                               \
                _cc_xor_3_addrs(*(void**)_cc_dllst_object.head,            \
                                &(_cc_dllst_object.head), head);           \
        }                                                                  \
                                                                           \
        _cc_dllst_object.size--;                                           \
        *(void**)head = _cc_dllst_object.avsp;                             \
        _cc_dllst_object.avsp = head;                                      \
    }                                                                      \
}


#define cc_dllst_pop_back(_cc_dllst_object)                                \
{                                                                          \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
    {                                                                      \
        __cc_warning_dllst_is_empty(_cc_dllst_object);                     \
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
            _cc_dllst_object.tail =                                        \
                _cc_xor_2_addrs(*(void**)_cc_dllst_object.tail,            \
                                &(_cc_dllst_object.tail));                 \
                                                                           \
            *(void**)_cc_dllst_object.tail =                               \
                _cc_xor_3_addrs(*(void**)_cc_dllst_object.tail,            \
                                &(_cc_dllst_object.tail), tail);           \
        }                                                                  \
                                                                           \
        _cc_dllst_object.size--;                                           \
        *(void**)tail = _cc_dllst_object.avsp;                             \
        _cc_dllst_object.avsp = tail;                                      \
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
    while (_cc_dllst_object.pool != NULL)                                  \
    {                                                                      \
        _cc_dllst_object.pool_dup = _cc_dllst_object.pool;                 \
        _cc_dllst_object.pool = *(void**)_cc_dllst_object.pool;            \
        free(_cc_dllst_object.pool_dup);                                   \
    }                                                                      \
                                                                           \
    _cc_dllst_object.size = 0;                                             \
    _cc_dllst_object.vcnt = 0;                                             \
    _cc_dllst_object.head = NULL;                                          \
    _cc_dllst_object.tail = NULL;                                          \
}



#endif
