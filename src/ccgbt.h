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
        ccgbt_struct_extd(elem_t, 2, NORMAL)

#define ccgbt_struct_pckd(elem_t)                                              \
                                                                               \
        ccgbt_struct_extd(elem_t, 2, PACKED)

#define ccgbt_struct_extd(elem_t, _LvN_iter, _ALIGN_)                          \
                                                                               \
        struct CCGBT_BODY                                                      \
        {                                                                      \
            /* size and node record */                                         \
            int size,  used,  vcnt;                                            \
                                                                               \
            /* start: 1st alloc node num, ratio: growth ratio      */          \
            /* thrsh: max growth num                               */          \
            /* example: start = 2, ratio = 2, thrsh = 17           */          \
            /* -> 2, 4, 8, 16, 32, 17, 17,...                      */          \
            /* block increment info for linked list                */          \
            /* int start, ratio, thrsh;                            */          \
                                                                               \
            /* Depth to generate new branch when insert reach leaf */          \
            int start;                                                         \
                                                                               \
            struct CCGBT_NODE                                                  \
            {                                                                  \
                struct CCGBT_NODE *lnk[3];                                     \
                elem_t val;                                                    \
            }   *avsp, *pnode, *stnl;                                          \
        }



/* ccdll initialize */




#endif
