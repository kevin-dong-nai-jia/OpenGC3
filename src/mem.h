#ifndef _CC_MEM_H_
#define _CC_MEM_H_

#include <stdio.h>
#include <string.h>

#include "msg.h"
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



/* ccxll management */


#define _ccxll_alloc(_ccxll)  _safe_alloc((_ccxll), sizeof(*(_ccxll)))

#define _ccxll_free(_ccxll)   _safe_free ((_ccxll))



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



/* stack management */


#define _stack_alloc(_ccxll, _items, _pbase, _pptr_)                           \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_items) > (_ccxll)->_pptr_##_limit && (_items) != 0)                  \
    {                                                                          \
        void  *_list = NULL;                                                   \
        size_t _sz   = sizeof(*(_ccxll)->_pptr_);                              \
                                                                               \
        _safe_alloc(_list, _sz * ((_ccxll)->_pptr_##_base + (_items)));        \
                                                                               \
        memcpy(_list, (_ccxll)->_pptr_, _sz * (_ccxll)->_pptr_##_base);        \
                                                                               \
        if ((_ccxll)->_pptr_ != NULL)                                          \
            _safe_free((_ccxll)->_pptr_);                                      \
        (_ccxll)->_pptr_ = _list;                                              \
                                                                               \
        memset((_ccxll)->_pptr_ + (_ccxll)->_pptr_##_base, 0, _sz * (_items)); \
                                                                               \
        (_ccxll)->_pptr_##_limit = (_items);                                   \
    }                                                                          \
                                                                               \
    *(_pbase) = (_ccxll)->_pptr_##_base;                                       \
    (_ccxll)->_pptr_##_base  += (_items);                                      \
    (_ccxll)->_pptr_##_limit -= (_items);                                      \
)


#define _stack_free(_ccxll, _pptr_)                                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_ccxll)->_pptr_ != NULL)                                              \
        _safe_free((_ccxll)->_pptr_);                                          \
                                                                               \
    (_ccxll)->_pptr_##_base  = 0;                                              \
    (_ccxll)->_pptr_##_limit = 0;                                              \
)



/* _it / _xl management */


#define _it_alloc(_ccxll, _items, _pbase)                                      \
                                                                               \
        _it_xl_alloc(_ccxll, _items, _pbase, ccxll_iter_init, _it, *)

#define _xl_alloc(_ccxll, _items, _pbase)                                      \
                                                                               \
        _it_xl_alloc(_ccxll, _items, _pbase, ccxll_init_from, _xl,  )

#define _it_xl_alloc(_ccxll, _items, _pbase, _pinit, _itxl_, _ASTRD_)          \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _stack_alloc((_ccxll), (_items), (_pbase), _itxl_);                        \
                                                                               \
    for (int _idx = *(_pbase); _idx < *(_pbase) + (_items); _idx++)            \
    {                                                                          \
        if ((_ccxll)->_itxl_[_idx] == NULL && *(#_ASTRD_) == '*')              \
            _safe_alloc((_ccxll)->_itxl_[_idx], sizeof(**(_ccxll)->_itxl_));   \
                                                                               \
        _pinit(_ASTRD_(_ccxll)->_itxl_[_idx], (_ccxll));                       \
    }                                                                          \
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
        _it_xl_free(_ccxll, _it)

#define _xl_free(_ccxll)                                                       \
                                                                               \
        _it_xl_free(_ccxll, _xl)

#define _it_xl_free(_ccxll, _itxl_)                                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    for (int _idx = 0; _idx < (_ccxll)->_itxl_##_limit; _idx++)                \
        if ((_ccxll)->_itxl_[_idx] != NULL)                                    \
            _safe_free((_ccxll)->_itxl_[_idx]);                                \
                                                                               \
    _stack_free((_ccxll), _itxl_);                                             \
)



#endif
