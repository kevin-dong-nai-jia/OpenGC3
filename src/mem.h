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


#define _it_alloc(_it, _alloc_items)  /* TODO */                               \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    size_t  _it_len  = 0;                                                      \
    void   *_it_list = NULL;                                                   \
                                                                               \
    if ((_it) != NULL)                                                         \
        while ((_it)[_it_len++] != NULL);                                      \
)


/* safe (de-)allocation */


#define _safe_alloc(_void_ptr, _alloc_bytes)                                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_void_ptr) != NULL)                                                   \
        _CC_ERROR(_CC_ERROR_MSG_MEMORY_LEAK);                                  \
    else                                                                       \
        (_void_ptr) = malloc((_alloc_bytes));                                  \
)


#define _safe_free(_void_ptr)                                                  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_void_ptr) == NULL)                                                   \
        _CC_ERROR(_CC_ERROR_MSG_DOUBLE_FREE);                                  \
    else                                                                       \
        free((_void_ptr)), (_void_ptr) = NULL;                                 \
)



/* error and fatal */


#define _CC_ERROR_MSG_MEMORY_LEAK "Potential Memory Leak Problem."
#define _CC_ERROR_MSG_DOUBLE_FREE "Potential Double Free Problem."

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
