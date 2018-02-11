#ifndef OPENGC3_LIST_BASE_H
#define OPENGC3_LIST_BASE_H


/* exclusive or */


#define XOR2(_addr_a, _addr_b)                                                 \
(                                                                              \
    (void*)((uintptr_t)(void*)(_addr_a) ^                                      \
            (uintptr_t)(void*)(_addr_b))                                       \
)


#define XOR3(_addr_a, _addr_b, _addr_c)                                        \
(                                                                              \
    (void*)((uintptr_t)(void*)(_addr_a) ^                                      \
            (uintptr_t)(void*)(_addr_b) ^                                      \
            (uintptr_t)(void*)(_addr_c))                                       \
)


#define XOR2_SWAP(_addr_a, _addr_b)                                            \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_addr_a) = XOR2((_addr_a), (_addr_b)),                                    \
    (_addr_b) = XOR2((_addr_a), (_addr_b)),                                    \
    (_addr_a) = XOR2((_addr_a), (_addr_b))                                     \
)



/* cc_ll initialize */


#define cc_ll_init_extd(_cc_ll, _start, _ratio, _thrsh, _ll_)                  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_cc_ll) = NULL;                                                           \
                                                                               \
    _##_ll_##_init_extd((_cc_ll), (_start), (_ratio), (_thrsh), 1);            \
                                                                               \
    _itarr_init((_cc_ll), _ll_);                                               \
    _##_ll_##_iter_init((_cc_ll)->_iter, (_cc_ll), 1);                         \
)


#define _cc_ll_init_extd(_cc_ll, _start, _ratio, _thrsh, _alloc, _ll_)         \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int  _c_tmp =  _alloc;                                                     \
    if ((_c_tmp))  _cont_alloc((_cc_ll));                                      \
                                                                               \
    _##_ll_##_init_core((_cc_ll), (_c_tmp));                                   \
    _##_ll_##_init_info((_cc_ll), (_start), (_ratio), (_thrsh));               \
)


#define _cc_ll_init_core(_cc_ll, _alloc, _ll_)                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    _##_ll_##_init_seed((_cc_ll)),                                             \
                                                                               \
    (_cc_ll)->avsp = (_cc_ll)->pnode  = NULL,                                  \
    (_cc_ll)->pool = (_cc_ll)->pblock = NULL,                                  \
                                                                               \
    (_cc_ll)->itarr = NULL,                                                    \
    (_cc_ll)->_iter = NULL,                                                    \
                                                                               \
    (_cc_ll)->_it = ((_alloc) ? NULL : (_cc_ll)->_it),                         \
    (_cc_ll)->_co = ((_alloc) ? NULL : (_cc_ll)->_co),                         \
    (_cc_ll)->_it_base  = ((_alloc) ? 0 : (_cc_ll)->_it_base ),                \
    (_cc_ll)->_it_limit = ((_alloc) ? 0 : (_cc_ll)->_it_limit),                \
    (_cc_ll)->_co_base  = ((_alloc) ? 0 : (_cc_ll)->_co_base ),                \
    (_cc_ll)->_co_limit = ((_alloc) ? 0 : (_cc_ll)->_co_limit)                 \
)


#define _cc_ll_init_info(_cc_ll, _start, _ratio, _thrsh)                       \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_cc_ll)->start = ((_start) > 0) ? (_start) : 1,                           \
    (_cc_ll)->ratio = ((_ratio) > 0) ? (_ratio) : 1,                           \
    (_cc_ll)->thrsh = ((_thrsh) > (_cc_ll)->start) ? (_thrsh) : (_cc_ll)->start\
)


#define _cc_ll_iter_init(_iter, _cc_ll, _alloc, _ll_)                          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int  _i_tmp =  _alloc;                                                     \
    if ((_i_tmp))  _iter_alloc((_iter));                                       \
                                                                               \
    _ll_##_iter_init((_iter), (_cc_ll), (_i_tmp));                             \
)



/* cc_ll destroy */


#define cc_ll_free(_cc_ll)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_cc_ll) == NULL)  break;                                              \
                                                                               \
    _it_free((_cc_ll));                                                        \
    _co_free((_cc_ll));                                                        \
                                                                               \
    _iter_free ((_cc_ll)->_iter);                                              \
    _itarr_free((_cc_ll));                                                     \
    _block_free((_cc_ll));                                                     \
    _cont_free ((_cc_ll));                                                     \
)



/* cc_ll operations */


#define cc_ll_move_into(_cc_ll_d, _cc_ll_s, _ll_, _opt_, _auxr_)               \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely(_ll_##_empty((_cc_ll_s))))  break;                           \
                                                                               \
    _it_init        ((_cc_ll_d), 1, _base_m1, _ll_);                           \
    _it_init##_auxr_((_cc_ll_s), 1, _base_m2, _ll_);                           \
    _it_init        ((_cc_ll_s), 1, _base_m3, _ll_);                           \
                                                                               \
    _##_ll_##_move_into##_opt_(_it        ((_cc_ll_d), _base_m1, 0),           \
                               _it        ((_cc_ll_s), _base_m3, 0),           \
                               _it##_auxr_((_cc_ll_s), _base_m2, 0));          \
                                                                               \
    _it_clear        ((_cc_ll_d), 1, , );                                      \
    _it_clear        ((_cc_ll_s), 1, , );                                      \
    _it_clear##_auxr_((_cc_ll_s), 1, _base_m2, _ll_);                          \
)


#define _cc_ll_move_into(_iter_l, _iter_m, _iter_r, _ll_)                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _ll_##_iter_tail ((_iter_l));                                              \
    _ll_##_iter_begin((_iter_m));                                              \
    _ll_##_iter_tail ((_iter_r));                                              \
                                                                               \
    _ll_##_move_range_extd((_iter_l), (_iter_m),                               \
                           (_iter_r), _ll_##_size((_iter_m)->cont));           \
)


#define cc_ll_merge_extd(_cc_ll_d, _cc_ll_s, _leq, _ll_, _opt_, _auxr_, _last) \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely(_ll_##_empty((_cc_ll_s))))  break;                           \
                                                                               \
    _it_init##_auxr_((_cc_ll_d), 1, _base_m4, _ll_);                           \
    _it_init        ((_cc_ll_s), 2, _base_m5, _ll_);                           \
                                                                               \
    _##_ll_##_merge##_opt_##_extd(_it##_auxr_((_cc_ll_d), _base_m4, 0),        \
                                  _it        ((_cc_ll_s), _base_m5, 0),        \
                                  _it        ((_cc_ll_s), _base_m5, 1),        \
                                  _leq, _last);                                \
                                                                               \
    _it_clear##_auxr_((_cc_ll_d), 1, _base_m4, _ll_);                          \
    _it_clear        ((_cc_ll_s), 2, , );                                      \
)


#define _cc_ll_merge_extd(_iter_l, _iter_m, _iter_r, _leq, _ll_, _opt_)        \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _ll_##_iter_tail ((_iter_l));                                              \
    _ll_##_iter_begin((_iter_m));                                              \
    _ll_##_iter_tail ((_iter_r));                                              \
                                                                               \
    _ll_##_move_range_extd((_iter_l), (_iter_m),                               \
                           (_iter_r), _ll_##_size((_iter_m)->cont));           \
                                                                               \
    _ll_##_iter_begin((_iter_l));                                              \
    _ll_##_iter_init ((_iter_r), (_iter_l)->cont, 0);                          \
    _ll_##_iter_tail ((_iter_r));                                              \
                                                                               \
    _ll_##_merge_range##_opt_##_extd((_iter_l), (_iter_m), (_iter_r), _leq);   \
)


#define cc_ll_merge_range_extd(_iter_l, _iter_m, _iter_r, _leq, _ll_, _opt_)   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_init((_iter_l)->cont, 1, _base_m6, _ll_);                              \
                                                                               \
    _##_ll_##_merge_range##_opt_##_extd((_iter_l), (_iter_m), (_iter_r),       \
                                    _it((_iter_l)->cont, _base_m6, 0), _leq);  \
                                                                               \
    _it_clear((_iter_l)->cont, 1, , );                                         \
)


#define _cc_ll_merge_range_extd(_iter_l, _iter_m, _iter_r, _iter_x, _leq, _ll_)\
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely((_iter_l)->cont != (_iter_m)->cont ||                        \
                  (_iter_m)->cont != (_iter_r)->cont))  break;                 \
                                                                               \
    _ll_##_iter_copy((_iter_x), (_iter_m));                                    \
                                                                               \
    for (register int _neq; ; )                                                \
    {                                                                          \
        while ((_neq = ((_iter_l)->curr.node                                   \
                     != (_iter_m)->curr.node)) &&  _leq((_iter_l), (_iter_m))) \
            (void)(_ll_##_iter_incr((_iter_l)));                               \
                                                                               \
        if (!(_neq))                                                           \
        {                                                                      \
            _ll_##_iter_copy((_iter_l), (_iter_r));                            \
            _ll_##_iter_copy((_iter_m), (_iter_r));  break;                    \
        }                                                                      \
                                                                               \
        (void)(_ll_##_iter_incr((_iter_x)));                                   \
                                                                               \
        while ((_neq = ((_iter_x)->curr.node                                   \
                     != (_iter_r)->curr.node)) && !_leq((_iter_l), (_iter_x))) \
            (void)(_ll_##_iter_incr((_iter_x)));                               \
                                                                               \
        _ll_##_move_range((_iter_l), (_iter_m), (_iter_x));                    \
        _ll_##_iter_copy ((_iter_m), (_iter_x));                               \
                                                                               \
        if (!(_neq))                                                           \
        {                                                                      \
            _ll_##_iter_copy((_iter_l), (_iter_x));                            \
            _ll_##_iter_copy((_iter_r), (_iter_x));  break;                    \
        }                                                                      \
    }                                                                          \
)


#define cc_ll_sort_extd(_cc_ll, _leq, _ll_, _opt_)                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely(_ll_##_size((_cc_ll)) <= 1))  break;                         \
                                                                               \
    int _buck = (int)(log2(_ll_##_size((_cc_ll)))) + 1;                        \
                                                                               \
    _co_init((_cc_ll), 1 + _buck, _base_s1, _ll_);                             \
    _it_init((_cc_ll), 2        , _base_s2, _ll_);                             \
                                                                               \
    _##_ll_##_sort##_opt_##_extd(      (_cc_ll),                               \
                                   _co((_cc_ll), _base_s1, 0),                 \
                                 &(_co((_cc_ll), _base_s1, 1)),                \
                                   _it((_cc_ll), _base_s2, 0),                 \
                                   _it((_cc_ll), _base_s2, 1), _leq);          \
                                                                               \
    _co_clear((_cc_ll), _buck + 1, , );                                        \
    _it_clear((_cc_ll),  2, , );                                               \
)


#define _cc_ll_sort_extd(_cc_ll,  _carry,  _pcont,                             \
                         _iter_a, _iter_b, _leq, _ll_, _opt_)                  \
STATEMENT_                                                                     \
(                                                                              \
    int _fill = 0, _curr;                                                      \
                                                                               \
    do                                                                         \
    {   _ll_##_iter_init ((_iter_a), (_carry), 0);                             \
        _ll_##_iter_init ((_iter_b), (_cc_ll), 0);                             \
        _ll_##_move_begin((_iter_a), (_iter_b));                               \
                                                                               \
        for (_curr = 0; _curr != _fill &&                                      \
                       !(_ll_##_empty((_pcont)[_curr])); _curr++)              \
        {                                                                      \
            _ll_##_merge##_opt_##_extd((_pcont)[_curr], (_carry), _leq, 0);    \
            _ll_##_swap((_pcont)[_curr], (_carry));                            \
        }                                                                      \
        _ll_##_swap((_pcont)[_curr], (_carry));                                \
                                                                               \
        if (_unlikely(_curr == _fill))  _fill++;                               \
    }                                                                          \
    while (!(_ll_##_empty((_cc_ll))));                                         \
                                                                               \
    for (_curr = 1; _curr < _fill; _curr++)                                    \
        _ll_##_merge##_opt_##_extd((_pcont)[_curr],                            \
                                   (_pcont)[_curr - 1], _leq,                  \
                                  !(_fill - _curr - 1));                       \
                                                                               \
    _ll_##_move_into##_opt_((_cc_ll), (_pcont)[_fill - 1]);                    \
)



/* cc_ll iterators */


#define cc_ll_iter_distance(_iter_a, _iter_b, _pdist, _ll_ /* != ccsll */ )    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_init((_iter_a)->cont, 1, _base_d1, _ll_);                              \
                                                                               \
    _ll_##_iter_copy(_it((_iter_a)->cont, _base_d1, 0), (_iter_a));            \
                                                                               \
    STATEMENT_                                                                 \
    (                                                                          \
        (*(_pdist)) = 0;                                                       \
        if ((_iter_a)->cont != (_iter_b)->cont)  break;                        \
                                                                               \
        while ((_iter_a)->curr.node != (_iter_b)->curr.node && ++(*(_pdist)))  \
               if (!(_ll_##_iter_incr((_iter_a))))  break;                     \
                                                                               \
        if ((_iter_a)->curr.node == (_iter_b)->curr.node)  break;              \
        else  (*(_pdist)) = 0;                                                 \
                                                                               \
        _ll_##_iter_copy((_iter_a), _it((_iter_a)->cont, _base_d1, 0));        \
                                                                               \
        while ((_iter_a)->curr.node != (_iter_b)->curr.node && --(*(_pdist)))  \
               if (!(_ll_##_iter_decr((_iter_a))))  break;                     \
                                                                               \
        if ((_iter_a)->curr.node == (_iter_b)->curr.node)  break;              \
        else  (*(_pdist)) = 0;                                                 \
    );                                                                         \
                                                                               \
    _ll_##_iter_copy((_iter_a), _it((_iter_a)->cont, _base_d1, 0));            \
                                                                               \
    _it_clear((_iter_a)->cont, 1, , );                                         \
)



/* cc_ll traversor */


#define CC_LL_INCR(_iter, _ll_)                                                \
                                                                               \
    for (_ll_##_iter_head((_iter)); _ll_##_iter_incr((_iter)); )


#define CC_LL_INCR_EXTD(_pval, _cc_ll, _ll_, ...)                              \
                                                                               \
    for (__typeof__((_cc_ll)->pnode->val) *_pval,                              \
         *_init = (_ll_##_iter_head((_cc_ll)->_iter), NULL);                   \
         (_ll_##_iter_incr((_cc_ll)->_iter)) &&                                \
         ((_pval) = &(LREF((_cc_ll)->_iter)), 1); (__VA_ARGS__), (void)_init)


#define CC_LL_DECR(_iter, _ll_)                                                \
                                                                               \
    for (_ll_##_iter_tail((_iter)); _ll_##_iter_decr((_iter)); )


#define CC_LL_DECR_EXTD(_pval, _cc_ll, _ll_, ...)                              \
                                                                               \
    for (__typeof__((_cc_ll)->pnode->val) *_pval,                              \
         *_init = (_ll_##_iter_tail((_cc_ll)->_iter), NULL);                   \
         (_ll_##_iter_decr((_cc_ll)->_iter)) &&                                \
         ((_pval) = &(LREF((_cc_ll)->_iter)), 1); (__VA_ARGS__), (void)_init)



#endif
