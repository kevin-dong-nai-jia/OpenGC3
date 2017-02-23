#ifndef CC_DLL_H
#define CC_DLL_H

#include "pool.h"
#include "misc.h"
#include "snym.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


/* synonym */

#define PRV lnk[0]
#define NXT lnk[1]

#define CCDLL       ADDID(CCDLL)
#define CCDLL_BODY  ADDID(CCDLL_BODY)
#define CCDLL_NODE  ADDID(CCDLL_NODE)
#define CCDLL_BLCK  ADDID(CCDLL_BLCK)
#define CCDLL_ITER  ADDID(CCDLL_ITER)
#define CCDLL_CURR  ADDID(CCDLL_CURR)


/* ccdll create */


#define ccdll(elem_t)                                                          \
                                                                               \
        ccdll_extd(elem_t, 1, NORMAL)

#define ccdll_pckd(elem_t)                                                     \
                                                                               \
        ccdll_extd(elem_t, 1, PACKED)

#define ccdll_extd(elem_t, _n_iter, _ALIGN_)                                   \
                                                                               \
        typedef ccdll_struct_extd(elem_t, _n_iter, _ALIGN_) *CCDLL;  CCDLL


#define ccdll_struct(elem_t)                                                   \
                                                                               \
        ccdll_struct_extd(elem_t, 1, NORMAL)

#define ccdll_struct_pckd(elem_t)                                              \
                                                                               \
        ccdll_struct_extd(elem_t, 1, PACKED)

#define ccdll_struct_extd(elem_t, _n_iter, _ALIGN_)                            \
                                                                               \
    struct CCDLL_BODY                                                          \
    {                                                                          \
        int size,  used,  vcnt;                   /* size and node record */   \
        int start, ratio, thrsh;                  /* block increment info */   \
                                                                               \
        struct CCDLL_NODE                                                      \
        {   struct CCDLL_NODE *lnk[2];                                         \
            elem_t val;                           /* val with prv and nxt */   \
        }   *avsp, *pnode, head, tail;            /* available space list */   \
                                                                               \
        struct CCDLL_BLCK                                                      \
        {   struct CCDLL_BLCK *next;              /* points to next block */   \
            PRAGMA_##_ALIGN_##_BGN                /* packed pragma starts */   \
            struct CCDLL_NODE nodes[1];           /* node structure array */   \
            PRAGMA_##_ALIGN_##_END                /* the pragma ends here */   \
        }   *pool, *pblock;                       /* points to 1-st block */   \
                                                                               \
        struct CCDLL_ITER                                                      \
        {   struct CCDLL_CURR                                                  \
            {   struct CCDLL_NODE *node;                                       \
            }   curr;                             /* points to curr  node */   \
            struct CCDLL_BODY *ccdll;             /* points to ccdll body */   \
        }   (*itarr)[_n_iter], *_iter, **_it;                                  \
                                                                               \
        struct CCDLL_BODY **_ll;                  /* internal use _it _ll */   \
                                                                               \
        unsigned char _it_base, _it_limit;                                     \
        unsigned char _ll_base, _ll_limit;                                     \
    }



/* ccdll initialize */


#define ccdll_init(_ccdll)                                                     \
                                                                               \
        ccdll_init_extd(_ccdll, 1 << 4, 1 << 1, 1 << 16)

#define ccdll_init_from(_ccdll_dst, _ccdll_src)                                \
                                                                               \
        ccdll_init_extd((_ccdll_dst),        (_ccdll_src)->start,              \
                        (_ccdll_src)->ratio, (_ccdll_src)->thrsh)

#define ccdll_init_extd(_ccdll, _start, _ratio, _thrsh)                        \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    (_ccdll) = NULL;                                                           \
                                                                               \
    _ccdll_init_extd((_ccdll), (_start), (_ratio), (_thrsh));                  \
                                                                               \
    _itarr_alloc((_ccdll), ccdll);                                             \
    _ccdll_iter_init((_ccdll)->_iter, (_ccdll));                               \
)


#define _ccdll_init_from(_ccdll_dst, _ccdll_src)                               \
                                                                               \
        _ccdll_init_extd((_ccdll_dst),        (_ccdll_src)->start,             \
                         (_ccdll_src)->ratio, (_ccdll_src)->thrsh)

#define _ccdll_init_extd(_ccdll, _start, _ratio, _thrsh)                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _cc_ll_alloc((_ccdll));                                                    \
                                                                               \
    _ccdll_init_core((_ccdll));                                                \
    _ccdll_init_info((_ccdll), (_start), (_ratio), (_thrsh));                  \
)


#define _ccdll_init_core(_ccdll)                                               \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    _ccdll_init_seed((_ccdll)),                                                \
                                                                               \
    (_ccdll)->used = (_ccdll)->vcnt   = 0,                                     \
    (_ccdll)->avsp = (_ccdll)->pnode  = NULL,                                  \
    (_ccdll)->pool = (_ccdll)->pblock = NULL,                                  \
                                                                               \
    (_ccdll)->itarr = NULL,                                                    \
    (_ccdll)->_iter = NULL,                                                    \
    (_ccdll)->_it   = NULL,                                                    \
    (_ccdll)->_ll   = NULL,                                                    \
    (_ccdll)->_it_base = (_ccdll)->_it_limit = 0,                              \
    (_ccdll)->_ll_base = (_ccdll)->_ll_limit = 0                               \
)


#define _ccdll_init_seed(_ccdll)                                               \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccdll)->size = 0,                                                        \
    (_ccdll)->head.PRV = NULL,                                                 \
    (_ccdll)->head.NXT = &((_ccdll)->tail),                                    \
    (_ccdll)->tail.PRV = &((_ccdll)->head),                                    \
    (_ccdll)->tail.NXT = NULL                                                  \
)


#define _ccdll_init_info(_ccdll, _start, _ratio, _thrsh)                       \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_ccdll)->start = ((_start) > 0) ? (_start) : 1,                           \
    (_ccdll)->ratio = ((_ratio) > 0) ? (_ratio) : 1,                           \
    (_ccdll)->thrsh = ((_thrsh) > (_ccdll)->start) ? (_thrsh) : (_ccdll)->start\
)


#define ccdll_iter_init(_iter, _ccdll)                                         \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = NULL,                                                 \
    (_iter)->ccdll = (_ccdll)                                                  \
)


#define _ccdll_iter_init(_iter, _ccdll)                                        \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _iter_alloc((_iter));                                                      \
                                                                               \
    ccdll_iter_init((_iter), (_ccdll));                                        \
)



/* ccdll destroy */


#define ccdll_free(_ccdll)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_free((_ccdll));                                                        \
    _ll_free((_ccdll));                                                        \
                                                                               \
    _iter_free ((_ccdll)->_iter);                                              \
    _itarr_free((_ccdll));                                                     \
    _block_free((_ccdll));                                                     \
    _cc_ll_free((_ccdll));                                                     \
)



/* ccdll access */


#define ccdll_front(_ccdll)  ((_ccdll)->head.NXT->val)

#define ccdll_back(_ccdll)   ((_ccdll)->tail.PRV->val)



/* ccdll capacity */


#define ccdll_size(_ccdll)   ((_ccdll)->size)

#define ccdll_empty(_ccdll)  ((ccdll_size((_ccdll))) == 0)



/* ccdll modifiers */


#define ccdll_push_front(_ccdll, _val) _ccdll_push(_ccdll, _val, head, NXT, PRV)

#define ccdll_push_back(_ccdll, _val)  _ccdll_push(_ccdll, _val, tail, PRV, NXT)

#define _ccdll_push(_ccdll, _val, _hdtl_, _pn_1_, _pn_2_)                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _node_alloc((_ccdll)->pnode, (_ccdll));                                    \
                                                                               \
    (_ccdll)->pnode->val = (_val);                                             \
                                                                               \
    (_ccdll)->pnode->_pn_1_ =   (_ccdll)->_hdtl_._pn_1_;                       \
    (_ccdll)->pnode->_pn_2_ = &((_ccdll)->_hdtl_);                             \
                                                                               \
    (_ccdll)->pnode->PRV->NXT = (_ccdll)->pnode;                               \
    (_ccdll)->pnode->NXT->PRV = (_ccdll)->pnode;                               \
                                                                               \
    (_ccdll)->size++;                                                          \
)


#define  ccdll_pop_front(_ccdll)  _ccdll_pop(_ccdll, head, NXT)

#define  ccdll_pop_back(_ccdll)   _ccdll_pop(_ccdll, tail, PRV)

#define _ccdll_pop(_ccdll, _hdtl_, _np_)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccdll_empty((_ccdll)))  break;                                         \
                                                                               \
    (_ccdll)->pnode = (_ccdll)->_hdtl_._np_;                                   \
                                                                               \
    (_ccdll)->pnode->PRV->NXT = (_ccdll)->pnode->NXT;                          \
    (_ccdll)->pnode->NXT->PRV = (_ccdll)->pnode->PRV;                          \
                                                                               \
    _node_clear((_ccdll)->pnode, (_ccdll));                                    \
                                                                               \
    (_ccdll)->size--;                                                          \
)


#define ccdll_insert(_iter, _val)                                              \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccdll_iter_at_head((_iter)))  break;                                   \
                                                                               \
    _node_alloc((_iter)->ccdll->pnode, (_iter)->ccdll);                        \
                                                                               \
    (_iter)->ccdll->pnode->val = (_val);                                       \
                                                                               \
    (_iter)->ccdll->pnode->PRV = (_iter)->curr.node->PRV;                      \
    (_iter)->ccdll->pnode->NXT = (_iter)->curr.node;                           \
                                                                               \
    (_iter)->curr.node->PRV->NXT = (_iter)->ccdll->pnode;                      \
    (_iter)->curr.node->PRV      = (_iter)->ccdll->pnode;                      \
    (_iter)->curr.node           = (_iter)->ccdll->pnode;                      \
                                                                               \
    (_iter)->ccdll->size++;                                                    \
)


#define ccdll_erase(_iter)                                                     \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if (ccdll_iter_at_head((_iter)) || ccdll_iter_at_tail((_iter)))  break;    \
                                                                               \
    (_iter)->curr.node->PRV->NXT = (_iter)->curr.node->NXT;                    \
    (_iter)->curr.node->NXT->PRV = (_iter)->curr.node->PRV;                    \
                                                                               \
    _node_clear((_iter)->curr.node, (_iter)->ccdll);                           \
                                                                               \
    (_iter)->curr.node = (_iter)->curr.node->NXT;                              \
                                                                               \
    (_iter)->ccdll->size--;                                                    \
)


#define ccdll_clear(_ccdll)                                                    \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    while (!(ccdll_empty((_ccdll))))  ccdll_pop_back((_ccdll));                \
)



/* ccdll operations */


#define ccdll_move_range(_iter_p, _iter_l, _iter_r)                            \
                                                                               \
        ccdll_move_range_extd(_iter_p, _iter_l, _iter_r,    -1)

#define ccdll_move_range_extd(_iter_p, _iter_l, _iter_r, _dist)                \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_iter_l)->ccdll != (_iter_r)->ccdll)  break;                          \
                                                                               \
    if ((_iter_p)->ccdll != (_iter_l)->ccdll)   {  /* NOT IMPLEMENTED */  }    \
                                                                               \
    if ((_iter_l)->curr.node == (_iter_r)->curr.node)  break;                  \
                                                                               \
    void *_bup = (_iter_r)->curr.node;                                         \
                                                                               \
    (_iter_r)->curr.node = (_iter_r)->curr.node->PRV;                          \
                                                                               \
    (_iter_l)->curr.node->PRV->NXT = (_iter_r)->curr.node->NXT;                \
    (_iter_r)->curr.node->NXT->PRV = (_iter_l)->curr.node->PRV;                \
                                                                               \
    (_iter_l)->curr.node->PRV      = (_iter_p)->curr.node->PRV;                \
    (_iter_p)->curr.node->PRV->NXT = (_iter_l)->curr.node;                     \
                                                                               \
    (_iter_r)->curr.node->NXT      = (_iter_p)->curr.node;                     \
    (_iter_p)->curr.node->PRV      = (_iter_r)->curr.node;                     \
                                                                               \
    (_iter_r)->curr.node = _bup;                                               \
)



/* ccdll iterators */


#define ccdll_iter_copy(_iter_dst, _iter_src)                                  \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    *(_iter_dst) = *(_iter_src)                                                \
)


#define ccdll_iter_head(_iter)                                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = &((_iter)->ccdll->head)                               \
)


#define ccdll_iter_tail(_iter)                                                 \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = &((_iter)->ccdll->tail)                               \
)


#define ccdll_iter_begin(_iter)                                                \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = ((_iter)->ccdll->head.NXT)                            \
)


#define ccdll_iter_end(_iter)                                                  \
                                                                               \
VOID_EXPR_                                                                     \
(                                                                              \
    (_iter)->curr.node = ((_iter)->ccdll->tail.PRV)                            \
)


#define ccdll_iter_at_head(_iter)   ((_iter)->curr.node->PRV == NULL)

#define ccdll_iter_at_tail(_iter)   ((_iter)->curr.node->NXT == NULL)

#define ccdll_iter_at_begin(_iter)  ( (_iter)->curr.node->PRV ==               \
                                    &((_iter)->ccdll->head))

#define ccdll_iter_at_end(_iter)    ( (_iter)->curr.node->NXT ==               \
                                    &((_iter)->ccdll->tail))


#define ccdll_iter_incr(_iter)                                                 \
(                                                                              \
    (ccdll_iter_at_tail(_iter)) ? (NULL) :                                     \
    ((_iter)->curr.node = (_iter)->curr.node->NXT)->NXT                        \
)


#define ccdll_iter_decr(_iter)                                                 \
(                                                                              \
    (ccdll_iter_at_head(_iter)) ? (NULL) :                                     \
    ((_iter)->curr.node = (_iter)->curr.node->PRV)->PRV                        \
)


#define ccdll_iter_advance(_iter, _diff)                                       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _len = (_diff);                                                        \
                                                                               \
    if (_len > 0)       {  while (ccdll_iter_incr((_iter)) && --_len);  }      \
    else if (_len < 0)  {  while (ccdll_iter_decr((_iter)) && ++_len);  }      \
)



/* ccdll traversor */


#define CCDLL_INCR(_iter)                                                      \
                                                                               \
    for (ccdll_iter_head((_iter)); ccdll_iter_incr((_iter)); )

#ifndef CCC_STRICT

#define CCDLL_INCR_AUTO(_pval, _ccdll)                                         \
                                                                               \
    ccdll_iter_head((_ccdll)->_iter);                                          \
                                                                               \
    for (__typeof__((_ccdll)->pnode->val) *_pval;                              \
         (ccdll_iter_incr((_ccdll)->_iter)) &&                                 \
         ((_pval) = &DREF((_ccdll)->_iter), 1); )

#endif // CCC_STRICT



#endif
