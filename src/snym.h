#ifndef _CC_SNYM_H_
#define _CC_SNYM_H_


/* Internal */

#define _T_CCXLL        ADDID(_T_CCXLL_      )
#define _S_CCXLL_BODY   ADDID(_S_CCXLL_BODY_ )
#define _S_CCXLL_NODE   ADDID(_S_CCXLL_NODE_ )
#define _S_CCXLL_BLOCK  ADDID(_S_CCXLL_BLOCK_)
#define _S_CCXLL_ITER   ADDID(_S_CCXLL_ITER_ )
#define _U_CCXLL_HDTL   ADDID(_U_CCXLL_HDTL_ )
#define _U_CCXLL_ITER   ADDID(_U_CCXLL_ITER_ )


/* External */

#define DREF            ccxll_iter_dref
#define DREF_PREV       ccxll_iter_dref_prev
#define DREF_NEXT       ccxll_iter_dref_next
#define ITER            ccxll_iter

#define XLEQ            CCXLL_LEQ_COMPAR


#endif
