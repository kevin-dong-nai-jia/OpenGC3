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

#ifndef CCC_STRICT
#define _unlikely(_expr)  (__builtin_expect(!!(_expr), 0))
#else
#define _unlikely(_expr)  (_expr)
#endif


/* Abbreviation */

#define DLEQ  CCDLL_LEQ_COMPAR
#define XLEQ  CCXLL_LEQ_COMPAR

#define LREF(_iter)       ((_iter)->curr.node->val)
#define DREF(_iter)       ((_iter)->curr.node->val)
#define DREF_PREV(_iter)  ((_iter)->curr.node->PRV->val)
#define DREF_NEXT(_iter)  ((_iter)->curr.node->NXT->val)
#define XREF(_iter)       ((_iter)->curr.node->val)
#define XREF_PREV(_iter)  ((_iter)->prev.node->val)
#define XREF_NEXT(_iter)  ((_iter)->next.node->val)

#define ITER(_cc_ll)               (ITER_NTH(_cc_ll, 0))
#define ITER_NTH(_cc_ll, _nth_it)  (&(*(_cc_ll)->itarr)[(_nth_it)])


#endif
