#ifndef _CC_MEM_H_
#define _CC_MEM_H_

#include <stdio.h>
#include <stdlib.h>


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
                fputs("FATAL ERROR: Failed To Allocate Memory", stderr),       \
                exit(EXIT_FAILURE);                                            \
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


#endif
