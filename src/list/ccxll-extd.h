#ifndef OPENGC3_LIST_CCXLL_EXTD_H
#define OPENGC3_LIST_CCXLL_EXTD_H

#include "base-extd.h"
#include "ccxll.h"
#include "../../src/vect/array.h"


/* ccxll integrity */


#define ccxll_is_sorted(_ccxll, _ptrue)                                        \
                                                                               \
        ccxll_is_sorted_extd(_ccxll, XLEQ, _ptrue)

#define ccxll_is_sorted_extd(_ccxll, _leq, _ptrue)                             \
                                                                               \
        cc_ll_is_sorted_extd(_ccxll, XLEQ, _ptrue, ccxll)


#define ccxll_is_robust(_ccxll, _ptrue)                                        \
                                                                               \
        cc_ll_is_robust(_ccxll, _ptrue, ccxll, CCXLL)



/* ccxll sort extended */


#define ccxll_sort_destruct(_ccxll)                                            \
                                                                               \
        ccxll_sort_destruct_extd(_ccxll, XLEQ, ACMP)

#define ccxll_sort_destruct_extd(_ccxll, _leq, _cmp)                           \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _sbup = ccxll_size((_ccxll));                                          \
                                                                               \
    (_ccxll)->pblock = (_ccxll)->pool;                                         \
                                                                               \
    while ((_ccxll)->pblock != NULL)                                           \
    {                                                                          \
        int _lo = ((_ccxll)->pblock->bnxt == NULL) ? (_ccxll)->vcnt : 0;       \
        int _hi = ((_ccxll)->pblock->ncnt - 1);                                \
                                                                               \
        array_sort((_ccxll)->pblock->nodes, &(_ccxll)->swap, _lo, _hi, _cmp);  \
                                                                               \
        (_ccxll)->pool   = (_ccxll)->pblock;                                   \
        (_ccxll)->pblock = (_ccxll)->pblock->bprv;                             \
    }                                                                          \
                                                                               \
    _ccxll_init_seed((_ccxll));                                                \
                                                                               \
    while (_sbup-- > 0)                                                        \
        ccxll_push_front_alloc((_ccxll));                                      \
                                                                               \
    ccxll_block_merge_extd((_ccxll), _leq);                                    \
)


#define ccxll_block_merge_extd(_ccxll, _leq)                                   \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    _it_init((_ccxll), 3, _base_s1, ccxll);                                    \
                                                                               \
    _ccxll_block_merge_extd((_ccxll), _it_((_ccxll), _base_s1, 0),             \
                                      _it_((_ccxll), _base_s1, 1),             \
                                      _it_((_ccxll), _base_s1, 2), _leq);      \
                                                                               \
    _it_clear((_ccxll), 3);                                                    \
)


#define _ccxll_block_merge_extd(_ccxll, _iter_l, _iter_m, _iter_r, _leq)       \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_ccxll)->pool == NULL)  break;                                        \
                                                                               \
    (_ccxll)->pblock = (_ccxll)->pool;                                         \
                                                                               \
    ccxll_iter_begin((_iter_r));                                               \
                                                                               \
    while ((_ccxll)->pblock != NULL)                                           \
    {                                                                          \
        ccxll_iter_begin((_iter_l));                                           \
        ccxll_iter_copy ((_iter_m), (_iter_r));                                \
                                                                               \
       _ccxll_iter_block((_ccxll)->pblock->bprv, (_iter_r));                   \
        ccxll_merge_range_extd((_iter_l), (_iter_m), (_iter_r), _leq);         \
                                                                               \
        (_ccxll)->pblock = (_ccxll)->pblock->bprv;                             \
    }                                                                          \
)


#define _ccxll_iter_block(_pblock, _iter)                                      \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    if ((_pblock) == NULL)                                                     \
        ccxll_iter_tail((_iter));                                              \
    else if ((_pblock)->bnxt == NULL)                                          \
        ccxll_iter_begin((_iter));                                             \
    else                                                                       \
    {                                                                          \
        (_iter)->curr.XOR = &(_pblock)->nodes[0];                              \
        (_iter)->prev.XOR = &(_pblock)->bnxt->nodes[(_pblock)->bnxt->ncnt - 1];\
        (_iter)->next.XOR = XOR2((_iter)->curr.node->XOR, (_iter)->prev.XOR);  \
    }                                                                          \
)



#endif
