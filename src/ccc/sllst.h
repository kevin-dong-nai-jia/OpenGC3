#ifndef _CCC_SLLST_H_
#define _CCC_SLLST_H_



/* xllst container struct */
/* xllst = sllst or dllst */


#define _cc_xllst_node(_cc_xllst_object)                                   \
                                                                           \
struct                                                                     \
{                                                                          \
    void *xrt[2];                                                          \
    _cc_xllst_object##_element_type val;                                   \
}



/* sllst container struct */


#define _cc_sllst_init {0, NULL}


#define cc_sllst(_cc_sllst_object,                                         \
                 _cc_sllst_element_type)                                   \
                                                                           \
struct                                                                     \
{                                                                          \
    int size;                                                              \
    void *head;                                                            \
}   _cc_sllst_object = _cc_sllst_init;                                     \
                                                                           \
typedef _cc_sllst_element_type _cc_sllst_object##_element_type


#define _cc_sllst_node(_cc_sllst_object) _cc_xllst_node(_cc_sllst_object)



#endif
