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


#endif
