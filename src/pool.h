#ifndef _CC_POOL_H_
#define _CC_POOL_H_

#include <stdio.h>
#include <string.h>

#include "mesg.h"
#include "misc.h"


/* safe allocation */


#define _safe_alloc(_void_ptr, _alloc_bytes)                                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_void_ptr) != NULL)                                                   \
        _CC_ERROR(_CC_ERROR_MSG_MEMORY_LEAK);                                  \
                                                                               \
    if (((_void_ptr) = malloc((_alloc_bytes))) == NULL)                        \
        _CC_FATAL(_CC_FATAL_MSG_MALLOC_FAIL);                                  \
)


#define _safe_free(_void_ptr)                                                  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_void_ptr) == NULL)                                                   \
        _CC_ERROR(_CC_ERROR_MSG_DOUBLE_FREE);                                  \
                                                                               \
    free((_void_ptr));                                                         \
    (_void_ptr) = NULL;                                                        \
)



/* ccxll / iter management */


#define _ccxll_alloc(_ccxll)  _safe_alloc((_ccxll), sizeof(*(_ccxll)))

#define _ccxll_free(_ccxll)   _safe_free ((_ccxll))

#define _iter_alloc(_iter)    _safe_alloc((_iter),  sizeof(*(_iter)))

#define _iter_free(_iter)     _safe_free ((_iter))



/* _it / _xl management */


#define _it_xl_total(_ccxll, _itxl_)                                           \
                                                                               \
        ((_ccxll)->_itxl_##_base + (_ccxll)->_itxl_##_limit)


#define _it_alloc(_ccxll, _items, _pbase)                                      \
                                                                               \
        _it_xl_alloc(_ccxll, _items, _pbase, _ccxll_iter_init, _it)

#define _xl_alloc(_ccxll, _items, _pbase)                                      \
                                                                               \
        _it_xl_alloc(_ccxll, _items, _pbase, _ccxll_init_from, _xl)

#define _it_xl_alloc(_ccxll, _items, _pbase, _pinit, _itxl_)                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    unsigned char _total = _it_xl_total((_ccxll), _itxl_);                     \
                                                                               \
    _stack_alloc((_ccxll), (_items), (_pbase), _itxl_);                        \
                                                                               \
    for (int _idx = _total; _idx < *(_pbase) + (_items); _idx++)               \
        _pinit((_ccxll)->_itxl_[_idx], (_ccxll));                              \
)


#define _it_clear(_ccxll, _items)                                              \
                                                                               \
        _it_xl_clear(_ccxll, _items, _it)

#define _xl_clear(_ccxll, _items)                                              \
                                                                               \
        _it_xl_clear(_ccxll, _items, _xl)

#define _it_xl_clear(_ccxll, _items, _itxl_)                                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_ccxll)->_itxl_##_base  -= (_items);                                      \
    (_ccxll)->_itxl_##_limit += (_items);                                      \
)


#define _it_free(_ccxll)                                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    unsigned char _it_total = _it_xl_total((_ccxll), _it);                     \
                                                                               \
    for (int _idx_it = 0; _idx_it < _it_total; _idx_it++)                      \
        _iter_free((_ccxll)->_it[_idx_it]);                                    \
                                                                               \
    _stack_free((_ccxll), _it);                                                \
)


#define _xl_free(_ccxll)                                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    unsigned char _xl_total = _it_xl_total((_ccxll), _xl);                     \
                                                                               \
    for (int _idx_xl = 0; _idx_xl < _xl_total; _idx_xl++)                      \
    {                                                                          \
        _it_free((_ccxll)->_xl[_idx_xl]);                                      \
        _block_free((_ccxll)->_xl[_idx_xl]);                                   \
        _ccxll_free((_ccxll)->_xl[_idx_xl]);                                   \
    }                                                                          \
                                                                               \
    _stack_free((_ccxll), _xl);                                                \
)



/* stack management */


#define _stack_alloc(_ccxll, _items, _pbase, _itxl_)                           \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_items) > (_ccxll)->_itxl_##_limit && (_items) != 0)                  \
    {                                                                          \
        size_t _size = sizeof(*(_ccxll)->_itxl_);                              \
        unsigned char _itxl_total = _it_xl_total((_ccxll), _itxl_);            \
                                                                               \
        void **_tmp = (void*)&((_ccxll)->_itxl_);                              \
        *_tmp = realloc(*_tmp, (_size * ((_ccxll)->_itxl_##_base + (_items))));\
                                                                               \
        memset((_ccxll)->_itxl_ + _itxl_total, 0,                              \
               (_size * ((_ccxll)->_itxl_##_base + (_items) - _itxl_total)));  \
                                                                               \
        (_ccxll)->_itxl_##_limit = (_items);                                   \
    }                                                                          \
                                                                               \
    *(_pbase) = (_ccxll)->_itxl_##_base;                                       \
    (_ccxll)->_itxl_##_base  += (_items);                                      \
    (_ccxll)->_itxl_##_limit -= (_items);                                      \
)


#define _stack_free(_ccxll, _itxl_)                                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_it_xl_total((_ccxll), _itxl_) != 0)                                   \
        _safe_free((_ccxll)->_itxl_);                                          \
                                                                               \
    (_ccxll)->_itxl_##_base  = 0;                                              \
    (_ccxll)->_itxl_##_limit = 0;                                              \
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
