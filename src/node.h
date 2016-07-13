#ifndef _NODE_H_
#define _NODE_H_

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
            link_t pool_dup = (_cc_ll).pool;                                   \
                                                                               \
            if ((_cc_ll).ncnt == 0)                                            \
                (_cc_ll).vcnt = ((_cc_ll).ncnt  = (_cc_ll).blkstart);          \
            else                                                               \
                (_cc_ll).vcnt = ((_cc_ll).ncnt  < (_cc_ll).blkthrsh) ?         \
                                ((_cc_ll).ncnt *= (_cc_ll).blkratio) :         \
                                ((_cc_ll).ncnt  = (_cc_ll).blkthrsh);          \
                                                                               \
            (_cc_ll).pool = malloc((sizeof((_cc_ll).block)) +                  \
                                   (sizeof((_cc_ll).block.nodes[0])) *         \
                                   ((_cc_ll).ncnt - 1));                       \
                                                                               \
            if ((_cc_ll).pool == NULL)                                         \
                fputs("FATAL ERROR: Failed To Allocate", stderr),              \
                exit(EXIT_FAILURE);                                            \
                                                                               \
            *(link_t*)(_cc_ll).pool = pool_dup;                                \
        }                                                                      \
                                                                               \
        (_pnode) = &((_cc_ll).pool->nodes[--(_cc_ll).vcnt]);                   \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        (_pnode) = (void*)((_cc_ll).avsp + (_cc_ll).val_offset);               \
        (_cc_ll).avsp = *(link_t*)(_cc_ll).avsp;                               \
    }                                                                          \
)


#define _node_clear(_plink, _cc_ll)                                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    *(link_t*)(_plink) = (_cc_ll).avsp;                                        \
    (_cc_ll).avsp = (_plink);                                                  \
)


#define _nodes_free(_cc_ll)                                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while ((_cc_ll).pool != NULL)                                              \
    {                                                                          \
        link_t pool_dup = (_cc_ll).pool;                                       \
                                                                               \
        (_cc_ll).pool = *(link_t*)(_cc_ll).pool;                               \
        free(pool_dup);                                                        \
    }                                                                          \
)


#endif
