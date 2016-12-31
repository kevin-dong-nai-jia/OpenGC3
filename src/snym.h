#ifndef CC_SNYM_H
#define CC_SNYM_H


/* Internal Use */

#define CC_T_CCXLL        ADDID(CC_T_CCXLL_      )
#define CC_S_CCXLL_BODY   ADDID(CC_S_CCXLL_BODY_ )
#define CC_S_CCXLL_NODE   ADDID(CC_S_CCXLL_NODE_ )
#define CC_S_CCXLL_BLOCK  ADDID(CC_S_CCXLL_BLOCK_)
#define CC_S_CCXLL_ITER   ADDID(CC_S_CCXLL_ITER_ )
#define CC_U_CCXLL_HDTL   ADDID(CC_U_CCXLL_HDTL_ )
#define CC_U_CCXLL_ITER   ADDID(CC_U_CCXLL_ITER_ )

#define ADDID APPENDLINE
#define CONCATLINE(N, L)  N ## L
#define EXPANDLINE(N, L)  CONCATLINE(N, L)
#define APPENDLINE(NAME)  EXPANDLINE(NAME, __LINE__)

#ifndef CCC_STRICT
#define _it_(_cc_ll, _iter, _offset)  (&(_iter)[(_offset)])
#else
#define _it_(_cc_ll, _base, _offset)  ((_cc_ll)->_it[(_base) + (_offset)])
#endif

#define _xl_(_cc_ll, _base, _offset)  ((_cc_ll)->_xl[(_base) + (_offset)])


/* Abbreviation */

#define XLEQ  CCXLL_LEQ_COMPAR

#define DREF(_iter)       ((_iter)->curr.node->val)
#define DREF_PREV(_iter)  ((_iter)->prev.node->val)
#define DREF_NEXT(_iter)  ((_iter)->next.node->val)

#define ITER(_cc_ll)               (ITER_NTH(_cc_ll, 0))
#define ITER_NTH(_cc_ll, _nth_it)  (&(*(_cc_ll)->itarr)[(_nth_it)])


#endif
