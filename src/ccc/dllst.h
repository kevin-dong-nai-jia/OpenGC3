#ifndef _CCC_DLLST_H_
#define _CCC_DLLST_H_

#include "sllst.h"



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
typedef _cc_dllst_element_type _cc_dllst_object##_element_type;            \
                                                                           \
cc_sllst(_##_cc_dllst_object##_avsp, _cc_dllst_element_type)


#define _cc_dllst_node(_cc_dllst_object) _cc_xllst_node(_cc_dllst_object)



/* dllst container iterators */


#define cc_dllst_iter(_cc_dllst_iter,                                      \
                      _cc_dllst_object)                                    \
                                                                           \
_cc_dllst_object##_element_type** _cc_dllst_iter = NULL


#define cc_dllst_iter_begin(_cc_dllst_object)                              \
(                                                                          \
    (_cc_dllst_object.head == NULL) ?                                      \
    (NULL) :                                                               \
    ((_cc_dllst_object##_element_type**)_cc_dllst_object.head + 1)         \
)


#define cc_dllst_iter_end(_cc_dllst_object)                                \
(                                                                          \
    (_cc_dllst_object.tail == NULL) ?                                      \
    (NULL) :                                                               \
    ((_cc_dllst_object##_element_type**)_cc_dllst_object.tail - 1)         \
)


#define cc_dllst_iter_is_valid(_cc_dllst_iter)                             \
(                                                                          \
    _cc_dllst_iter != NULL && *_cc_dllst_iter != NULL                      \
)


#define cc_dllst_iter_incr(_cc_dllst_iter)                                 \
(                                                                          \
    (*(_cc_dllst_iter + 1) == NULL) ?                                      \
    (_cc_dllst_iter = NULL) :                                              \
    (_cc_dllst_iter = (void*)(*((void***)_cc_dllst_iter + 1) + 1))         \
)


#define cc_dllst_iter_decr(_cc_dllst_iter)                                 \
(                                                                          \
    (*(_cc_dllst_iter - 1) == NULL) ?                                      \
    (_cc_dllst_iter = NULL) :                                              \
    (_cc_dllst_iter = (void*)(*((void***)_cc_dllst_iter - 1) - 1))         \
)



/* dllst container traversal */


#define cc_dllst_trav(_cc_dllst_iter,                                      \
                      _cc_dllst_object)                                    \
                                                                           \
for                                                                        \
(                                                                          \
    _cc_dllst_iter = cc_dllst_iter_begin(_cc_dllst_object);                \
    cc_dllst_iter_is_valid(_cc_dllst_iter);                                \
    cc_dllst_iter_incr(_cc_dllst_iter)                                     \
)


#define cc_dllst_revtrav(_cc_dllst_iter,                                   \
                         _cc_dllst_object)                                 \
                                                                           \
for                                                                        \
(                                                                          \
    _cc_dllst_iter = cc_dllst_iter_end(_cc_dllst_object);                  \
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


#define cc_dllst_push_back(_cc_dllst_object,                               \
                           _cc_dllst_push_back_value)                      \
{                                                                          \
    _cc_dllst_node(_cc_dllst_object) *new_node =                           \
        malloc(sizeof(_cc_dllst_node(_cc_dllst_object)));                  \
                                                                           \
    new_node->val    = _cc_dllst_push_back_value;                          \
    new_node->ptn[0] = NULL;                                               \
    new_node->ptn[1] = &(new_node->val);                                   \
    new_node->ptn[2] = NULL;                                               \
                                                                           \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
        _cc_dllst_object.head = &(new_node->ptn[0]);                       \
    else                                                                   \
        new_node->ptn[0] = _cc_dllst_object.tail,                          \
        *(void**)(_cc_dllst_object.tail) = &(new_node->ptn[0]);            \
                                                                           \
    _cc_dllst_object.tail = &(new_node->ptn[2]);                           \
    _cc_dllst_object.size++;                                               \
}


#define cc_dllst_push_front(_cc_dllst_object,                              \
                            _cc_dllst_push_front_value)                    \
{                                                                          \
    _cc_dllst_node(_cc_dllst_object) *new_node =                           \
        malloc(sizeof(_cc_dllst_node(_cc_dllst_object)));                  \
                                                                           \
    new_node->val    = _cc_dllst_push_front_value;                         \
    new_node->ptn[0] = NULL;                                               \
    new_node->ptn[1] = &(new_node->val);                                   \
    new_node->ptn[2] = NULL;                                               \
                                                                           \
    if (cc_dllst_empty(_cc_dllst_object))                                  \
        _cc_dllst_object.tail = &(new_node->ptn[2]);                       \
    else                                                                   \
        new_node->ptn[2] = _cc_dllst_object.head,                          \
        *(void**)(_cc_dllst_object.head) = &(new_node->ptn[2]);            \
                                                                           \
    _cc_dllst_object.head = &(new_node->ptn[0]);                           \
    _cc_dllst_object.size++;                                               \
}


#define cc_dllst_pop_back(_cc_dllst_object)                                \
    /* TODO */


#define cc_dllst_pop_front(_cc_dllst_object)                               \
    /* TODO */


#define cc_dllst_clear(_cc_dllst_object)                                   \
    /* TODO */



/* dllst container deallocation */


#define cc_dllst_dealloc(_cc_dllst_object)                                 \
{                                                                          \
    cc_dllst_iter(iter, _cc_dllst_object);                                 \
    cc_dllst_iter(iter_dup, _cc_dllst_object);                             \
    iter = cc_dllst_iter_begin(_cc_dllst_object);                          \
                                                                           \
    while (cc_dllst_iter_is_valid(iter))                                   \
    {                                                                      \
        iter_dup = iter;                                                   \
        cc_dllst_iter_incr(iter);                                          \
        free(&(*(iter_dup - 1)));                                          \
    }                                                                      \
                                                                           \
    _cc_dllst_object.tail = NULL;                                          \
    _cc_dllst_object.head = NULL;                                          \
    _cc_dllst_object.size = 0;                                             \
}



#endif
