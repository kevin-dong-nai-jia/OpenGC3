#ifndef OPENGC3_LIST_BASE_H
#define OPENGC3_LIST_BASE_H


/* cc_ll initialize */


#define cc_ll_init_extd(_cc_ll, _start, _ratio, _thrsh, _dx_)                  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_cc_ll) = NULL;                                                           \
                                                                               \
    _##_dx_##_init_extd((_cc_ll), (_start), (_ratio), (_thrsh), 1);            \
                                                                               \
    _itarr_alloc((_cc_ll), _dx_);                                              \
    _##_dx_##_iter_init((_cc_ll)->_iter, (_cc_ll), 1);                         \
)


#define _cc_ll_init_extd(_cc_ll, _start, _ratio, _thrsh, _alloc, _dx_)         \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_alloc))  _cont_alloc((_cc_ll));                                      \
                                                                               \
    _##_dx_##_init_core((_cc_ll));                                             \
    _##_dx_##_init_info((_cc_ll), (_start), (_ratio), (_thrsh));               \
)


#define _cc_ll_init_core(_cc_ll, _dx_)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    _##_dx_##_init_seed((_cc_ll)),                                             \
                                                                               \
    (_cc_ll)->avsp = (_cc_ll)->pnode  = NULL,                                  \
    (_cc_ll)->pool = (_cc_ll)->pblock = NULL,                                  \
                                                                               \
    (_cc_ll)->itarr = NULL,                                                    \
    (_cc_ll)->_iter = NULL,                                                    \
    (_cc_ll)->_it   = NULL,                                                    \
    (_cc_ll)->_co   = NULL,                                                    \
    (_cc_ll)->_it_base = (_cc_ll)->_it_limit = 0,                              \
    (_cc_ll)->_co_base = (_cc_ll)->_co_limit = 0                               \
)


#define _cc_ll_init_info(_cc_ll, _start, _ratio, _thrsh)                       \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_cc_ll)->start = ((_start) > 0) ? (_start) : 1,                           \
    (_cc_ll)->ratio = ((_ratio) > 0) ? (_ratio) : 1,                           \
    (_cc_ll)->thrsh = ((_thrsh) > (_cc_ll)->start) ? (_thrsh) : (_cc_ll)->start\
)


#define _cc_ll_iter_init(_iter, _cc_ll, _alloc, _dx_)                          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_alloc))  _iter_alloc((_iter));                                       \
                                                                               \
    _dx_##_iter_init((_iter), (_cc_ll));                                       \
)



/* cc_ll destroy */


#define cc_ll_free(_cc_ll)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_free((_cc_ll));                                                        \
    _co_free((_cc_ll));                                                        \
                                                                               \
    _iter_free ((_cc_ll)->_iter);                                              \
    _itarr_free((_cc_ll));                                                     \
    _block_free((_cc_ll));                                                     \
    _cont_free ((_cc_ll));                                                     \
)



/* cc_ll operations */


#define cc_ll_merge_extd(_cc_ll_d, _cc_ll_s, _leq, _dx_)                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
     if (_unlikely(_dx_##_empty((_cc_ll_s))))  break;                          \
                                                                               \
    _it_alloc((_cc_ll_d), 1, _base_m1, _dx_);                                  \
    _it_alloc((_cc_ll_s), 2, _base_m2, _dx_);                                  \
                                                                               \
    _##_dx_##_merge_extd((_cc_ll_d), _it_((_cc_ll_d), _base_m1, 0),            \
                                     _it_((_cc_ll_s), _base_m2, 0),            \
                                     _it_((_cc_ll_s), _base_m2, 1), _leq);     \
                                                                               \
    _it_clear((_cc_ll_d), 1);                                                  \
    _it_clear((_cc_ll_s), 2);                                                  \
)


#define _cc_ll_merge_extd(_cc_ll_d, _iter_l, _iter_m, _iter_r, _leq, _dx_)     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _dx_##_iter_tail ((_iter_l));                                              \
    _dx_##_iter_begin((_iter_m));                                              \
    _dx_##_iter_tail ((_iter_r));                                              \
                                                                               \
    _dx_##_move_range_extd( (_iter_l), (_iter_m), (_iter_r),                   \
                           _dx_##_size((_iter_m)->_dx_));                      \
                                                                               \
    _dx_##_iter_begin((_iter_l));                                              \
    _dx_##_iter_init ((_iter_r), (_cc_ll_d));                                  \
    _dx_##_iter_tail ((_iter_r));                                              \
                                                                               \
    _dx_##_merge_range_extd((_iter_l), (_iter_m), (_iter_r), _leq);            \
)


#define cc_ll_merge_range_extd(_iter_l, _iter_m, _iter_r, _leq, _dx_)          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_alloc((_iter_l)->_dx_, 1, _base_m3, _dx_);                             \
                                                                               \
    _##_dx_##_merge_range_extd((_iter_l), (_iter_m), (_iter_r),                \
                               _it_((_iter_l)->_dx_, _base_m3, 0), _leq);      \
                                                                               \
    _it_clear((_iter_l)->_dx_, 1);                                             \
)


#define _cc_ll_merge_range_extd(_iter_l, _iter_m, _iter_r, _iter_x, _leq, _dx_)\
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_l)->_dx_ != (_iter_m)->_dx_ ||                        \
                  (_iter_m)->_dx_ != (_iter_r)->_dx_))  break;                 \
                                                                               \
    _dx_##_iter_copy((_iter_x), (_iter_m));                                    \
                                                                               \
    while (1)                                                                  \
    {                                                                          \
        while ((_iter_l)->curr.node != (_iter_m)->curr.node &&                 \
                _leq((_iter_l), (_iter_m)))                                    \
            (void)_dx_##_iter_incr((_iter_l));                                 \
                                                                               \
        if (_unlikely((_iter_l)->curr.node == (_iter_m)->curr.node))           \
        {                                                                      \
            _dx_##_iter_copy((_iter_l), (_iter_r));                            \
            _dx_##_iter_copy((_iter_m), (_iter_r));  break;                    \
        }                                                                      \
                                                                               \
        (void)_dx_##_iter_incr((_iter_x));                                     \
                                                                               \
        while ((_iter_x)->curr.node != (_iter_r)->curr.node &&                 \
               !_leq((_iter_l), (_iter_x)))                                    \
            (void)_dx_##_iter_incr((_iter_x));                                 \
                                                                               \
        _dx_##_move_range((_iter_l), (_iter_m), (_iter_x));                    \
        _dx_##_iter_copy ((_iter_m), (_iter_x));                               \
                                                                               \
        if (_unlikely((_iter_x)->curr.node == (_iter_r)->curr.node))           \
        {                                                                      \
            _dx_##_iter_copy((_iter_l), (_iter_x));                            \
            _dx_##_iter_copy((_iter_r), (_iter_x));  break;                    \
        }                                                                      \
    }                                                                          \
)


#define cc_ll_sort_extd(_cc_ll, _leq, _dx_)                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely(_dx_##_size((_cc_ll)) <= 1))  break;                         \
                                                                               \
    int _buck = (int)(log2(_dx_##_size((_cc_ll)))) + 1;                        \
                                                                               \
    _co_alloc((_cc_ll), 1 + _buck, _base_s1, _dx_);                            \
    _it_alloc((_cc_ll), 2        , _base_s2, _dx_);                            \
                                                                               \
    _##_dx_##_sort_extd(       (_cc_ll),                                       \
                          _co_((_cc_ll), _base_s1, 0),                         \
                        &(_co_((_cc_ll), _base_s1, 1)),                        \
                          _it_((_cc_ll), _base_s2, 0),                         \
                          _it_((_cc_ll), _base_s2, 1), _leq);                  \
                                                                               \
    _co_clear((_cc_ll), _buck + 1);                                            \
    _it_clear((_cc_ll),  2);                                                   \
)


#define _cc_ll_sort_extd(_cc_ll, _carry, _pbuck, _iter_a, _iter_b, _leq, _dx_) \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _fill = 0, _curr;                                                      \
                                                                               \
    do                                                                         \
    {   _dx_##_iter_init ((_iter_a), (_carry));                                \
        _dx_##_iter_init ((_iter_b), (_cc_ll));                                \
        _dx_##_iter_begin((_iter_a));                                          \
        _dx_##_iter_begin((_iter_b));                                          \
                                                                               \
        _dx_##_move((_iter_a), (_iter_b));                                     \
                                                                               \
        for (_curr = 0; _curr != _fill &&                                      \
                       !(_dx_##_empty((_pbuck)[_curr])); _curr++)              \
        {                                                                      \
            _dx_##_merge_extd((_pbuck)[_curr], (_carry), _leq);                \
            _dx_##_swap      ((_pbuck)[_curr], (_carry));                      \
        }                                                                      \
        _dx_##_swap((_pbuck)[_curr], (_carry));                                \
                                                                               \
        if (_unlikely(_curr == _fill))  _fill++;                               \
    }                                                                          \
    while (!(_dx_##_empty((_cc_ll))));                                         \
                                                                               \
    for (_curr = 0; _curr < _fill; _curr++)                                    \
        _dx_##_merge_extd((_cc_ll), (_pbuck)[_curr], _leq);                    \
)



/* cc_ll traversor */


#define CC_LL_INCR(_iter, _dx_)                                                \
                                                                               \
    for (_dx_##_iter_head((_iter)); _dx_##_iter_incr((_iter)); )


#define CC_LL_INCR_EXTD(_pval, _cc_ll, _dx_, ...)                              \
                                                                               \
    for (__typeof__((_cc_ll)->pnode->val) *_pval,                              \
         *_init = (_dx_##_iter_head((_cc_ll)->_iter), NULL);                   \
         (_dx_##_iter_incr((_cc_ll)->_iter)) &&                                \
         ((_pval) = &(LREF((_cc_ll)->_iter)), 1); (__VA_ARGS__), (void)_init)


#define CC_LL_DECR(_iter, _dx_)                                                \
                                                                               \
    for (_dx_##_iter_tail((_iter)); _dx_##_iter_decr((_iter)); )


#define CC_LL_DECR_EXTD(_pval, _cc_ll, _dx_, ...)                              \
                                                                               \
    for (__typeof__((_cc_ll)->pnode->val) *_pval,                              \
         *_init = (_dx_##_iter_tail((_cc_ll)->_iter), NULL);                   \
         (_dx_##_iter_decr((_cc_ll)->_iter)) &&                                \
         ((_pval) = &(LREF((_cc_ll)->_iter)), 1); (__VA_ARGS__), (void)_init)



#endif
