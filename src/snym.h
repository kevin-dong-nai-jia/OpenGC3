#ifndef CC_SNYM_H
#define CC_SNYM_H


/* Dereference */

// ccdll & ccxll
#define LREF(_iter)         ((_iter)->curr.node->val)

// ccdll
#define DREF(_iter)         ((_iter)->curr.node->val)
#define DREF_PREV(_iter)    ((_iter)->curr.node->PRV->val)
#define DREF_NEXT(_iter)    ((_iter)->curr.node->NXT->val)

// ccxll
#define XREF(_iter)         ((_iter)->curr.node->val)
#define XREF_PREV(_iter)    ((_iter)->prev.node->val)
#define XREF_NEXT(_iter)    ((_iter)->next.node->val)

// ccgbt
#define GREF(_iter)         ((_iter)->curr.node->val)
#define GREF_PARENT(_iter)  ((_iter)->curr.node->PRN->val)
#define GREF_LEFT(_iter)    ((_iter)->curr.node->LFT->val)
#define GREF_RIGHT(_iter)   ((_iter)->curr.node->RGH->val)


/* Abbreviation */

// ccdll & ccxll
#define DLEQ  ccdll_comp_leq
#define XLEQ  ccxll_comp_leq

// ccdll & ccxll
#define ITER(_cont)               (ITER_NTH(_cont, 0))
#define ITER_NTH(_cont, _nth_it)  (&(*(_cont)->itarr)[(_nth_it)])

// ccarr
#define ELEM(_cont)               (ELEM_NTH(_cont, 0))
#define ELEM_NTH(_cont, _nth_el)  ((_cont).arr[(_nth_el)])


/* Abbreviation (Internal) */

#ifndef CC_STRICT
#define _it_(_cont, _iter, _offset)  (&(_iter)[(_offset)])
#define _co_(_cont, _base, _offset)  ((_cont)->_co[(_base) + (_offset)])
#else
#define _it_(_cont, _base, _offset)  ((_cont)->_it[(_base) + (_offset)])
#define _co_(_cont, _base, _offset)  ((_cont)->_co[(_base) + (_offset)])
#endif // CC_STRICT

#ifndef CC_STRICT
#define _unlikely(_expr)  (__builtin_expect(!!(_expr), 0))
#else
#define _unlikely(_expr)  (_expr)
#endif // CC_STRICT


#endif
