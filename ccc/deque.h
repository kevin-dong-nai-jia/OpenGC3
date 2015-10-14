#ifndef _CCC_DEQUE_H_
#define _CCC_DEQUE_H_

#include <stdio.h>
#include <stdlib.h>



/* deque container struct */


#define cc_deque(_cc_deque_element_type)                                   \
                                                                           \
struct                                                                     \
{                                                                          \
    _cc_deque_element_type **head;                                         \
    _cc_deque_element_type **tail;                                         \
                                                                           \
    int size;                                                              \
}


#define cc_deque_init {NULL, NULL, 0}


#define _cc_deque_node(_cc_deque_element_type)                             \
                                                                           \
struct                                                                     \
{                                                                          \
    _cc_deque_element_type val;                                            \
                                                                           \
    void *ptni[4];                                                         \
}

/* NOTE: ptni: p stands for prev   */
/*             t stands for this   */
/*             n stands for next   */
/*             i stands for itself */



/* deque container iterators */


#define cc_deque_iter(_cc_deque_element_type) _cc_deque_element_type**


#define cc_deque_iter_init NULL


#define cc_deque_iter_begin(_cc_deque_object)                              \
(                                                                          \
    ((_cc_deque_object).head == NULL) ?                                    \
    (NULL) :                                                               \
    ((_cc_deque_object).head + 1)                                          \
)


#define cc_deque_iter_end(_cc_deque_object)                                \
(                                                                          \
    ((_cc_deque_object).tail == NULL) ?                                    \
    (NULL) :                                                               \
    ((_cc_deque_object).tail - 1)                                          \
)


#define cc_deque_iter_is_valid(_cc_deque_iter_object)                      \
(                                                                          \
    (_cc_deque_iter_object) != NULL && *(_cc_deque_iter_object) != NULL    \
)


#define cc_deque_iter_next(_cc_deque_iter_object,                          \
                           _cc_deque_element_type)                         \
(                                                                          \
    (*((_cc_deque_iter_object) + 1) == NULL) ?                             \
    (NULL) :                                                               \
    ((_cc_deque_element_type**)(*((_cc_deque_iter_object) + 1)) + 1)       \
)


#define cc_deque_iter_incr(_cc_deque_iter_object,                          \
                           _cc_deque_element_type)                         \
(                                                                          \
    (*((_cc_deque_iter_object) + 1) == NULL) ?                             \
    ((_cc_deque_iter_object) = NULL) :                                     \
    ((_cc_deque_iter_object) =                                             \
        ((_cc_deque_element_type**)(*((_cc_deque_iter_object) + 1)) + 1))  \
)


#define cc_deque_iter_incr_prefix(_cc_deque_iter_object,                   \
                                  _cc_deque_element_type)                  \
(                                                                          \
    cc_deque_iter_incr((_cc_deque_iter_object), _cc_deque_element_type),   \
    (_cc_deque_iter_object)                                                \
)


#define cc_deque_iter_incr_postfix(_cc_deque_iter_object,                  \
                                   _cc_deque_element_type)                 \
                                                                           \
    /* RESERVED, NOT IMPLEMENTED */



/* deque container traversal */


#define cc_deque_traversal(_cc_deque_object,                               \
                           _cc_deque_iter_object,                          \
                           _cc_deque_element_type)                         \
                                                                           \
for                                                                        \
(                                                                          \
    (_cc_deque_iter_object) = cc_deque_iter_begin((_cc_deque_object));     \
    cc_deque_iter_is_valid((_cc_deque_iter_object));                       \
    cc_deque_iter_incr((_cc_deque_iter_object), _cc_deque_element_type)    \
)



/* deque container capacity */


#define cc_deque_size(_cc_deque_object)                                    \
(                                                                          \
    (_cc_deque_object).size                                                \
)


#define cc_deque_empty(_cc_deque_object)                                   \
(                                                                          \
    cc_deque_size((_cc_deque_object)) == 0                                 \
)



/* deque container modifiers */


#define cc_deque_push_back(_cc_deque_object,                               \
                           _cc_deque_element_type,                         \
                           _cc_deque_push_back_value)                      \
                                                                           \
{                                                                          \
    _cc_deque_node(_cc_deque_element_type) *new_node =                     \
        malloc(sizeof(_cc_deque_node(_cc_deque_element_type)));            \
                                                                           \
    new_node->val     = _cc_deque_push_back_value;                         \
    new_node->ptni[1] = &new_node->val;                                    \
    new_node->ptni[3] = new_node;                                          \
                                                                           \
    if (cc_deque_empty((_cc_deque_object)))                                \
        new_node->ptni[0] = new_node->ptni[2] = NULL,                      \
        (_cc_deque_object).head =                                          \
            (_cc_deque_element_type**)(&(new_node->ptni[0]));              \
    else                                                                   \
        *(_cc_deque_object).tail = (void*)(&(new_node->ptni[0])),          \
        new_node->ptni[0] = (void*)(&(*(_cc_deque_object).tail));          \
                                                                           \
    (_cc_deque_object).tail =                                              \
        (_cc_deque_element_type**)(&(new_node->ptni[2]));                  \
                                                                           \
    (_cc_deque_object).size++;                                             \
}


#define cc_deque_push_front(_cc_deque_object,                              \
                            _cc_deque_element_type,                        \
                            _cc_deque_push_front_value)                    \
                                                                           \
{                                                                          \
    _cc_deque_node(_cc_deque_element_type) *new_node =                     \
        malloc(sizeof(_cc_deque_node(_cc_deque_element_type)));            \
                                                                           \
    new_node->val     = _cc_deque_push_front_value;                        \
    new_node->ptni[1] = &new_node->val;                                    \
    new_node->ptni[3] = new_node;                                          \
                                                                           \
    if (cc_deque_empty((_cc_deque_object)))                                \
        new_node->ptni[0] = new_node->ptni[2] = NULL,                      \
        (_cc_deque_object).tail =                                          \
            (_cc_deque_element_type**)(&(new_node->ptni[2]));              \
    else                                                                   \
        *(_cc_deque_object).head = (void*)(&(new_node->ptni[2])),          \
        new_node->ptni[2] = (void*)(&(*(_cc_deque_object).head));          \
                                                                           \
    (_cc_deque_object).head =                                              \
        (_cc_deque_element_type**)(&(new_node->ptni[0]));                  \
                                                                           \
    (_cc_deque_object).size++;                                             \
}



/* deque container deallocation */


#define cc_deque_dealloc(_cc_deque_object,                                 \
                         _cc_deque_element_type)                           \
{                                                                          \
    cc_deque_iter(_cc_deque_element_type) iter =                           \
        cc_deque_iter_begin((_cc_deque_object));                           \
                                                                           \
    while (cc_deque_iter_is_valid(iter))                                   \
    {                                                                      \
        cc_deque_iter(_cc_deque_element_type) iter_dup = iter;             \
        cc_deque_iter_incr(iter, _cc_deque_element_type);                  \
        free(*(iter_dup + 2));                                             \
    }                                                                      \
                                                                           \
    (_cc_deque_object).tail = (_cc_deque_object).head = NULL;              \
}



#endif
