#ifndef OPENGC3_LIST_EXTD_CCDLL_H
#define OPENGC3_LIST_EXTD_CCDLL_H

#include "ccdll.h"
#include "extd-base.h"


/* ccdll integrity */


#define  ccdll_is_sorted(_ccdll, _ptrue)                                       \
                                                                               \
         ccdll_is_sorted_extd(_ccdll, DLEQ, _ptrue)

#define  ccdll_is_sorted_extd(_ccdll, _leq, _ptrue)                            \
                                                                               \
         cc_ll_is_sorted_extd(_ccdll, _leq, _ptrue, ccdll)


#define ccdll_is_robust(_ccdll, _ptrue)                                        \
                                                                               \
        cc_ll_is_robust(_ccdll, _ptrue, ccdll, CCDLL)



#endif
