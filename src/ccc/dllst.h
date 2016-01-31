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
        void *lnk;                                                         \
        _cc_dllst_object##_element_type val;                               \
    }



/* dllst container constructor */


#define _cc_dllst_init {0, NULL, NULL, NULL}


#define cc_dllst(_cc_dllst_object,                                         \
                 _cc_dllst_element_type)                                   \
                                                                           \
    struct                                                                 \
    {                                                                      \
        int size;                                                          \
        void *head, *tail, *avsp;                                          \
    }   _cc_dllst_object = _cc_dllst_init;                                 \
                                                                           \
    typedef _cc_dllst_element_type _cc_dllst_object##_element_type;        \
                                                                           \
    _cc_dllst_node(_cc_dllst_object) _cc_dllst_object##_internal_node;     \
                                                                           \
    ptrdiff_t _cc_dllst_object##_val_offset =                              \
        (char*)&(_cc_dllst_object##_internal_node.val) -                   \
        (char*)&(_cc_dllst_object##_internal_node.lnk)



/* dllst container iterators */


#define _cc_dllst_iter_hptnt_init {NULL, NULL, NULL, NULL, NULL}


#define cc_dllst_iter(_cc_dllst_iter,                                      \
                      _cc_dllst_object)                                    \
                                                                           \
    void *_cc_dllst_iter##_hptnt[5] = _cc_dllst_iter_hptnt_init;           \
                                                                           \
    void** const _cc_dllst_iter = &(_cc_dllst_iter##_hptnt[2]);            \
                                                                           \
    typedef _cc_dllst_object##_element_type _cc_dllst_iter##_element_type; \
                                                                           \
    ptrdiff_t _cc_dllst_iter##_val_offset = _cc_dllst_object##_val_offset


#define cc_dllst_iter_copy(_cc_dllst_iter_dst,                             \
                           _cc_dllst_iter_src)                             \
{                                                                          \
    memcpy(_cc_dllst_iter_dst##_hptnt,                                     \
           _cc_dllst_iter_src##_hptnt,                                     \
           sizeof(_cc_dllst_iter_dst##_hptnt))                             \
}


#define cc_dllst_iter_clear(_cc_dllst_iter)                                \
{                                                                          \
    memset(_cc_dllst_iter##_hptnt, 0, sizeof(_cc_dllst_iter##_hptnt))      \
}


#define cc_dllst_iter_valid(_cc_dllst_iter)                                \
(                                                                          \
       _cc_dllst_iter != NULL                                              \
    && *(_cc_dllst_iter - 1) != NULL                                       \
    && *(_cc_dllst_iter + 1) != NULL                                       \
)


#define cc_dllst_iter_dereference(_cc_dllst_iter)                          \
(                                                                          \
   *(                                                                      \
        (_cc_dllst_iter##_element_type*)                                   \
        (                                                                  \
            (!(cc_dllst_iter_valid(_cc_dllst_iter))) ?                     \
            __cc_error_dllst_iter_cannot_be_dereferenced(_cc_dllst_iter) : \
            (char*)(*_cc_dllst_iter) + _cc_dllst_iter##_val_offset         \
        )                                                                  \
    )                                                                      \
)


#define cc_dllst_iter_deref(_cc_dllst_iter)                                \
(                                                                          \
    cc_dllst_iter_dereference(_cc_dllst_iter)                              \
)


#define _cc_dllst_addr_xor_2(_cc_dllst_addr_a,                             \
                             _cc_dllst_addr_b)                             \
(                                                                          \
    (void**)((uintptr_t)(void*)(_cc_dllst_addr_a) ^                        \
             (uintptr_t)(void*)(_cc_dllst_addr_b))                         \
)


#define _cc_dllst_addr_xor_3(_cc_dllst_addr_a,                             \
                             _cc_dllst_addr_b,                             \
                             _cc_dllst_addr_c)                             \
(                                                                          \
    (void**)((uintptr_t)(void*)(_cc_dllst_addr_a) ^                        \
             (uintptr_t)(void*)(_cc_dllst_addr_b) ^                        \
             (uintptr_t)(void*)(_cc_dllst_addr_c))                         \
)


#define cc_dllst_iter_begin(_cc_dllst_iter,                                \
                            _cc_dllst_object)                              \
(                                                                          \
    (_cc_dllst_object.head == NULL) ?                                      \
    (                                                                      \
        (void**)__cc_warning_dllst_is_empty(_cc_dllst_object)              \
    ) :                                                                    \
    (                                                                      \
        _cc_dllst_iter##_hptnt[0] = &(_cc_dllst_object.head),              \
        _cc_dllst_iter##_hptnt[1] = &(_cc_dllst_object.head),              \
        _cc_dllst_iter##_hptnt[2] = _cc_dllst_object.head,                 \
        _cc_dllst_iter##_hptnt[3] =                                        \
            _cc_dllst_addr_xor_2(&(_cc_dllst_object.head),                 \
                                 *(void**)_cc_dllst_object.head),          \
        _cc_dllst_iter##_hptnt[4] = &(_cc_dllst_object.tail),              \
        _cc_dllst_iter                                                     \
    )                                                                      \
)


#define cc_dllst_iter_end(_cc_dllst_iter,                                  \
                          _cc_dllst_object)                                \
(                                                                          \
    (_cc_dllst_object.tail == NULL) ?                                      \
    (                                                                      \
        (void**)__cc_warning_dllst_is_empty(_cc_dllst_object)              \
    ) :                                                                    \
    (                                                                      \
        _cc_dllst_iter##_hptnt[4] = &(_cc_dllst_object.tail),              \
        _cc_dllst_iter##_hptnt[3] = &(_cc_dllst_object.tail),              \
        _cc_dllst_iter##_hptnt[2] = _cc_dllst_object.tail,                 \
        _cc_dllst_iter##_hptnt[1] =                                        \
            _cc_dllst_addr_xor_2(&(_cc_dllst_object.tail),                 \
                                 *(void**)_cc_dllst_object.tail),          \
        _cc_dllst_iter##_hptnt[0] = &(_cc_dllst_object.head),              \
        _cc_dllst_iter                                                     \
    )                                                                      \
)


#define cc_dllst_iter_incr(_cc_dllst_iter)                                 \
(                                                                          \
    (!(cc_dllst_iter_valid(_cc_dllst_iter))) ?                             \
    (                                                                      \
        (void**)__cc_warning_dllst_iter_is_invalid(_cc_dllst_iter)         \
    ) :                                                                    \
    (                                                                      \
        _cc_dllst_iter##_hptnt[1] = _cc_dllst_iter##_hptnt[2],             \
        _cc_dllst_iter##_hptnt[2] = _cc_dllst_iter##_hptnt[3],             \
        _cc_dllst_iter##_hptnt[3] =                                        \
        (                                                                  \
            (_cc_dllst_iter##_hptnt[2] == _cc_dllst_iter##_hptnt[4]) ?     \
            (NULL) :                                                       \
            (_cc_dllst_addr_xor_2(_cc_dllst_iter##_hptnt[1],               \
                                  *(void**)_cc_dllst_iter##_hptnt[2]))     \
        ),                                                                 \
        _cc_dllst_iter                                                     \
    )                                                                      \
)


#define cc_dllst_iter_decr(_cc_dllst_iter)                                 \
(                                                                          \
    (!(cc_dllst_iter_valid(_cc_dllst_iter))) ?                             \
    (                                                                      \
        (void**)__cc_warning_dllst_iter_is_invalid(_cc_dllst_iter)         \
    ) :                                                                    \
    (                                                                      \
        _cc_dllst_iter##_hptnt[3] = _cc_dllst_iter##_hptnt[2],             \
        _cc_dllst_iter##_hptnt[2] = _cc_dllst_iter##_hptnt[1],             \
        _cc_dllst_iter##_hptnt[1] =                                        \
        (                                                                  \
            (_cc_dllst_iter##_hptnt[2] == _cc_dllst_iter##_hptnt[0]) ?     \
            (NULL) :                                                       \
            (_cc_dllst_addr_xor_2(_cc_dllst_iter##_hptnt[3],               \
                                  *(void**)_cc_dllst_iter##_hptnt[2]))     \
        ),                                                                 \
        _cc_dllst_iter                                                     \
    )                                                                      \
)



/* dllst container traversal */


#define cc_dllst_trav(_cc_dllst_object,                                    \
                      _cc_dllst_iter)                                      \
                                                                           \
for                                                                        \
(                                                                          \
    cc_dllst_iter_begin(_cc_dllst_iter, _cc_dllst_object);                 \
    cc_dllst_iter_valid(_cc_dllst_iter);                                   \
    cc_dllst_iter_incr(_cc_dllst_iter)                                     \
)


#define cc_dllst_trav_rev(_cc_dllst_object,                                \
                          _cc_dllst_iter)                                  \
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
   *(                                                                      \
        (_cc_dllst_object##_element_type*)                                 \
        (                                                                  \
            (cc_dllst_empty(_cc_dllst_object)) ?                           \
            __cc_warning_dllst_is_empty(_cc_dllst_object) :                \
            (char*)_cc_dllst_object.head + _cc_dllst_object##_val_offset   \
        )                                                                  \
    )                                                                      \
)


#define cc_dllst_back(_cc_dllst_object)                                    \
(                                                                          \
   *(                                                                      \
        (_cc_dllst_object##_element_type*)                                 \
        (                                                                  \
            (cc_dllst_empty(_cc_dllst_object)) ?                           \
            __cc_warning_dllst_is_empty(_cc_dllst_object) :                \
            (char*)_cc_dllst_object.tail + _cc_dllst_object##_val_offset   \
        )                                                                  \
    )                                                                      \
)



/* dllst container modifiers */


#define cc_dllst_push_front(_cc_dllst_object,                              \
                            _cc_dllst_push_front_value)                    \
{                                                                          \
    _cc_dllst_node(_cc_dllst_object) *new_node;                            \
                                                                           \
    if (_cc_dllst_object.avsp == NULL)                                     \
    {                                                                      \
        new_node = malloc(sizeof(_cc_dllst_node(_cc_dllst_object)));       \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        new_node = _cc_dllst_object.avsp;                                  \
        _cc_dllst_object.avsp = *(void**)_cc_dllst_object.avsp;            \
    }                                                                      \
                                                                           \
    new_node->val = _cc_dllst_push_front_value;                            \
                                                                           \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
    {                                                                      \
        new_node->lnk = _cc_dllst_addr_xor_2(&(_cc_dllst_object.head),     \
                                             &(_cc_dllst_object.tail));    \
                                                                           \
        _cc_dllst_object.tail = &(new_node->lnk);                          \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        new_node->lnk = _cc_dllst_addr_xor_2(&(_cc_dllst_object.head),     \
                                             _cc_dllst_object.head);       \
                                                                           \
        *(void**)_cc_dllst_object.head =                                   \
            _cc_dllst_addr_xor_3(*(void**)_cc_dllst_object.head,           \
                                 &(_cc_dllst_object.head),                 \
                                 &(new_node->lnk));                        \
    }                                                                      \
                                                                           \
    _cc_dllst_object.head = &(new_node->lnk);                              \
    _cc_dllst_object.size++;                                               \
}


#define cc_dllst_push_back(_cc_dllst_object,                               \
                           _cc_dllst_push_back_value)                      \
{                                                                          \
    _cc_dllst_node(_cc_dllst_object) *new_node;                            \
                                                                           \
    if (_cc_dllst_object.avsp == NULL)                                     \
    {                                                                      \
        new_node = malloc(sizeof(_cc_dllst_node(_cc_dllst_object)));       \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        new_node = _cc_dllst_object.avsp;                                  \
        _cc_dllst_object.avsp = *(void**)_cc_dllst_object.avsp;            \
    }                                                                      \
                                                                           \
    new_node->val = _cc_dllst_push_back_value;                             \
                                                                           \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
    {                                                                      \
        new_node->lnk = _cc_dllst_addr_xor_2(&(_cc_dllst_object.head),     \
                                             &(_cc_dllst_object.tail));    \
                                                                           \
        _cc_dllst_object.head = &(new_node->lnk);                          \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        new_node->lnk = _cc_dllst_addr_xor_2(&(_cc_dllst_object.tail),     \
                                             _cc_dllst_object.tail);       \
                                                                           \
        *(void**)_cc_dllst_object.tail =                                   \
            _cc_dllst_addr_xor_3(*(void**)_cc_dllst_object.tail,           \
                                 &(_cc_dllst_object.tail),                 \
                                 &(new_node->lnk));                        \
    }                                                                      \
                                                                           \
    _cc_dllst_object.tail = &(new_node->lnk);                              \
    _cc_dllst_object.size++;                                               \
}


#define cc_dllst_pop_front(_cc_dllst_object)                               \
{                                                                          \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
    {                                                                      \
        __cc_warning_dllst_is_empty(_cc_dllst_object);                     \
    }                                                                      \
    else                                                                   \
    {                                                                      \
        void *begin = _cc_dllst_object.head;                               \
                                                                           \
        if (cc_dllst_size(_cc_dllst_object) == 1)                          \
        {                                                                  \
            _cc_dllst_object.head = NULL;                                  \
            _cc_dllst_object.tail = NULL;                                  \
        }                                                                  \
        else                                                               \
        {                                                                  \
            _cc_dllst_object.head =                                        \
                _cc_dllst_addr_xor_2(*(void**)_cc_dllst_object.head,       \
                                     &(_cc_dllst_object.head));            \
                                                                           \
            *(void**)_cc_dllst_object.head =                               \
                _cc_dllst_addr_xor_3(*(void**)_cc_dllst_object.head,       \
                                     &(_cc_dllst_object.head),             \
                                     begin);                               \
        }                                                                  \
                                                                           \
        *(void**)begin = _cc_dllst_object.avsp;                            \
        _cc_dllst_object.avsp = begin;                                     \
        _cc_dllst_object.size--;                                           \
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
        void *end = _cc_dllst_object.tail;                                 \
                                                                           \
        if (cc_dllst_size(_cc_dllst_object) == 1)                          \
        {                                                                  \
            _cc_dllst_object.head = NULL;                                  \
            _cc_dllst_object.tail = NULL;                                  \
        }                                                                  \
        else                                                               \
        {                                                                  \
            _cc_dllst_object.tail =                                        \
                _cc_dllst_addr_xor_2(*(void**)_cc_dllst_object.tail,       \
                                     &(_cc_dllst_object.tail));            \
                                                                           \
            *(void**)_cc_dllst_object.tail =                               \
                _cc_dllst_addr_xor_3(*(void**)_cc_dllst_object.tail,       \
                                     &(_cc_dllst_object.tail),             \
                                     end);                                 \
        }                                                                  \
                                                                           \
        *(void**)end = _cc_dllst_object.avsp;                              \
        _cc_dllst_object.avsp = end;                                       \
        _cc_dllst_object.size--;                                           \
    }                                                                      \
}


#define cc_dllst_insert(_cc_dllst_iter,                                    \
                        _cc_dllst_insert_value)                            \
    /* TODO */


#define cc_dllst_erase(_cc_dllst_iter)                                     \
    /* TODO */


#define cc_dllst_swap(_cc_dllst_object_a,                                  \
                      _cc_dllst_object_b)                                  \
    /* TODO */


#define cc_dllst_resize(_cc_dllst_object,                                  \
                        _cc_dllst_padding_value)                           \
    /* TODO */


#define cc_dllst_clear(_cc_dllst_object)                                   \
{                                                                          \
    do cc_dllst_pop_back(_cc_dllst_object)                                 \
    while (!(cc_dllst_empty(_cc_dllst_object)));                           \
}



/* dllst container destructor */


#define cc_dllst_free(_cc_dllst_object)                                    \
{                                                                          \
    if (cc_dllst_size(_cc_dllst_object) >= 1)                              \
    {                                                                      \
        cc_dllst_iter(iter, _cc_dllst_object);                             \
        cc_dllst_iter_begin(iter, _cc_dllst_object);                       \
                                                                           \
        do                                                                 \
        {                                                                  \
            cc_dllst_iter_incr(iter);                                      \
            free(*(iter - 1));                                             \
        }                                                                  \
        while (cc_dllst_iter_valid(iter));                                 \
                                                                           \
        (void)iter_val_offset;                                             \
    }                                                                      \
                                                                           \
    while (_cc_dllst_object.avsp != NULL)                                  \
    {                                                                      \
        void *avsp_bup = _cc_dllst_object.avsp;                            \
        _cc_dllst_object.avsp = *(void**)_cc_dllst_object.avsp;            \
        free(avsp_bup);                                                    \
    }                                                                      \
                                                                           \
    _cc_dllst_object.tail = NULL;                                          \
    _cc_dllst_object.head = NULL;                                          \
    _cc_dllst_object.size = 0;                                             \
}



#endif
