#ifndef CC_SNYM_H
#define CC_SNYM_H


/* Internal Use */

#ifndef CCC_STRICT
#define _it_(_cc_ll, _iter, _offset)  (&(_iter)[(_offset)])
#define _ll_(_cc_ll, _base, _offset)  ((_cc_ll)->_ll[(_base) + (_offset)])
#else
#define _it_(_cc_ll, _base, _offset)  ((_cc_ll)->_it[(_base) + (_offset)])
#define _ll_(_cc_ll, _base, _offset)  ((_cc_ll)->_ll[(_base) + (_offset)])
#endif


/* Abbreviation */

#define XLEQ  CCXLL_LEQ_COMPAR

#define DREF(_iter)       ((_iter)->curr.node->val)
#define DREF_PREV(_iter)  ((_iter)->curr.node->PRV->val)
#define DREF_NEXT(_iter)  ((_iter)->curr.node->NXT->val)
#define XREF_PREV(_iter)  ((_iter)->prev.node->val)
#define XREF_NEXT(_iter)  ((_iter)->next.node->val)

#define ITER(_cc_ll)               (ITER_NTH(_cc_ll, 0))
#define ITER_NTH(_cc_ll, _nth_it)  (&(*(_cc_ll)->itarr)[(_nth_it)])


#endif
