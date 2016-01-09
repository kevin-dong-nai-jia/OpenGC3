#ifndef _CCC_DEQUE_H_
#define _CCC_DEQUE_H_



/* deque container struct */


#define _cc_flag_deque    0
#define _cc_flag_avs_list 1

#define cc_deque(_cc_deque_object,                                         \
                 _cc_deque_element_type)                                   \
                                                                           \
typedef _cc_deque_element_type _cc_deque_object##_element_type;            \
                                                                           \
struct                                                                     \
{                                                                          \
    int flag, size;                                                        \
    void **head, **tail;                                                   \
                                                                           \
    struct                                                                 \
    {                                                                      \
        int flag, size;                                                    \
        void **head, **tail;                                               \
    }   avs_list;                                                          \
                                                                           \
}   _cc_deque_object = {_cc_flag_deque, 0, NULL, NULL,                     \
                        {_cc_flag_avs_list, 0, NULL, NULL}}


#define _cc_deque_node(_cc_deque_object)                                   \
                                                                           \
struct                                                                     \
{                                                                          \
    _cc_deque_object##_element_type val;                                   \
                                                                           \
    void *pitn[4];                                                         \
}



/* deque container iterators */


#define cc_deque_iter(_cc_deque_iter,                                      \
                      _cc_deque_object)                                    \
                                                                           \
typedef _cc_deque_object##_element_type _cc_deque_iter##_element_type;     \
                                                                           \
_cc_deque_object##_element_type** _cc_deque_iter = NULL


#define _cc_deque_iter_no_typedef(_cc_deque_iter,                          \
                                  _cc_deque_object)                        \
                                                                           \
_cc_deque_object##_element_type** _cc_deque_iter = NULL


#define cc_deque_iter_begin(_cc_deque_object)                              \
(                                                                          \
    ((_cc_deque_object).head == NULL) ?                                    \
    (NULL) :                                                               \
    ((_cc_deque_object##_element_type**)(_cc_deque_object).head + 2)       \
)


#define cc_deque_iter_end(_cc_deque_object)                                \
(                                                                          \
    ((_cc_deque_object).tail == NULL) ?                                    \
    (NULL) :                                                               \
    ((_cc_deque_object##_element_type**)(_cc_deque_object).tail - 1)       \
)


#define cc_deque_iter_is_valid(_cc_deque_iter)                             \
(                                                                          \
    (_cc_deque_iter) != NULL && *(_cc_deque_iter) != NULL                  \
)


#define cc_deque_iter_incr(_cc_deque_iter)                                 \
(                                                                          \
    (*((_cc_deque_iter) + 1) == NULL) ?                                    \
    ((_cc_deque_iter) = NULL) :                                            \
    ((_cc_deque_iter) =                                                    \
        ((_cc_deque_iter##_element_type**)(*((_cc_deque_iter) + 1)) + 2))  \
)


#define cc_deque_iter_incr_prefix(_cc_deque_iter)                          \
(                                                                          \
    cc_deque_iter_incr((_cc_deque_iter)),                                  \
    (_cc_deque_iter)                                                       \
)


#define cc_deque_iter_decr(_cc_deque_iter)                                 \
(                                                                          \
    (*((_cc_deque_iter) - 2) == NULL) ?                                    \
    ((_cc_deque_iter) = NULL) :                                            \
    ((_cc_deque_iter) =                                                    \
        ((_cc_deque_iter##_element_type**)(*((_cc_deque_iter) - 2)) - 1))  \
)


#define cc_deque_iter_decr_prefix(_cc_deque_iter)                          \
(                                                                          \
    cc_deque_iter_decr((_cc_deque_iter)),                                  \
    (_cc_deque_iter)                                                       \
)



/* deque container traversal */


#define cc_deque_trav(_cc_deque_iter,                                      \
                      _cc_deque_object)                                    \
                                                                           \
for                                                                        \
(                                                                          \
    (_cc_deque_iter) = cc_deque_iter_begin(_cc_deque_object);              \
    cc_deque_iter_is_valid((_cc_deque_iter));                              \
    cc_deque_iter_incr(_cc_deque_iter)                                     \
)


#define cc_deque_rtrav(_cc_deque_iter,                                     \
                       _cc_deque_object)                                   \
                                                                           \
for                                                                        \
(                                                                          \
    (_cc_deque_iter) = cc_deque_iter_end(_cc_deque_object);                \
    cc_deque_iter_is_valid((_cc_deque_iter));                              \
    cc_deque_iter_decr(_cc_deque_iter)                                     \
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


#define cc_deque_resize(_cc_deque_object,                                  \
                        _cc_deque_padding_value)                           \
    /* TODO */


/* deque container modifiers */


#define cc_deque_push_back(_cc_deque_object,                               \
                           _cc_deque_push_back_value)                      \
{                                                                          \
    _cc_deque_node(_cc_deque_object) *new_node =                           \
        malloc(sizeof(_cc_deque_node(_cc_deque_object)));                  \
                                                                           \
    new_node->val     = _cc_deque_push_back_value;                         \
    new_node->pitn[0] = NULL;                                              \
    new_node->pitn[1] = new_node;                                          \
    new_node->pitn[2] = &(new_node->val);                                  \
    new_node->pitn[3] = NULL;                                              \
                                                                           \
    if (cc_deque_empty((_cc_deque_object)))                                \
        (_cc_deque_object).head = &(new_node->pitn[0]);                    \
    else                                                                   \
        new_node->pitn[0] = &(*(_cc_deque_object).tail),                   \
        *(_cc_deque_object).tail = &(new_node->pitn[0]);                   \
                                                                           \
    (_cc_deque_object).tail = &(new_node->pitn[3]);                        \
    (_cc_deque_object).size++;                                             \
}


#define cc_deque_push_front(_cc_deque_object,                              \
                            _cc_deque_push_front_value)                    \
{                                                                          \
    _cc_deque_node(_cc_deque_object) *new_node =                           \
        malloc(sizeof(_cc_deque_node(_cc_deque_object)));                  \
                                                                           \
    new_node->val     = _cc_deque_push_front_value;                        \
    new_node->pitn[0] = NULL;                                              \
    new_node->pitn[1] = new_node;                                          \
    new_node->pitn[2] = &(new_node->val);                                  \
    new_node->pitn[3] = NULL;                                              \
                                                                           \
    if (cc_deque_empty((_cc_deque_object)))                                \
        (_cc_deque_object).tail = &(new_node->pitn[3]);                    \
    else                                                                   \
        new_node->pitn[3] = &(*(_cc_deque_object).head),                   \
        *(_cc_deque_object).head = &(new_node->pitn[3]);                   \
                                                                           \
    (_cc_deque_object).head = &(new_node->pitn[0]);                        \
    (_cc_deque_object).size++;                                             \
}


#define cc_deque_pop_back(_cc_deque_object)                                \
    /* TODO */


/* deque container deallocation */


#define cc_deque_dealloc(_cc_deque_object)                                 \
{                                                                          \
    cc_deque_iter(iter, _cc_deque_object);                                 \
    _cc_deque_iter_no_typedef(iter_dup, _cc_deque_object);                 \
    iter = cc_deque_iter_begin(_cc_deque_object);                          \
                                                                           \
    while (cc_deque_iter_is_valid(iter))                                   \
    {                                                                      \
        iter_dup = iter;                                                   \
        cc_deque_iter_incr(iter);                                          \
        free(*(iter_dup - 1));                                             \
    }                                                                      \
                                                                           \
    (_cc_deque_object).tail = (_cc_deque_object).head = NULL;              \
}



#endif
