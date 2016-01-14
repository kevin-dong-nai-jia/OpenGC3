#ifndef _CCC_DLLST_H_
#define _CCC_DLLST_H_

#include "sllst.h"
#include <stdint.h>
#include <string.h>



/* dllst container struct */


#define _cc_dllst_init {0, NULL, NULL}


#define cc_dllst(_cc_dllst_object,                                         \
                 _cc_dllst_element_type)                                   \
                                                                           \
struct                                                                     \
{                                                                          \
    int size;                                                              \
    void *head, *tail;                                                     \
}   _cc_dllst_object = _cc_dllst_init;                                     \
                                                                           \
typedef _cc_dllst_element_type _cc_dllst_object##_element_type             \
                                                                           \
/* ; cc_sllst(_##_cc_dllst_object##_avsp, _cc_dllst_element_type) */


#define _cc_dllst_node(_cc_dllst_object) _cc_xllst_node(_cc_dllst_object)



/* dllst container iterators */


#define _cc_dllst_iter_hptnt_init {NULL, NULL, NULL, NULL, NULL}


#define cc_dllst_iter(_cc_dllst_iter,                                      \
                      _cc_dllst_object)                                    \
                                                                           \
void **_##_cc_dllst_iter##_hptnt[5] = _cc_dllst_iter_hptnt_init;           \
                                                                           \
_cc_dllst_object##_element_type*** const                                   \
    _cc_dllst_iter = (void*)(&(_##_cc_dllst_iter##_hptnt[2]))


#define _cc_dllst_addr_xor(_cc_dllst_addr_a,                               \
                           _cc_dllst_addr_b)                               \
                                                                           \
(                                                                          \
    (void**)((uintptr_t)(void*)(_cc_dllst_addr_a) ^                        \
             (uintptr_t)(void*)(_cc_dllst_addr_b))                         \
)


#define cc_dllst_iter_copy(_cc_dllst_iter_dst,                             \
                           _cc_dllst_iter_src)                             \
{                                                                          \
    memcpy(_##_cc_dllst_iter_dst##_hptnt,                                  \
           _##_cc_dllst_iter_src##_hptnt,                                  \
           sizeof(_##_cc_dllst_iter_dst##_hptnt));                         \
}


#define cc_dllst_iter_begin(_cc_dllst_iter,                                \
                            _cc_dllst_object)                              \
(                                                                          \
    (_cc_dllst_object.head == NULL) ? (0) :                                \
    (                                                                      \
        _##_cc_dllst_iter##_hptnt[0] = &(_cc_dllst_object.head),           \
        _##_cc_dllst_iter##_hptnt[1] = &(_cc_dllst_object.head),           \
        _##_cc_dllst_iter##_hptnt[2] = _cc_dllst_object.head,              \
        _##_cc_dllst_iter##_hptnt[3] =                                     \
            _cc_dllst_addr_xor(&(_cc_dllst_object.head),                   \
                               *((void**)_cc_dllst_object.head - 1)) + 1,  \
        _##_cc_dllst_iter##_hptnt[4] = &(_cc_dllst_object.tail)            \
    )                                                                      \
)


#define cc_dllst_iter_end(_cc_dllst_iter,                                  \
                          _cc_dllst_object)                                \
(                                                                          \
    (_cc_dllst_object.tail == NULL) ? (0) :                                \
    (                                                                      \
        _##_cc_dllst_iter##_hptnt[4] = &(_cc_dllst_object.tail),           \
        _##_cc_dllst_iter##_hptnt[3] = &(_cc_dllst_object.tail),           \
        _##_cc_dllst_iter##_hptnt[2] = _cc_dllst_object.tail,              \
        _##_cc_dllst_iter##_hptnt[1] =                                     \
            _cc_dllst_addr_xor(&(_cc_dllst_object.tail),                   \
                               *((void**)_cc_dllst_object.tail - 1)) + 1,  \
        _##_cc_dllst_iter##_hptnt[0] = &(_cc_dllst_object.head)            \
    )                                                                      \
)


#define cc_dllst_iter_is_valid(_cc_dllst_iter)                             \
(                                                                          \
    _cc_dllst_iter != NULL        &&                                       \
    *(_cc_dllst_iter - 1) != NULL &&                                       \
    *(_cc_dllst_iter + 1) != NULL                                          \
)


#define cc_dllst_iter_incr(_cc_dllst_iter)                                 \
(                                                                          \
    (!(cc_dllst_iter_is_valid(_cc_dllst_iter))) ? (0) :                    \
    (                                                                      \
        _##_cc_dllst_iter##_hptnt[1] = _##_cc_dllst_iter##_hptnt[2],       \
        _##_cc_dllst_iter##_hptnt[2] = _##_cc_dllst_iter##_hptnt[3],       \
        _##_cc_dllst_iter##_hptnt[3] =                                     \
        (                                                                  \
            (_##_cc_dllst_iter##_hptnt[2] ==                               \
             _##_cc_dllst_iter##_hptnt[4] + 1) ? (NULL) :                  \
            (_cc_dllst_addr_xor(&(*(_##_cc_dllst_iter##_hptnt[1] - 1)),    \
                                *(_##_cc_dllst_iter##_hptnt[2] - 1)) + 1)  \
        )                                                                  \
    )                                                                      \
)


#define cc_dllst_iter_decr(_cc_dllst_iter)                                 \
(                                                                          \
    (!(cc_dllst_iter_is_valid(_cc_dllst_iter))) ? (0) :                    \
    (                                                                      \
        _##_cc_dllst_iter##_hptnt[3] = _##_cc_dllst_iter##_hptnt[2],       \
        _##_cc_dllst_iter##_hptnt[2] = _##_cc_dllst_iter##_hptnt[1],       \
        _##_cc_dllst_iter##_hptnt[1] =                                     \
        (                                                                  \
            (_##_cc_dllst_iter##_hptnt[2] ==                               \
             _##_cc_dllst_iter##_hptnt[0] + 1) ? (NULL) :                  \
            (_cc_dllst_addr_xor(&(*(_##_cc_dllst_iter##_hptnt[3] - 1)),    \
                                *(_##_cc_dllst_iter##_hptnt[2] - 1)) + 1)  \
        )                                                                  \
    )                                                                      \
)



/* dllst container traversal */


#define cc_dllst_trav(_cc_dllst_iter,                                      \
                      _cc_dllst_object)                                    \
                                                                           \
for                                                                        \
(                                                                          \
    cc_dllst_iter_begin(_cc_dllst_iter, _cc_dllst_object);                 \
    cc_dllst_iter_is_valid(_cc_dllst_iter);                                \
    cc_dllst_iter_incr(_cc_dllst_iter)                                     \
)


#define cc_dllst_trav_rev(_cc_dllst_iter,                                  \
                          _cc_dllst_object)                                \
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



/* dllst container modifiers */


#define cc_dllst_push_front(_cc_dllst_object,                              \
                            _cc_dllst_push_front_value)                    \
{                                                                          \
    _cc_dllst_node(_cc_dllst_object) *new_node =                           \
        malloc(sizeof(_cc_dllst_node(_cc_dllst_object)));                  \
                                                                           \
    new_node->val    = _cc_dllst_push_front_value;                         \
    new_node->xrt[1] = &(new_node->val);                                   \
                                                                           \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
        new_node->xrt[0] = _cc_dllst_addr_xor(&(_cc_dllst_object.head),    \
                                              &(_cc_dllst_object.tail)),   \
        _cc_dllst_object.tail = &(new_node->xrt[1]);                       \
    else                                                                   \
        new_node->xrt[0] =                                                 \
            _cc_dllst_addr_xor(&(_cc_dllst_object.head),                   \
                               &(*((void**)_cc_dllst_object.head - 1))),   \
        *((void**)_cc_dllst_object.head - 1) =                             \
            _cc_dllst_addr_xor(*((void**)_cc_dllst_object.head - 1),       \
                               _cc_dllst_addr_xor(&(_cc_dllst_object.head),\
                                                  &(new_node->xrt[0])));   \
                                                                           \
    _cc_dllst_object.head = &(new_node->xrt[1]);                           \
    _cc_dllst_object.size++;                                               \
}


#define cc_dllst_push_back(_cc_dllst_object,                               \
                           _cc_dllst_push_back_value)                      \
{                                                                          \
    _cc_dllst_node(_cc_dllst_object) *new_node =                           \
        malloc(sizeof(_cc_dllst_node(_cc_dllst_object)));                  \
                                                                           \
    new_node->val    = _cc_dllst_push_back_value;                          \
    new_node->xrt[1] = &(new_node->val);                                   \
                                                                           \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
        new_node->xrt[0] = _cc_dllst_addr_xor(&(_cc_dllst_object.head),    \
                                              &(_cc_dllst_object.tail)),   \
        _cc_dllst_object.head = &(new_node->xrt[1]);                       \
    else                                                                   \
        new_node->xrt[0] =                                                 \
            _cc_dllst_addr_xor(&(_cc_dllst_object.tail),                   \
                               &(*((void**)_cc_dllst_object.tail - 1))),   \
        *((void**)_cc_dllst_object.tail - 1) =                             \
            _cc_dllst_addr_xor(*((void**)_cc_dllst_object.tail - 1),       \
                               _cc_dllst_addr_xor(&(_cc_dllst_object.tail),\
                                                  &(new_node->xrt[0])));   \
                                                                           \
    _cc_dllst_object.tail = &(new_node->xrt[1]);                           \
    _cc_dllst_object.size++;                                               \
}


#define cc_dllst_pop_front(_cc_dllst_object)                               \
    /* TODO */


#define cc_dllst_pop_back(_cc_dllst_object)                                \
    /* TODO */


#define cc_dllst_clear(_cc_dllst_object)                                   \
    /* TODO */



/* dllst container deallocation */


#define cc_dllst_dealloc(_cc_dllst_object)                                 \
{                                                                          \
    cc_dllst_iter(iter, _cc_dllst_object);                                 \
    cc_dllst_iter_begin(iter, _cc_dllst_object);                           \
                                                                           \
    if (cc_dllst_size(_cc_dllst_object) >= 1)                              \
        do                                                                 \
        {                                                                  \
            cc_dllst_iter_incr(iter);                                      \
            free(&(*(*(iter - 1) - 1)));                                   \
        }                                                                  \
        while (cc_dllst_iter_is_valid(iter));                              \
                                                                           \
    _cc_dllst_object.tail = NULL;                                          \
    _cc_dllst_object.head = NULL;                                          \
    _cc_dllst_object.size = 0;                                             \
}



#endif
