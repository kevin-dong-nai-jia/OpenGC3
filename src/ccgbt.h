#ifndef CC_GBT_H
#define CC_GBT_H

#include "pool.h"
#include "misc.h"
#include "snym.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


/* synonym */

#define PRN lnk[0]
#define LFT lnk[1]
#define RGH lnk[2]


/* Attach line number to struct instance name.
 * To ensure each instance is unique.
 */

#define CCGBT       ADDID(CCGBT)
#define CCGBT_CONT  ADDID(CCGBT_CONT)
#define CCGBT_NODE  ADDID(CCGBT_NODE)
#define CCGBT_BLCK  ADDID(CCGBT_BLCK)
#define CCGBT_ITER  ADDID(CCGBT_ITER)
#define CCGBT_CURR  ADDID(CCGBT_CURR)



/* ccgbt instance create */


#define ccgbt(elem_t)                                                          \
                                                                               \
        ccgbt_extd(elem_t, 1, NORMAL)

#define ccgbt_pckd(elem_t)                                                     \
                                                                               \
        ccgbt_extd(elem_t, 1, PACKED)

#define ccgbt_extd(elem_t, _n_iter, _ALIGN_)                                   \
                                                                               \
        typedef ccgbt_struct_extd(elem_t, _n_iter, _ALIGN_) *CCGBT;  CCGBT


#define ccgbt_struct(elem_t)                                                   \
                                                                               \
        ccgbt_struct_extd(elem_t, 1, NORMAL)

#define ccgbt_struct_pckd(elem_t)                                              \
                                                                               \
        ccgbt_struct_extd(elem_t, 1, PACKED)

#define ccgbt_struct_extd(elem_t, _n_iter, _ALIGN_)                            \
                                                                               \
    struct CCGBT_CONT                                                          \
    {                                                                          \
        /* size and node record */                                             \
        /* size is used node, used is used nodes in latest blck */             \
        /* vcnt is new "fresh" node in latest blck              */             \
        int size,  used,  vcnt;                                                \
                                                                               \
        /* block increment info for linked list                */              \
        /* int start, ratio, thrsh;                            */              \
        int start, ratio, thrsh;                                               \
                                                                               \
        /* root is sentinel/root node */                                       \
        struct CCGBT_NODE                                                      \
        {   struct CCGBT_NODE *lnk[3];                                         \
            elem_t val;                                                        \
            /* *pnode is aux blck */                                           \
        }   *avsp, *pnode, root;                                               \
                                                                               \
        /* A group of nodes, is the instance of pool.h, and managed by it */   \
        /* start: 1st alloc node num in a blck, ratio: growth ratio       */   \
        /* thrsh: max nodes in a blck growth num                          */   \
        /* look for int start, ratio, thrsh                               */   \
        /* example: start = 2, ratio = 2, thrsh = 17                      */   \
        /* -> 2, 4, 8, 16, 32, 17, 17,... nodes per blcks next time.      */   \
        struct CCGBT_BLCK                                                      \
        {   struct CCGBT_BLCK *next;          /* points to next block */       \
            PRAGMA_##_ALIGN_##_BGN            /* packed pragma starts */       \
            struct CCGBT_NODE nodes[1];       /* node structure array */       \
            PRAGMA_##_ALIGN_##_END            /* the pragma ends here */       \
            /* *pblock is aux blck */                                          \
        }   *pool, *pblock;                   /* points to 1-st block */       \
                                                                               \
        struct CCGBT_ITER                                                      \
        {   struct CCGBT_CURR                                                  \
            {   struct CCGBT_NODE *node;                                       \
            }   curr;                             /* points to curr   node */  \
            struct CCGBT_CONT *ccgbt;             /* points to ccg  body */    \
        }   (*itarr)[_n_iter], *_iter, **_it;     /* **it_: Auxiliary iters*/  \
                                                                               \
        /* Auxiliary container for special function */                         \
        struct CCGBT_CONT **_co;                  /* internal use _it _co */   \
                                                                               \
        unsigned char _it_base, _it_limit;                                     \
        unsigned char _co_base, _co_limit;                                     \
    }



/* ccgbt initialize */


#define ccgbt_init(_ccgbt)                                                     \
                                                                               \
        ccgbt_init_extd(_ccgbt, 1 << 4, 1 << 1, 1 << 16)

#define ccgbt_init_extd(_ccgbt, _start, _ratio, _thrsh)                        \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_ccgbt) = NULL;                                                           \
                                                                               \
    _ccgbt_init_extd((_ccgbt), (_start), (_ratio), (_thrsh), 1);               \
                                                                               \
    _itarr_alloc((_ccgbt), ccgbt);                                             \
    _ccgbt_iter_init((_ccgbt)->_iter, (_ccgbt), 1);                            \
)


#define _ccgbt_init(_ccgbt_dst, _ccgbt_src, _alloc)                            \
                                                                               \
        _ccgbt_init_extd(_ccgbt_dst, -1,     -1,     -1, _alloc)

#define _ccgbt_init_extd(_ccgbt, _start, _ratio, _thrsh, _alloc)               \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_alloc))                                                              \
        _cont_alloc((_ccgbt));                                                 \
                                                                               \
    _ccgbt_init_core((_ccgbt));                                                \
    _ccgbt_init_info((_ccgbt), (_start), (_ratio), (_thrsh));                  \
)


#define _ccgbt_init_core(_ccgbt)                                               \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    _ccgbt_init_seed((_ccgbt)),                                                \
                                                                               \
    (_ccgbt)->used = (_ccgbt)->vcnt   = 0,                                     \
    (_ccgbt)->avsp = (_ccgbt)->pnode  = NULL,                                  \
    (_ccgbt)->pool = (_ccgbt)->pblock = NULL,                                  \
                                                                               \
    (_ccgbt)->itarr = NULL,                                                    \
    (_ccgbt)->_iter = NULL,                                                    \
    (_ccgbt)->_it   = NULL,                                                    \
    (_ccgbt)->_co   = NULL,                                                    \
    (_ccgbt)->_it_base = (_ccgbt)->_it_limit = 0,                              \
    (_ccgbt)->_co_base = (_ccgbt)->_co_limit = 0                               \
)


#define _ccgbt_init_seed(_ccgbt)                                               \
/* when return value is meaningless, use VOID_EXPR_() */                       \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccgbt)->size = 0,                                                        \
    (_ccgbt)->root.PRN = NULL,                                                 \
    (_ccgbt)->root.LFT = NULL,                                                 \
    (_ccgbt)->root.RGH = NULL                                                  \
)


#define _ccgbt_init_info(_ccgbt, _start, _ratio, _thrsh)                       \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccgbt)->start = ((_start) > 0) ? (_start) : 1,                           \
    (_ccgbt)->ratio = ((_ratio) > 0) ? (_ratio) : 1,                           \
    (_ccgbt)->thrsh = ((_thrsh) > (_ccgbt)->start) ? (_thrsh) : (_ccgbt)->start\
)


#define ccgbt_iter_init(_iter, _ccgbt)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = NULL,                                                 \
    (_iter)->ccgbt = (_ccgbt)                                                  \
)


#define _ccgbt_iter_init(_iter, _ccgbt, _alloc)                                \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_alloc))                                                              \
        _iter_alloc((_iter));                                                  \
                                                                               \
    ccgbt_iter_init((_iter), (_ccgbt));                                        \
)



/* ccgbt destroy */


#define ccgbt_free(_ccgbt)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_free    ((_ccgbt));                                                    \
    _co_free    ((_ccgbt));                                                    \
                                                                               \
    _iter_free  ((_ccgbt)->_iter);                                             \
    _itarr_free ((_ccgbt));                                                    \
    _block_free ((_ccgbt));                                                    \
    _cont_free  ((_ccgbt));                                                    \
)



/* ccgbt access */


#define ccgbt_root(_ccgbt)  ((_ccgbt)->root.val)



/* ccgbt capacity */


#define ccgbt_size(_ccgbt)   ((_ccgbt)->size)

#define ccgbt_empty(_ccgbt)  ((ccgbt_size((_ccgbt))) == 0)



/* ccgbt modifiers */


#define ccgbt_left(_iter)   ((_iter)->curr.node->LFT->val)

#define ccgbt_right(_iter)  ((_iter)->curr.node->RGH->val)

/* insert a new node with value (_val) to [LFT|RGH]  */
#define  ccgbt_insert_left(_iter, _val)                                        \
                                                                               \
        _ccgbt_insert(_iter, _val, LFT, RGH)

#define  ccgbt_insert_right(_iter, _val)                                       \
                                                                               \
        _ccgbt_insert(_iter, _val, RGH, LFT)
/* _targ_: target side, _oppo_: opposite side */
#define _ccgbt_insert(_iter, _val, _targ_, _oppo_)                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    /* _node_alloc(_pnode,_cont) : to alloc a new node (provided by pool.h) */ \
    /* _pnode = pointer to newly allocated node                             */ \
    /* _cont  = pointer to this container. @(_iter)-ccgbt                   */ \
                                                                               \
    _node_alloc((_iter)->ccgbt->pnode, (_iter)->ccgbt);                        \
                                                                               \
    (_iter)->ccgbt->pnode->val = (_val);                                       \
                                                                               \
    (_iter)->ccgbt->pnode->_oppo_ = NULL;                                      \
    (_iter)->ccgbt->pnode->_targ_ = (_iter)->curr.node->_targ_;                \
    (_iter)->ccgbt->pnode->PRN = (_iter)->curr.node;                           \
                                                                               \
    if ((_iter)->curr.node->_targ_)                                            \
        (_iter)->curr.node->_targ_->PRN = (_iter)->ccgbt->pnode;               \
                                                                               \
    (_iter)->curr.node->_targ_ = (_iter)->ccgbt->pnode;                        \
                                                                               \
    (_iter)->ccgbt->size++;                                                    \
)


/* insert a new node with value (_val) to [LFT|RGH]                     */
/* and move the other chile tree to the corresponding site of new node  */
#define  ccgbt_push_left(_iter, _val)                                          \
                                                                               \
        _ccgbt_push(_iter, _val, LFT, RGH)

#define  ccgbt_push_right(_iter, _val)                                         \
                                                                               \
        _ccgbt_push(_iter, _val, RGH, LFT)
/* _targ_: target side, _oppo_: opposite side */
#define _ccgbt_push(_iter, _val, _targ_, _oppo_)                               \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _ccgbt_insert((_iter), (_val), _targ_, _oppo_);                            \
                                                                               \
    (_iter)->curr.node->_targ_->_oppo_ = (_iter)->curr.node->_oppo_;           \
    (_iter)->curr.node->_oppo_         = NULL;                                 \
)


#define  ccgbt_erase_left(_iter)                                               \
                                                                               \
        _ccgbt_erase(_iter, LFT, RGH)

#define  ccgbt_erase_right(_iter)                                              \
                                                                               \
        _ccgbt_erase(_iter, RGH, LFT)
/* _targ_: target side, _oppo_: opposite side */
#define _ccgbt_erase(_iter, _targ_, _oppo_)                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely(ccgbt_iter_at_root((_iter))))  break;                        \
                                                                               \
    /* alloc a group of new aux iters.       */                                \
    /* In this case, we alloc two aux iters. */                                \
    _it_alloc((_iter)->ccgbt, 2, _base_e, ccgbt);                              \
                                                                               \
    _ccgbt_erase_repl((_iter),                                                 \
                      _it_((_iter)->ccgbt, _base_e, 0),                        \
                      _it_((_iter)->ccgbt, _base_e, 1), _targ_, _oppo_);       \
                                                                               \
    _it_clear((_iter)->ccgbt, 2);                                              \
)

#define _ccgbt_erase_repl(_iter, _iter_a, _iter_b, _targ_, _oppo_)             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    /* set aux iter[0] to target child */                                      \
    (_iter_a)->curr.node = (_iter)->curr.node->_targ_;                         \
                                                                               \
    /* connect parent node._oppo_ to the sibling. */                           \
    if ((_iter)->curr.node == (_iter)->curr.node->PRN->_oppo_)                 \
        (_iter)->curr.node->PRN->_oppo_ = (_iter_a)->curr.node;                \
    else                                                                       \
        (_iter)->curr.node->PRN->_targ_ = (_iter_a)->curr.node;                \
                                                                               \
    (_iter_a)->curr.node->PRN = (_iter)->curr.node->PRN;                       \
                                                                               \
    /* set aux iter[1] to the other child */                                   \
    (_iter_b)->curr.node = (_iter)->curr.node->_oppo_;                         \
                                                                               \
    /* iterate to the end of sibling */                                        \
    while (_ccgbt_iter_move((_iter_a), _oppo_));                               \
                                                                               \
    /* paste the other child to the end of sibling */                          \
    (_iter_a)->curr.node->_oppo_ = (_iter_b)->curr.node;                       \
                                                                               \
    if ((_iter_a)->curr.node->_oppo_)                                          \
        (_iter_a)->curr.node->_oppo_->PRN = (_iter_a)->curr.node;              \
                                                                               \
    (_iter_a)->curr.node = (_iter)->curr.node->PRN;                            \
                                                                               \
    /* clear current node */                                                   \
    _node_clear((_iter)->curr.node, (_iter)->ccgbt);                           \
                                                                               \
    /* move _iter to parent */                                                 \
    (_iter)->curr.node = (_iter_a)->curr.node;                                 \
                                                                               \
    (_iter)->ccgbt->size--;                                                    \
)

/* swap two child subtrees */
#define ccgbt_swap_subtree(_iter)                                              \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    void *_tmp = (_iter)->curr.node->LFT;                                      \
                                                                               \
    (_iter)->curr.node->LFT = (_iter)->curr.node->RGH;                         \
    (_iter)->curr.node->RGH = _tmp;                                            \
)

/* swap two containers */
#define ccgbt_swap(_ccgbt_a, _ccgbt_b)                                         \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    void *_bup = (_ccgbt_a);                                                   \
                                                                               \
    (_ccgbt_a) = (_ccgbt_b);                                                   \
    (_ccgbt_b) = _bup;                                                         \
)



/* ccgbt operations */


/* ccgbt comparators */


/* ccgbt iterators */


#define ccgbt_iter_copy(_iter_dst, _iter_src)                                  \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    *(_iter_dst) = *(_iter_src)                                                \
)


#define ccgbt_iter_root(_iter)                                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = &((_iter)->ccgbt->root)                               \
)


#define  ccgbt_iter_left(_iter)    _ccgbt_iter_move(_iter, LFT)

#define  ccgbt_iter_right(_iter)   _ccgbt_iter_move(_iter, RGH)

#define  ccgbt_iter_parent(_iter)  _ccgbt_iter_move(_iter, PRN)

#define _ccgbt_iter_move(_iter, _dir_)                                         \
(                                                                              \
    ((_iter)->curr.node->_dir_) ?                                              \
    ((_iter)->curr.node = (_iter)->curr.node->_dir_)->_dir_ : (NULL)           \
)


#define ccgbt_iter_at_root(_iter)                                              \
(                                                                              \
    (_iter)->curr.node == &((_iter)->ccgbt->root)                              \
)


#define ccgbt_iter_at_leaf(_iter)                                              \
(                                                                              \
    (ccgbt_iter_no_left(_iter) && ccgbt_iter_no_right(_iter))                  \
)


#define ccgbt_iter_no_left(_iter)                                              \
(                                                                              \
    ((_iter)->curr.node->LFT == NULL)                                          \
)


#define ccgbt_iter_no_right(_iter)                                             \
(                                                                              \
    ((_iter)->curr.node->RGH == NULL)                                          \
)



/* ccgbt traversor */


#ifndef CCC_STRICT

#define ccgbt_infix_auto(_pval, _iter, _expr_)                                 \
                                                                               \
     ccgbt_iter_root(_iter);                                                   \
                                                                               \
     for (__typeof__((_ccdll)->pnode->val) *_pval;                             \
                                              )                                \
 

#endif // !CCC_STRICT





#endif
