#ifndef _CCC_DLLST_H_
#define _CCC_DLLST_H_

#include "debug.h"

#include <stdint.h>
#include <string.h>



/* dllst container */


#define _cc_dllst_init {0, NULL, NULL, NULL}


#define cc_dllst(_cc_dllst_object,                                         \
                 _cc_dllst_element_type)                                   \
                                                                           \
struct                                                                     \
{                                                                          \
    int size;                                                              \
    void *head, *tail, *avsp;                                              \
}   _cc_dllst_object = _cc_dllst_init;                                     \
                                                                           \
typedef _cc_dllst_element_type _cc_dllst_object##_element_type


#define _cc_dllst_node(_cc_dllst_object)                                   \
                                                                           \
struct                                                                     \
{                                                                          \
    void *lnk[2];                                                          \
    _cc_dllst_object##_element_type val;                                   \
}



/* dllst container iterators */


#define _cc_dllst_iter_hptnt_init {NULL, NULL, NULL, NULL, NULL}


#define cc_dllst_iter(_cc_dllst_iter,                                      \
                      _cc_dllst_object)                                    \
                                                                           \
void **_##_cc_dllst_iter##_hptnt[5] = _cc_dllst_iter_hptnt_init;           \
                                                                           \
typedef _cc_dllst_object##_element_type _cc_dllst_iter##_element_type;     \
                                                                           \
_cc_dllst_iter##_element_type*** const                                     \
    _cc_dllst_iter = (void*)(&(_##_cc_dllst_iter##_hptnt[2]))


#define cc_dllst_iter_copy(_cc_dllst_iter_dst,                             \
                           _cc_dllst_iter_src)                             \
{                                                                          \
    memcpy(_##_cc_dllst_iter_dst##_hptnt,                                  \
           _##_cc_dllst_iter_src##_hptnt,                                  \
           sizeof(_##_cc_dllst_iter_dst##_hptnt))                          \
}


#define cc_dllst_iter_clear(_cc_dllst_iter)                                \
{                                                                          \
    memset(_##_cc_dllst_iter##_hptnt, 0,                                   \
           sizeof(_##_cc_dllst_iter##_hptnt))                              \
}


#define cc_dllst_iter_is_valid(_cc_dllst_iter)                             \
(                                                                          \
    _cc_dllst_iter != NULL        &&                                       \
    *(_cc_dllst_iter - 1) != NULL &&                                       \
    *(_cc_dllst_iter + 1) != NULL                                          \
)


#define cc_dllst_iter_dereference(_cc_dllst_iter)                          \
(                                                                          \
 ***(                                                                      \
        (!(cc_dllst_iter_is_valid(_cc_dllst_iter))) ?                      \
        (                                                                  \
            (_cc_dllst_iter##_element_type***)                             \
               __cc_error_dllst_iter_cannot_be_dereferenced(_cc_dllst_iter)\
        ) :                                                                \
        (                                                                  \
            _cc_dllst_iter                                                 \
        )                                                                  \
    )                                                                      \
)


#define cc_dref(_cc_dllst_iter)                                            \
(                                                                          \
    cc_dllst_iter_dereference(_cc_dllst_iter)                              \
)


#define _cc_dllst_addr_xor(_cc_dllst_addr_a,                               \
                           _cc_dllst_addr_b)                               \
                                                                           \
(                                                                          \
    (void**)((uintptr_t)(void*)(_cc_dllst_addr_a) ^                        \
             (uintptr_t)(void*)(_cc_dllst_addr_b))                         \
)


#define cc_dllst_iter_begin(_cc_dllst_iter,                                \
                            _cc_dllst_object)                              \
(                                                                          \
    (_cc_dllst_object.head == NULL) ?                                      \
    (                                                                      \
        (_cc_dllst_iter##_element_type***)                                 \
            __cc_warning_dllst_is_empty(_cc_dllst_object)                  \
    ) :                                                                    \
    (                                                                      \
        _##_cc_dllst_iter##_hptnt[0] = &(_cc_dllst_object.head),           \
        _##_cc_dllst_iter##_hptnt[1] = &(_cc_dllst_object.head),           \
        _##_cc_dllst_iter##_hptnt[2] = _cc_dllst_object.head,              \
        _##_cc_dllst_iter##_hptnt[3] =                                     \
            _cc_dllst_addr_xor(&(_cc_dllst_object.head),                   \
                               *((void**)_cc_dllst_object.head - 1)) + 1,  \
        _##_cc_dllst_iter##_hptnt[4] = &(_cc_dllst_object.tail),           \
        _cc_dllst_iter                                                     \
    )                                                                      \
)


#define cc_dllst_iter_end(_cc_dllst_iter,                                  \
                          _cc_dllst_object)                                \
(                                                                          \
    (_cc_dllst_object.tail == NULL) ?                                      \
    (                                                                      \
        (_cc_dllst_iter##_element_type***)                                 \
            __cc_warning_dllst_is_empty(_cc_dllst_object)                  \
    ) :                                                                    \
    (                                                                      \
        _##_cc_dllst_iter##_hptnt[4] = &(_cc_dllst_object.tail),           \
        _##_cc_dllst_iter##_hptnt[3] = &(_cc_dllst_object.tail),           \
        _##_cc_dllst_iter##_hptnt[2] = _cc_dllst_object.tail,              \
        _##_cc_dllst_iter##_hptnt[1] =                                     \
            _cc_dllst_addr_xor(&(_cc_dllst_object.tail),                   \
                               *((void**)_cc_dllst_object.tail - 1)) + 1,  \
        _##_cc_dllst_iter##_hptnt[0] = &(_cc_dllst_object.head),           \
        _cc_dllst_iter                                                     \
    )                                                                      \
)


#define cc_dllst_iter_incr(_cc_dllst_iter)                                 \
(                                                                          \
    (!(cc_dllst_iter_is_valid(_cc_dllst_iter))) ?                          \
    (                                                                      \
        (_cc_dllst_iter##_element_type***)                                 \
            __cc_warning_dllst_iter_is_invalid(_cc_dllst_iter)             \
    ) :                                                                    \
    (                                                                      \
        _##_cc_dllst_iter##_hptnt[1] = _##_cc_dllst_iter##_hptnt[2],       \
        _##_cc_dllst_iter##_hptnt[2] = _##_cc_dllst_iter##_hptnt[3],       \
        _##_cc_dllst_iter##_hptnt[3] =                                     \
        (                                                                  \
            (_##_cc_dllst_iter##_hptnt[2] ==                               \
             _##_cc_dllst_iter##_hptnt[4] + 1) ? (NULL) :                  \
            (_cc_dllst_addr_xor(&(*(_##_cc_dllst_iter##_hptnt[1] - 1)),    \
                                *(_##_cc_dllst_iter##_hptnt[2] - 1)) + 1)  \
        ),                                                                 \
        _cc_dllst_iter                                                     \
    )                                                                      \
)


#define cc_dllst_iter_decr(_cc_dllst_iter)                                 \
(                                                                          \
    (!(cc_dllst_iter_is_valid(_cc_dllst_iter))) ?                          \
    (                                                                      \
        (_cc_dllst_iter##_element_type***)                                 \
            __cc_warning_dllst_iter_is_invalid(_cc_dllst_iter)             \
    ) :                                                                    \
    (                                                                      \
        _##_cc_dllst_iter##_hptnt[3] = _##_cc_dllst_iter##_hptnt[2],       \
        _##_cc_dllst_iter##_hptnt[2] = _##_cc_dllst_iter##_hptnt[1],       \
        _##_cc_dllst_iter##_hptnt[1] =                                     \
        (                                                                  \
            (_##_cc_dllst_iter##_hptnt[2] ==                               \
             _##_cc_dllst_iter##_hptnt[0] + 1) ? (NULL) :                  \
            (_cc_dllst_addr_xor(&(*(_##_cc_dllst_iter##_hptnt[3] - 1)),    \
                                *(_##_cc_dllst_iter##_hptnt[2] - 1)) + 1)  \
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
    cc_dllst_iter_is_valid(_cc_dllst_iter);                                \
    cc_dllst_iter_incr(_cc_dllst_iter)                                     \
)


#define cc_dllst_trav_rev(_cc_dllst_object,                                \
                          _cc_dllst_iter)                                  \
                                                                           \
for                                                                        \
(                                                                          \
    cc_dllst_iter_end(_cc_dllst_iter, _cc_dllst_object);                   \
    cc_dllst_iter_is_valid(_cc_dllst_iter);                                \
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


#define cc_dllst_resize(_cc_dllst_object,                                  \
                        _cc_dllst_padding_value)                           \
    /* TODO */



/* dllst element access */


#define cc_dllst_front(_cc_dllst_object)                                   \
(                                                                          \
  **(                                                                      \
        (cc_dllst_empty(_cc_dllst_object)) ?                               \
        (                                                                  \
            (_cc_dllst_object##_element_type**)                            \
                __cc_warning_dllst_is_empty(_cc_dllst_object)              \
        ) :                                                                \
        (                                                                  \
            (_cc_dllst_object##_element_type**)_cc_dllst_object.head       \
        )                                                                  \
    )                                                                      \
)


#define cc_dllst_back(_cc_dllst_object)                                    \
(                                                                          \
  **(                                                                      \
        (cc_dllst_empty(_cc_dllst_object)) ?                               \
        (                                                                  \
            (_cc_dllst_object##_element_type**)                            \
                __cc_warning_dllst_is_empty(_cc_dllst_object)              \
        ) :                                                                \
        (                                                                  \
            (_cc_dllst_object##_element_type**)_cc_dllst_object.tail       \
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
        new_node = malloc(sizeof(_cc_dllst_node(_cc_dllst_object)));       \
    else                                                                   \
    {                                                                      \
        new_node = _cc_dllst_object.avsp;                                  \
        _cc_dllst_object.avsp = *((void**)_cc_dllst_object.avsp + 1);      \
    }                                                                      \
                                                                           \
    new_node->val    = _cc_dllst_push_front_value;                         \
    new_node->lnk[1] = &(new_node->val);                                   \
                                                                           \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
        new_node->lnk[0] = _cc_dllst_addr_xor(&(_cc_dllst_object.head),    \
                                              &(_cc_dllst_object.tail)),   \
        _cc_dllst_object.tail = &(new_node->lnk[1]);                       \
    else                                                                   \
        new_node->lnk[0] =                                                 \
            _cc_dllst_addr_xor(&(_cc_dllst_object.head),                   \
                               &(*((void**)_cc_dllst_object.head - 1))),   \
        *((void**)_cc_dllst_object.head - 1) =                             \
            _cc_dllst_addr_xor(*((void**)_cc_dllst_object.head - 1),       \
                               _cc_dllst_addr_xor(&(_cc_dllst_object.head),\
                                                  &(new_node->lnk[0])));   \
                                                                           \
    _cc_dllst_object.head = &(new_node->lnk[1]);                           \
    _cc_dllst_object.size++;                                               \
}


#define cc_dllst_push_back(_cc_dllst_object,                               \
                           _cc_dllst_push_back_value)                      \
{                                                                          \
    _cc_dllst_node(_cc_dllst_object) *new_node;                            \
                                                                           \
    if (_cc_dllst_object.avsp == NULL)                                     \
        new_node = malloc(sizeof(_cc_dllst_node(_cc_dllst_object)));       \
    else                                                                   \
    {                                                                      \
        new_node = _cc_dllst_object.avsp;                                  \
        _cc_dllst_object.avsp = *((void**)_cc_dllst_object.avsp + 1);      \
    }                                                                      \
                                                                           \
    new_node->val    = _cc_dllst_push_back_value;                          \
    new_node->lnk[1] = &(new_node->val);                                   \
                                                                           \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
        new_node->lnk[0] = _cc_dllst_addr_xor(&(_cc_dllst_object.head),    \
                                              &(_cc_dllst_object.tail)),   \
        _cc_dllst_object.head = &(new_node->lnk[1]);                       \
    else                                                                   \
        new_node->lnk[0] =                                                 \
            _cc_dllst_addr_xor(&(_cc_dllst_object.tail),                   \
                               &(*((void**)_cc_dllst_object.tail - 1))),   \
        *((void**)_cc_dllst_object.tail - 1) =                             \
            _cc_dllst_addr_xor(*((void**)_cc_dllst_object.tail - 1),       \
                               _cc_dllst_addr_xor(&(_cc_dllst_object.tail),\
                                                  &(new_node->lnk[0])));   \
                                                                           \
    _cc_dllst_object.tail = &(new_node->lnk[1]);                           \
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
        void *begin = (void**)_cc_dllst_object.head - 1;                   \
                                                                           \
        if (cc_dllst_size(_cc_dllst_object) == 1)                          \
        {                                                                  \
            _cc_dllst_object.head = NULL;                                  \
            _cc_dllst_object.tail = NULL;                                  \
        }                                                                  \
        else                                                               \
        {                                                                  \
            _cc_dllst_object.head =                                        \
                _cc_dllst_addr_xor(*((void**)_cc_dllst_object.head - 1),   \
                                   &(_cc_dllst_object.head)) + 1;          \
                                                                           \
            *((void**)_cc_dllst_object.head - 1) =                         \
                _cc_dllst_addr_xor(*((void**)_cc_dllst_object.head - 1),   \
                    _cc_dllst_addr_xor(&(_cc_dllst_object.head), begin));  \
        }                                                                  \
                                                                           \
        *((void**)begin + 1) = _cc_dllst_object.avsp;                      \
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
        void *end = (void**)_cc_dllst_object.tail - 1;                     \
                                                                           \
        if (cc_dllst_size(_cc_dllst_object) == 1)                          \
        {                                                                  \
            _cc_dllst_object.head = NULL;                                  \
            _cc_dllst_object.tail = NULL;                                  \
        }                                                                  \
        else                                                               \
        {                                                                  \
            _cc_dllst_object.tail =                                        \
                _cc_dllst_addr_xor(*((void**)_cc_dllst_object.tail - 1),   \
                                   &(_cc_dllst_object.tail)) + 1;          \
                                                                           \
            *((void**)_cc_dllst_object.tail - 1) =                         \
                _cc_dllst_addr_xor(*((void**)_cc_dllst_object.tail - 1),   \
                    _cc_dllst_addr_xor(&(_cc_dllst_object.tail), end));    \
        }                                                                  \
                                                                           \
        *((void**)end + 1) = _cc_dllst_object.avsp;                        \
        _cc_dllst_object.avsp = end;                                       \
        _cc_dllst_object.size--;                                           \
    }                                                                      \
}


#define cc_dllst_clear(_cc_dllst_object)                                   \
{                                                                          \
    do                                                                     \
    {                                                                      \
        cc_dllst_pop_back(_cc_dllst_object);                               \
    }                                                                      \
    while (!(cc_dllst_empty(_cc_dllst_object)));                           \
}



/* dllst container deallocation */


#define cc_dllst_dealloc(_cc_dllst_object)                                 \
{                                                                          \
    if (cc_dllst_size(_cc_dllst_object) >= 1)                              \
    {                                                                      \
        cc_dllst_iter(iter, _cc_dllst_object);                             \
        cc_dllst_iter_begin(iter, _cc_dllst_object);                       \
                                                                           \
        do                                                                 \
        {                                                                  \
            cc_dllst_iter_incr(iter);                                      \
                                                                           \
            free(&(*(*(iter - 1) - 1)));                                   \
        }                                                                  \
        while (cc_dllst_iter_is_valid(iter));                              \
    }                                                                      \
                                                                           \
    while (_cc_dllst_object.avsp != NULL)                                  \
    {                                                                      \
        void *avsp_bup = _cc_dllst_object.avsp;                            \
                                                                           \
        _cc_dllst_object.avsp = *((void**)_cc_dllst_object.avsp + 1);      \
                                                                           \
        free(avsp_bup);                                                    \
    }                                                                      \
                                                                           \
    _cc_dllst_object.tail = NULL;                                          \
    _cc_dllst_object.head = NULL;                                          \
    _cc_dllst_object.size = 0;                                             \
}



#endif
