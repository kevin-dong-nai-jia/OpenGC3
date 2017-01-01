#ifndef CC_POOL_H
#define CC_POOL_H

#include "mesg.h"
#include "misc.h"

#include <stdio.h>
#include <string.h>


/* safe allocation */


#define _safe_alloc(_void_ptr, _alloc_bytes)                                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_void_ptr) != NULL)                                                   \
        CC_ERROR(CC_ERROR_MSG_MEMORY_LEAK);                                    \
                                                                               \
    if (((_void_ptr) = malloc((_alloc_bytes))) == NULL)                        \
        CC_FATAL(CC_FATAL_MSG_MALLOC_FAIL);                                    \
)


#define _safe_free(_void_ptr)                                                  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_void_ptr) == NULL)                                                   \
        CC_ERROR(CC_ERROR_MSG_DOUBLE_FREE);                                    \
                                                                               \
    free((_void_ptr));                                                         \
    (_void_ptr) = NULL;                                                        \
)



/* cc_ll / iter management */


#define _cc_ll_alloc(_cc_ll)  _safe_alloc((_cc_ll), sizeof(*(_cc_ll)))

#define _cc_ll_free(_cc_ll)   _safe_free ((_cc_ll))

#define _iter_alloc(_iter)    _safe_alloc((_iter),  sizeof(*(_iter)))

#define _iter_free(_iter)     _safe_free ((_iter))



/* itarr management */


#define _itarr_alloc(_cc_ll, _cc_ll_)                                          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _safe_alloc((_cc_ll)->itarr, sizeof(*(_cc_ll)->itarr));                    \
                                                                               \
    for (int _idx = 0; _idx < (int)ELEMOF(*(_cc_ll)->itarr); _idx++)           \
        _cc_ll_##_iter_init(&((*(_cc_ll)->itarr)[_idx]), (_cc_ll));            \
)


#define _itarr_free(_cc_ll)                                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _safe_free((_cc_ll)->itarr);                                               \
)



/* _it / _ll management */


#define _it_ll_total(_cc_ll, _itll_)                                           \
                                                                               \
        ((_cc_ll)->_itll_##_base + (_cc_ll)->_itll_##_limit)


#ifndef CCC_STRICT

#define _it_alloc(_cc_ll, _items, _iter, _cc_ll_)                              \
                                                                               \
        __typeof__(**(_cc_ll)->_it) _iter[(_items)];                           \
                                                                               \
        for (int _cnt = 0; _cnt < (_items); _cnt++)                            \
            _cc_ll_##_iter_init(_it_((_cc_ll), _iter, _cnt), (_cc_ll))

#else

#define _it_alloc(_cc_ll, _items, _base, _cc_ll_)                              \
                                                                               \
        int _base;                                                             \
                                                                               \
        _it_ll_alloc((_cc_ll), (_items), &(_base),                             \
                     _##_cc_ll_##_iter_init, _it);                             \
                                                                               \
        for (int _cnt = 0; _cnt < (_items); _cnt++)                            \
            _cc_ll_##_iter_init(_it_((_cc_ll), _base, _cnt), (_cc_ll))

#endif // CCC_STRICT


#define _ll_alloc(_cc_ll, _items, _base, _cc_ll_)                              \
                                                                               \
        int _base;                                                             \
                                                                               \
        _it_ll_alloc((_cc_ll), (_items), &(_base), _##_cc_ll_##_init_from, _ll)


#define _it_ll_alloc(_cc_ll, _items, _pbase, _pinit, _itll_)                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    unsigned char _itll_total = _it_ll_total((_cc_ll), _itll_);                \
                                                                               \
    _stack_alloc((_cc_ll), (_items), (_pbase), _itll_);                        \
                                                                               \
    for (int _idx = _itll_total; _idx < (*(_pbase) + (_items)); _idx++)        \
        _pinit((_cc_ll)->_itll_[_idx], (_cc_ll));                              \
)


#ifndef CCC_STRICT

#define _it_clear(_cc_ll, _items)

#else

#define _it_clear(_cc_ll, _items)                                              \
                                                                               \
        _it_ll_clear(_cc_ll, _items, _it)

#endif // CCC_STRICT


#define _ll_clear(_cc_ll, _items)                                              \
                                                                               \
        _it_ll_clear(_cc_ll, _items, _ll)


#define _it_ll_clear(_cc_ll, _items, _itll_)                                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _stack_clear((_cc_ll), (_items), _itll_);                                  \
)


#define _it_free(_cc_ll)                                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    unsigned char _it_total = _it_ll_total((_cc_ll), _it);                     \
                                                                               \
    for (int _idx_it = 0; _idx_it < _it_total; _idx_it++)                      \
        _iter_free((_cc_ll)->_it[_idx_it]);                                    \
                                                                               \
    _stack_free((_cc_ll), _it);                                                \
)


#define _ll_free(_cc_ll)                                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    unsigned char _ll_total = _it_ll_total((_cc_ll), _ll);                     \
                                                                               \
    for (int _idx_ll = 0; _idx_ll < _ll_total; _idx_ll++)                      \
    {                                                                          \
        _it_free((_cc_ll)->_ll[_idx_ll]);                                      \
                                                                               \
        _block_free((_cc_ll)->_ll[_idx_ll]);                                   \
        _cc_ll_free((_cc_ll)->_ll[_idx_ll]);                                   \
    }                                                                          \
                                                                               \
    _stack_free((_cc_ll), _ll);                                                \
)



/* stack management */


#define _stack_alloc(_cc_ll, _items, _pbase, _itll_)                           \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_items) > (_cc_ll)->_itll_##_limit && (_items) != 0)                  \
    {                                                                          \
        size_t _size = sizeof(*(_cc_ll)->_itll_);                              \
        unsigned char _stack_total = _it_ll_total((_cc_ll), _itll_);           \
                                                                               \
        void **_tmp = (void*)&((_cc_ll)->_itll_);                              \
        *_tmp = realloc(*_tmp, (_size * ((_cc_ll)->_itll_##_base + (_items))));\
                                                                               \
        memset((_cc_ll)->_itll_ + _stack_total, 0,                             \
               (_size * ((_cc_ll)->_itll_##_base + (_items) - _stack_total))); \
                                                                               \
        (_cc_ll)->_itll_##_limit = (_items);                                   \
    }                                                                          \
                                                                               \
    *(_pbase) = (_cc_ll)->_itll_##_base;                                       \
    (_cc_ll)->_itll_##_base  += (_items);                                      \
    (_cc_ll)->_itll_##_limit -= (_items);                                      \
)


#define _stack_clear(_cc_ll, _items, _itll_)                                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_cc_ll)->_itll_##_base  -= (_items);                                      \
    (_cc_ll)->_itll_##_limit += (_items);                                      \
)


#define _stack_free(_cc_ll, _itll_)                                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_it_ll_total((_cc_ll), _itll_) != 0)                                   \
        _safe_free((_cc_ll)->_itll_);                                          \
                                                                               \
    (_cc_ll)->_itll_##_base  = 0;                                              \
    (_cc_ll)->_itll_##_limit = 0;                                              \
)



/* node / block management */


#define _node_alloc(_pnode, _cc_ll)                                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_cc_ll)->avsp == NULL)                                                \
    {                                                                          \
        if ((_cc_ll)->vcnt == 0)                                               \
        {                                                                      \
            (_cc_ll)->pblock = (_cc_ll)->pool;                                 \
            (_cc_ll)->pool   = NULL;                                           \
                                                                               \
            if ((_cc_ll)->used == 0)                                           \
                (_cc_ll)->vcnt = ((_cc_ll)->used  = (_cc_ll)->start);          \
            else                                                               \
                (_cc_ll)->vcnt = ((_cc_ll)->used  < (_cc_ll)->thrsh) ?         \
                                 ((_cc_ll)->used *= (_cc_ll)->ratio) :         \
                                 ((_cc_ll)->used  = (_cc_ll)->thrsh);          \
                                                                               \
            _safe_alloc((_cc_ll)->pool, (sizeof(*(_cc_ll)->pblock)) +          \
                                        (sizeof( (_cc_ll)->pblock->nodes)) *   \
                                        (size_t)((_cc_ll)->used - 1));         \
                                                                               \
            (_cc_ll)->pool->next = (_cc_ll)->pblock;                           \
        }                                                                      \
                                                                               \
        (_pnode) = &((_cc_ll)->pool->nodes[--(_cc_ll)->vcnt]);                 \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        (_pnode)       = (_cc_ll)->avsp;                                       \
        (_cc_ll)->avsp = (_cc_ll)->avsp->lnk;                                  \
    }                                                                          \
)


#define _node_clear(_pnode, _cc_ll)                                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_pnode)->lnk  = (_cc_ll)->avsp;                                           \
    (_cc_ll)->avsp = (_pnode);                                                 \
)


#define _block_free(_cc_ll)                                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while ((_cc_ll)->pool != NULL)                                             \
    {                                                                          \
        (_cc_ll)->pblock = (_cc_ll)->pool;                                     \
        (_cc_ll)->pool   = (_cc_ll)->pool->next;                               \
        _safe_free((_cc_ll)->pblock);                                          \
    }                                                                          \
)



#endif
