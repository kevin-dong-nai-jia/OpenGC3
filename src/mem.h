#ifndef _CC_MEM_H_
#define _CC_MEM_H_

#include <stdio.h>
#include <stdlib.h>

#include "misc.h"


/* node operations */


#define _node_alloc(_pnode, _cc_ll)                                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_cc_ll).avsp == NULL)                                                 \
    {                                                                          \
        if ((_cc_ll).vcnt == 0)                                                \
        {                                                                      \
            (_cc_ll).pblock = (_cc_ll).pool;                                   \
                                                                               \
            if ((_cc_ll).used == 0)                                            \
                (_cc_ll).vcnt = ((_cc_ll).used  = (_cc_ll).start);             \
            else                                                               \
                (_cc_ll).vcnt = ((_cc_ll).used  < (_cc_ll).thrsh) ?            \
                                ((_cc_ll).used *= (_cc_ll).ratio) :            \
                                ((_cc_ll).used  = (_cc_ll).thrsh);             \
                                                                               \
            (_cc_ll).pool = malloc((sizeof(*(_cc_ll).pblock)) +                \
                                   (sizeof( (_cc_ll).pblock->nodes)) *         \
                                   (size_t)((_cc_ll).used - 1));               \
                                                                               \
            if ((_cc_ll).pool == NULL)                                         \
                _CC_FATAL(_CC_FATAL_MSG_ALLOC_FAIL);                           \
                                                                               \
            (_cc_ll).pool->next = (_cc_ll).pblock;                             \
        }                                                                      \
                                                                               \
        (_pnode) = &((_cc_ll).pool->nodes[--(_cc_ll).vcnt]);                   \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        (_pnode) = (_cc_ll).avsp;                                              \
        (_cc_ll).avsp = (_cc_ll).avsp->lnk;                                    \
    }                                                                          \
)


#define _node_clear(_pnode, _cc_ll)                                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_pnode)->lnk = (_cc_ll).avsp;                                             \
    (_cc_ll).avsp = (_pnode);                                                  \
)


#define _block_free(_cc_ll)                                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while ((_cc_ll).pool != NULL)                                              \
    {                                                                          \
        (_cc_ll).pblock = (_cc_ll).pool;                                       \
        (_cc_ll).pool   = (_cc_ll).pool->next;                                 \
        free((_cc_ll).pblock);                                                 \
    }                                                                          \
)



/* _it / _xl management */


#define _it_alloc(_ccxll, _items, _pbase)                                      \
                                                                               \
        _stack_alloc(_ccxll, _items, _pbase, ccxll_iter_init, _it)

#define _xl_alloc(_ccxll, _items, _pbase)                                      \
                                                                               \
        _stack_alloc(_ccxll, _items, _pbase, ccxll_init_from, _xl)

#define _stack_alloc(_ccxll, _items, _pbase, _pinit, _itxl_)                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int   _len  = 0;                                                           \
    void *_list = NULL;                                                        \
                                                                               \
    if ((_ccxll)._itxl_ != NULL)                                               \
        while ((_ccxll)._itxl_[++_len] != NULL);                               \
                                                                               \
    _safe_alloc(_list, (sizeof(**(_ccxll)._itxl_)) *                           \
                       ((size_t)_len + (_items) + 1));                         \
    memcpy(_list, (_ccxll)._itxl_, sizeof(**(_ccxll)._itxl_) * (size_t)_len);  \
                                                                               \
    if (_len != 0)                                                             \
        _safe_free((_ccxll)._itxl_);                                           \
                                                                               \
    (_ccxll)._itxl_ = _list;                                                   \
    (_ccxll)._itxl_ = _list;                                                   \
    (_ccxll)._itxl_[(_len + (_items))] = NULL;                                 \
                                                                               \
    for (int _idx = _len; _idx < _len + (_items); _idx++)                      \
    {                                                                          \
        (_ccxll)._itxl_[_idx] = NULL;                                          \
        _safe_alloc((_ccxll)._itxl_[_idx], sizeof(**(_ccxll)._itxl_));         \
        _pinit(*(_ccxll)._itxl_[_idx], (_ccxll));                              \
    }                                                                          \
                                                                               \
    *(_pbase) = _len;                                                          \
)


#define _it_free(_ccxll, _items)                                               \
                                                                               \
        _stack_free(_ccxll, _items, _it)

#define _xl_free(_ccxll, _items)                                               \
                                                                               \
        _stack_free(_ccxll, _items, _xl)

#define _stack_free(_ccxll, _items, _itxl_)                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _len = 0;                                                              \
                                                                               \
    if ((_ccxll)._itxl_ != NULL)                                               \
        while ((_ccxll)._itxl_[++_len] != NULL);                               \
                                                                               \
    for (int _idx = _len - 1; (_idx >= 0) && (_idx >= _len - (_items)); _idx--)\
        _safe_free((_ccxll)._itxl_[_idx]);                                     \
                                                                               \
    if ((_len <= _items) && (_items != 0))                                     \
        _safe_free((_ccxll)._itxl_);                                           \
)



/* safe (de-)allocation */


#define _safe_alloc(_void_ptr, _alloc_bytes)                                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_void_ptr) != NULL)                                                   \
        _CC_ERROR(_CC_ERROR_MSG_MEMORY_LEAK);                                  \
                                                                               \
    (_void_ptr) = malloc((_alloc_bytes));                                      \
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



/* error and fatal */


#define _CC_ERROR_MSG_MEMORY_LEAK          "Potential Memory Leak Problem."
#define _CC_ERROR_MSG_DOUBLE_FREE          "Potential Double Free Problem."

#define _CC_ERROR(_CC_ERROR_MSG)                                               \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    fprintf(stderr, "> CCC::ERROR: %s\n", _CC_ERROR_MSG);                      \
)


#define _CC_FATAL_MSG_ALLOC_FAIL           "Memory Space Allocation Failure."
#define _CC_FATAL_MSG_ALLOC_FAIL_EXITCODE  (-1)

#define _CC_FATAL(_CC_FATAL_MSG)                                               \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    fprintf(stderr, "> CCC::FATAL: %s\n", _CC_FATAL_MSG);                      \
    exit(_CC_FATAL_MSG##_EXITCODE);                                            \
)


#endif
