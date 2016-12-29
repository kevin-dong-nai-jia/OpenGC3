#ifndef CC_SNYM_H
#define CC_SNYM_H


/* Internal Use */

#define ADDID APPENDLINE
#define CONCATLINE(N, L)  N ## L
#define EXPANDLINE(N, L)  CONCATLINE(N, L)
#define APPENDLINE(NAME)  EXPANDLINE(NAME, __LINE__)

#define CC_T_CCXLL        ADDID(CC_T_CCXLL_      )
#define CC_S_CCXLL_BODY   ADDID(CC_S_CCXLL_BODY_ )
#define CC_S_CCXLL_NODE   ADDID(CC_S_CCXLL_NODE_ )
#define CC_S_CCXLL_BLOCK  ADDID(CC_S_CCXLL_BLOCK_)
#define CC_S_CCXLL_ITER   ADDID(CC_S_CCXLL_ITER_ )
#define CC_U_CCXLL_HDTL   ADDID(CC_U_CCXLL_HDTL_ )
#define CC_U_CCXLL_ITER   ADDID(CC_U_CCXLL_ITER_ )

#ifdef CCC_STRICT
#define _it_(_ccxll, _base, _offset)  ((_ccxll)->_it[(_base) + (_offset)])
#else
#define _it_(_ccxll, _iter, _offset)  (&(_iter)[(_offset)])
#endif

#define _xl_(_ccxll, _base, _offset)  ((_ccxll)->_xl[(_base) + (_offset)])


/* Abbreviation */

#define DREF             ccxll_iter_dref
#define DREF_PREV        ccxll_iter_dref_prev
#define DREF_NEXT        ccxll_iter_dref_next

#define XLEQ             CCXLL_LEQ_COMPAR

#define ITER_NTH         ccxll_iter
#define ITER(_iter)      ccxll_iter(_iter, 0)


#endif
