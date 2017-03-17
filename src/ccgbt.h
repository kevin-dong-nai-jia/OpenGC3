#ifndef CC_GBT_H
#define CC_GBT_H

#include "pool.h"
#include "misc.h"
#include "snym.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


/* synonym       */

#define PRN lnk[0]
#define LFT lnk[1]
#define RGH lnk[2]


/* Attach line number to struct instance name.
 * To ensure each instance is unique.
 */

#define CCGBT       ADDID(CCGBT)
#define CCGBT_BODY  ADDID(CCGBT_BODY)
#define CCGBT_NODE  ADDID(CCGBT_NODE)
#define CCGBT_BLCK  ADDID(CCGBT_BLCK)
#define CCGBT_ITER  ADDID(CCGBT_ITER)
#define CCGBT_CURR  ADDID(CCGBT_CURR)



/* ccgbt instance create */


#define ccgbt(elem_t)                                                          \
                                                                               \
        ccgbt_extd(elem_t, NORMAL)

#define ccgbt_pckd(elem_t)                                                     \
                                                                               \
        ccgbt_extd(elem_t, PACKED)

#define ccgbt_extd(elem_t, _ALIGN_)		                                       \
                                                                               \
        typedef ccgbt_struct_extd(elem_t, _ALIGN_) *CCGBT;  CCGBT


#define ccgbt_struct(elem_t)                                                   \
                                                                               \
        ccgbt_struct_extd(elem_t, 1, NORMAL)

#define ccgbt_struct_pckd(elem_t)                                              \
                                                                               \
        ccgbt_struct_extd(elem_t, 1, PACKED)

#define ccgbt_struct_extd(elem_t, _n_iter, _ALIGN_)                            \
                                                                               \
    struct CCGBT_BODY                                                          \
    {                                                                          \
        /* size and node record */                                             \
        int size,  used,  vcnt;                                                \
                                                                               \
        /* block increment info for linked list                */              \
        /* int start, ratio, thrsh;                            */              \
        int start, ratio, thrsh;                                               \
                                                                               \
        struct CCGBT_NODE                                                      \
        {                                                                      \
            struct CCGBT_NODE *lnk[3];                                         \
            elem_t val;                                                        \
        }   *avsp, *pnode, *sentinel;                                          \
                                                                               \
        /* A group of nodes, is the instance of pool.h, and managed by it */   \
        /* start: 1st alloc node num in a blck, ratio: growth ratio       */   \
        /* thrsh: max nodes in a blck growth num                          */   \
        /* look for int start, ratio, thrsh                               */   \
        /* example: start = 2, ratio = 2, thrsh = 17                      */   \
        /* -> 2, 4, 8, 16, 32, 17, 17,... nodes per blcks next time.      */   \
        struct CCGBT_BLCK                                                      \
        {                                                                      \
            struct CCGBT_BLCK *next;          /* points to next block */       \
            PRAGMA_##_ALIGN_##_BGN            /* packed pragma starts */       \
            struct CCGBT_NODE nodes[1];       /* node structure array */       \
            PRAGMA_##_ALIGN_##_END            /* the pragma ends here */       \
        }   *pool, *pblock                    /* points to 1-st block */       \
                                                                               \
                                                                               \
        struct CCGBT_ITER                                                      \
        {   struct CCGBT_CURR                                                  \
            {   struct CCGBT_NODE *node;                                       \
            }   curr;                             /* points to curr   node */  \
            struct CCGBT_BODY *ccgbt;             /* points to ccdll  body */  \
        }   (*itarr)[_n_iter], *_iter, **_it;     /* **it_: Auxiliary iters*/  \
                                                                               \
                                                                               \
        /* Auxiliary container for special function */                         \
        struct CCGBT_BODY **_co;                  /* internal use _it _co */   \
                                                                               \
        unsigned char _it_base, _it_limit;                                     \
        unsigned char _co_base, _co_limit;                                     \                                                                 \
    }



/* ccdll initialize */


#define ccgbt_init(_ccgbt)                                                     \
                                                                               \
        ccgbt_init_extd(_ccgbt, 1 << 4, 1 << 1, 1 << 16)

#endif
