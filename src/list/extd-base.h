#ifndef OPENGC3_LIST_EXTD_BASE_H
#define OPENGC3_LIST_EXTD_BASE_H


/* cc_ll integrity */


#define cc_ll_is_sorted_extd(_cc_ll, _leq, _ptrue, _ll_)                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (*(_ptrue)) = 1;                                                           \
                                                                               \
    if (_ll_##_size((_cc_ll)) <= 1)  break;                                    \
                                                                               \
    _it_init((_cc_ll), 2, _base_s3, _ll_);                                     \
                                                                               \
    _ll_##_iter_head (_it((_cc_ll), _base_s3, 0));                             \
    _ll_##_iter_begin(_it((_cc_ll), _base_s3, 1));                             \
                                                                               \
    while (1)                                                                  \
    {                                                                          \
        (void)_ll_##_iter_incr(_it((_cc_ll), _base_s3, 0));                    \
        (void)_ll_##_iter_incr(_it((_cc_ll), _base_s3, 1));                    \
                                                                               \
        if (!(_leq(_it((_cc_ll), _base_s3, 0), _it((_cc_ll), _base_s3, 1))))   \
        {                                                                      \
            (*(_ptrue)) = 0;  break;                                           \
        }                                                                      \
                                                                               \
        if (_ll_##_iter_at_end(_it((_cc_ll), _base_s3, 1)))  break;            \
    }                                                                          \
                                                                               \
    _it_clear((_cc_ll), 2, , );                                                \
)


#define cc_ll_is_robust(_cc_ll, _ptrue, _ll_, _LL_)                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _size;                                                                 \
    *(_ptrue) = 1;                                                             \
                                                                               \
    _size = 0;                                                                 \
    _LL_##_INCR((_cc_ll)->_iter)  _size++;                                     \
    *(_ptrue) &= !!(_ll_##_size((_cc_ll)) == _size);                           \
                                                                               \
    _size = 0;                                                                 \
    _LL_##_DECR((_cc_ll)->_iter)  _size++;                                     \
    *(_ptrue) &= !!(_ll_##_size((_cc_ll)) == _size);                           \
)



#endif
