#ifndef _CC_SNYM_H_
#define _CC_SNYM_H_


/* Internal Use */

#define ADDID APPENDLINE
#define CONCATLINE(N, L) N ## L
#define EXPANDLINE(N, L) CONCATLINE(N, L)
#define APPENDLINE(NAME) EXPANDLINE(NAME, __LINE__)

#define _T_CCXLL         ADDID(_T_CCXLL_      )
#define _S_CCXLL_BODY    ADDID(_S_CCXLL_BODY_ )
#define _S_CCXLL_NODE    ADDID(_S_CCXLL_NODE_ )
#define _S_CCXLL_BLOCK   ADDID(_S_CCXLL_BLOCK_)
#define _S_CCXLL_ITER    ADDID(_S_CCXLL_ITER_ )
#define _U_CCXLL_HDTL    ADDID(_U_CCXLL_HDTL_ )
#define _U_CCXLL_ITER    ADDID(_U_CCXLL_ITER_ )

#ifdef _CCC_STRICT
#define _ITER(_ccxll, _name, _offset)  ((_ccxll)->_it[(_name) + (_offset)])
#else
#define _ITER(_ccxll, _name, _offset)  (&(_name)[(_offset)])
#endif // _CCC_STRICT


/* Abbreviation */

#define DREF             ccxll_iter_dref
#define DREF_PREV        ccxll_iter_dref_prev
#define DREF_NEXT        ccxll_iter_dref_next

#define XLEQ             CCXLL_LEQ_COMPAR

#define ITER_NTH         ccxll_iter
#define ITER(_iter)      ccxll_iter(_iter, 0)


#endif
