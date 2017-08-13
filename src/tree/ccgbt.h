#ifndef OPENGC3_TREE_CCGBT_H
#define OPENGC3_TREE_CCGBT_H

#include "../base/pool.h"
#include "../base/misc.h"
#include "../base/snym.h"


#include <stdint.h>
#include <stdbool.h>


/* ccgbt create */


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
        int size,  last,  vcnt;                                                \
        int start, ratio, thrsh;                                               \
                                                                               \
        struct CCGBT_NODE                                                      \
        {   struct CCGBT_NODE *lnk[3];                                         \
            uint8_t flag;                                                      \
            elem_t  val;                                                       \
        }   *avsp, *pnode, root;                                               \
                                                                               \
        /* start: 1st alloc node num in a blck, ratio: growth ratio       */   \
        /* thrsh: maximum* amount of nodes in a single block              */   \
        /* Ex. start = 2, ratio = 2, thrsh = 17:                          */   \
        /* ncnt = 2 -> 4 -> 8 -> 16 -> 32 -> 17 -> 17, ...                */   \
                                                                               \
        struct CCGBT_BLCK                                                      \
        {   struct CCGBT_BLCK *bprv, *bnxt;       /* next and prev blocks */   \
            PRAGMA_##_ALIGN_##_BGN                /* packed pragma starts */   \
            int ncnt;                             /* the item of the node */   \
            struct CCGBT_NODE nodes[];            /* node structure array */   \
            PRAGMA_##_ALIGN_##_END                /* the pragma ends here */   \
        }   *pool, *pblock;                       /* points to 1-st block */   \
                                                                               \
        struct CCGBT_ITER                                                      \
        {   struct CCGBT_PTRS                                                  \
            {   struct CCGBT_NODE *node;                                       \
            }   curr;                             /* points to curr  node */   \
            struct CCGBT_CONT *ccgbt;             /* points to CCGBT body */   \
        }   (*itarr)[_n_iter], *_iter, **_it;                                  \
                                                                               \
        struct CCGBT_CONT **_co;                  /* internal use _it _co */   \
                                                                               \
        int _it_base, _it_limit;                                               \
        int _co_base, _co_limit;                                               \
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
    _itarr_init((_ccgbt), ccgbt);                                              \
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
    (_ccgbt)->last = (_ccgbt)->vcnt   = 0,                                     \
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
                                                                               \
VOID_EXPR_  /* when return value is meaningless, use VOID_EXPR_() */           \
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

#define ccgbt_left(_iter)   ((_iter)->curr.node->LFT->val)

#define ccgbt_right(_iter)  ((_iter)->curr.node->RGH->val)



/* ccgbt capacity */


#define ccgbt_size(_ccgbt)   ((_ccgbt)->size)

#define ccgbt_empty(_ccgbt)  ((ccgbt_size((_ccgbt))) == 0)



/* ccgbt modifiers */


/* insert a new node with the value to the left/right  */

#define  ccgbt_insert_left(_iter, _val)   _ccgbt_insert(_iter, _val, LFT, RGH)

#define  ccgbt_insert_right(_iter, _val)  _ccgbt_insert(_iter, _val, RGH, LFT)

#define _ccgbt_insert(_iter, _val, _targ_, _oppo_)                             \
                   /* _targ_: target side, _oppo_: opposite side */            \
STATEMENT_                                                                     \
(                                                                              \
    _node_alloc((_iter)->ccgbt->pnode, (_iter)->ccgbt);                        \
                                                                               \
    (_iter)->ccgbt->pnode->val = (_val);                                       \
                                                                               \
    (_iter)->ccgbt->pnode->_oppo_ = NULL;                                      \
    (_iter)->ccgbt->pnode->_targ_ = (_iter)->curr.node->_targ_;                \
    (_iter)->ccgbt->pnode->PRN = (_iter)->curr.node;                           \
    (_iter)->ccgbt->pnode->flag = 0;                                           \
                                                                               \
    if ((_iter)->curr.node->_targ_)                                            \
        (_iter)->curr.node->_targ_->PRN = (_iter)->ccgbt->pnode;               \
                                                                               \
    (_iter)->curr.node->_targ_ = (_iter)->ccgbt->pnode;                        \
                                                                               \
    (_iter)->ccgbt->size++;                                                    \
)


/* insert a new node with the value to the left/right                */
/* and move the other subtree to the corresponding site of new node  */

#define  ccgbt_push_left(_iter, _val)   _ccgbt_push(_iter, _val, LFT, RGH)

#define  ccgbt_push_right(_iter, _val)  _ccgbt_push(_iter, _val, RGH, LFT)

#define _ccgbt_push(_iter, _val, _targ_, _oppo_)                               \
                 /* _targ_: target side, _oppo_: opposite side */              \
STATEMENT_                                                                     \
(                                                                              \
    _ccgbt_insert((_iter), (_val), _targ_, _oppo_);                            \
                                                                               \
    if ((_iter)->curr.node->_oppo_)                                            \
        (_iter)->curr.node->_oppo_->PRN = (_iter)->curr.node->_targ_;          \
                                                                               \
    (_iter)->curr.node->_targ_->_oppo_ = (_iter)->curr.node->_oppo_;           \
    (_iter)->curr.node->_oppo_         = NULL;                                 \
)


#define  ccgbt_erase_left(_iter)   _ccgbt_erase_dir(_iter, LFT, RGH)

#define  ccgbt_erase_right(_iter)  _ccgbt_erase_dir(_iter, RGH, LFT)

#define _ccgbt_erase_dir(_iter, _targ_, _oppo_)                                \
                      /* _targ_: target side, _oppo_: opposite side */         \
STATEMENT_                                                                     \
(                                                                              \
    if (_unlikely(ccgbt_iter_at_root((_iter))))  break;                        \
                                                                               \
    _it_init((_iter)->ccgbt, 2, _base_e, ccgbt);                               \
                                                                               \
    _ccgbt_erase(     (_iter),                                                 \
                 _it_((_iter)->ccgbt, _base_e, 0),                             \
                 _it_((_iter)->ccgbt, _base_e, 1), _targ_, _oppo_);            \
                                                                               \
    _it_clear((_iter)->ccgbt, 2);                                              \
)

#define _ccgbt_erase(_iter, _iter_a, _iter_b, _targ_, _oppo_)                  \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    /* set aux iter[0] to the target child */                                  \
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
    while (_ccgbt_iter_dir((_iter_a), _oppo_));                                \
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


#define  ccgbt_iter_left(_iter)    _ccgbt_iter_dir(_iter, LFT)

#define  ccgbt_iter_right(_iter)   _ccgbt_iter_dir(_iter, RGH)

#define  ccgbt_iter_parent(_iter)  _ccgbt_iter_dir(_iter, PRN)

#define _ccgbt_iter_dir(_iter, _dir_)                                          \
(                                                                              \
    ((_iter)->curr.node->_dir_) ?                                              \
    ((_iter)->curr.node = (_iter)->curr.node->_dir_)->_dir_ : (NULL)           \
)


#define ccgbt_iter_at_root(_iter)                                              \
(                                                                              \
    ((_iter)->curr.node == &((_iter)->ccgbt->root))                            \
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


#define CCGBT_LFT_SEEK 0
#define CCGBT_RGH_SEEK 1
#define CCGBT_PRN_SEEK 2


typedef struct
{   void*   root;
    void*   last;
    int     depth;
    uint8_t seek;
}   ccgbt_trav_t;

#define CCGBT_INORDER(_iter, _stat)                                            \
                                                                               \
    for (((_stat)->root  = (_iter)->curr.node,                                 \
          (_stat)->last  = NULL,                                               \
          (_stat)->depth = 0,                                                  \
          (_stat)->seek  = CCGBT_LFT_SEEK);                                    \
         (_iter)->curr.node != (_stat)->last; )

#define ccgbt_iter_restore(_iter, _stat)                                       \
(                                                                              \
    (_iter)->curr.node = (_stat)->root                                         \
)

#define ccgbt_iter_inorder(_iter, _stat)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_stat)->last == NULL)                                                 \
        _ccgbt_find_inorder_rightmost((_iter), (_stat));                       \
                                                                               \
                                                                               \
    if ((_stat)->seek == CCGBT_LFT_SEEK)                                       \
        _ccgbt_iter_inorder_leftmost ((_iter), (_stat));                       \
                                                                               \
    else if ((_stat)->seek == CCGBT_PRN_SEEK)                                  \
        _ccgbt_iter_inorder_parent   ((_iter), (_stat));                       \
                                                                               \
    else /* if ((_stat)->seek == CCGBT_RGH_SEEK) */                            \
        _ccgbt_iter_inorder_right    ((_iter), (_stat));                       \
)

#define _ccgbt_find_inorder_rightmost(_iter, _stat)                            \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    void *rbup = (_iter)->curr.node;                                           \
                                                                               \
    while (ccgbt_iter_right((_iter)));                                         \
                                                                               \
    (_stat)->last = (_iter)->curr.node;                                        \
    (_iter)->curr.node = rbup;                                                 \
)

#define _ccgbt_iter_inorder_leftmost(_iter, _stat)                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while (true)                                                               \
    {                                                                          \
        /* The flag of the node without left child is set to 0,                \
           because it'll be the target for following traversal. */             \
                                                                               \
        ccgbt_flag_set((_iter), CCGBT_FLAG_TRAV_OFFSET);                       \
                                                                               \
        if (ccgbt_iter_no_left((_iter)))                                       \
        {                                                                      \
            if (ccgbt_iter_no_right((_iter)))                                  \
                (_stat)->seek = CCGBT_PRN_SEEK;                                \
            else                                                               \
                (_stat)->seek = CCGBT_RGH_SEEK;                                \
            break;                                                             \
        }                                                                      \
                                                                               \
        ccgbt_iter_left((_iter));                                              \
                                                                               \
        (_stat)->depth++;                                                      \
    }                                                                          \
)

#define _ccgbt_iter_inorder_parent(_iter, _stat)                               \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    ccgbt_flag_unset((_iter), CCGBT_FLAG_TRAV_OFFSET);                         \
                                                                               \
    while (1)                                                                  \
    {                                                                          \
        (_stat)->depth--;                                                      \
                                                                               \
        ccgbt_iter_parent((_iter));                                            \
                                                                               \
        if (ccgbt_flag_get((_iter), CCGBT_FLAG_TRAV_OFFSET))                   \
        {                                                                      \
            (_stat)->seek = ((ccgbt_iter_no_right((_iter)) ?                   \
                             CCGBT_PRN_SEEK : CCGBT_RGH_SEEK));                \
            break;                                                             \
        }                                                                      \
    }                                                                          \
)

#define _ccgbt_iter_inorder_right(_iter, _stat)                                \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_stat)->depth++;                                                          \
                                                                               \
    ccgbt_flag_unset((_iter), CCGBT_FLAG_TRAV_OFFSET);                         \
                                                                               \
    /* Set flag for the first node at the right branch for later use. */       \
    ccgbt_iter_right((_iter));                                                 \
    ccgbt_flag_set  ((_iter), CCGBT_FLAG_TRAV_OFFSET);                         \
                                                                               \
    if (ccgbt_iter_no_left((_iter)))                                           \
    {                                                                          \
        if (ccgbt_iter_no_right((_iter)))                                      \
            (_stat)->seek = CCGBT_PRN_SEEK;                                    \
        else                                                                   \
            (_stat)->seek = CCGBT_RGH_SEEK;                                    \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        _ccgbt_iter_inorder_leftmost((_iter), (_stat));                        \
    }                                                                          \
)



/* ccgbt flag */


#define CCGBT_FLAG_TRAV_OFFSET  (0)


#define ccgbt_flag_get(_iter, _offs)                                           \
(                                                                              \
    (((_iter)->curr.node->flag) & (uint8_t)(1 << (_offs))) >> (_offs)          \
)


#define ccgbt_flag_set(_iter, _offs)                                           \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    ((_iter)->curr.node->flag) |= (uint8_t)0x1 << (_offs)                      \
)


#define ccgbt_flag_toggle(_iter, _offs)                                        \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    ((_iter)->curr.node->flag) ^= (uint8_t)0x1 << (_offs)                      \
)


#define ccgbt_flag_unset(_iter, _offs)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    ccgbt_flag_set   ((_iter), (_offs)),                                       \
    ccgbt_flag_toggle((_iter), (_offs))                                        \
)



#endif
