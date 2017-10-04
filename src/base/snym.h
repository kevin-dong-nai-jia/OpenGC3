#ifndef OPENGC3_BASE_SNYM_H
#define OPENGC3_BASE_SNYM_H


/* dereference */

// ccdll
#define DREF(_iter)         ((_iter)->curr.node->val)
#define DREF_PREV(_iter)    ((_iter)->curr.node->PRV->val)
#define DREF_NEXT(_iter)    ((_iter)->curr.node->NXT->val)

// ccsll
#define SREF(_iter)         ((_iter)->curr.node->val)
#define SREF_NEXT(_iter)    ((_iter)->curr.node->NXT->val)

// ccxll
#define XREF(_iter)         ((_iter)->curr.node->val)
#define XREF_PREV(_iter)    ((_iter)->prev.node->val)
#define XREF_NEXT(_iter)    ((_iter)->next.node->val)

// cc[dsx]ll
#define LREF(_iter)         ((_iter)->curr.node->val)

// ccgbt
#define GREF(_iter)         ((_iter)->curr.node->val)
#define GREF_PARENT(_iter)  ((_iter)->curr.node->PRN->val)
#define GREF_LEFT(_iter)    ((_iter)->curr.node->LFT->val)
#define GREF_RIGHT(_iter)   ((_iter)->curr.node->RGH->val)


/* abbreviation */

// ccdll
#define DLEQ       ccdll_comp_leq
#define DLEQ_PREV  ccdll_comp_leq_prev
#define DLEQ_NEXT  ccdll_comp_leq_next
#define DGEQ       ccdll_comp_geq
#define DGEQ_PREV  ccdll_comp_geq_prev
#define DGEQ_NEXT  ccdll_comp_geq_next

// ccsll
#define SLEQ       ccsll_comp_leq
#define SLEQ_NEXT  ccsll_comp_leq_next
#define SGEQ       ccsll_comp_geq
#define SGEQ_NEXT  ccsll_comp_geq_next

// ccxll
#define XLEQ       ccxll_comp_leq
#define XLEQ_PREV  ccxll_comp_leq_prev
#define XLEQ_NEXT  ccxll_comp_leq_next
#define XGEQ       ccxll_comp_geq
#define XGEQ_PREV  ccxll_comp_geq_prev
#define XGEQ_NEXT  ccxll_comp_geq_next

// cc[dsx]ll
#define ITER(_cont)               (ITER_NTH(_cont, 0))
#define ITER_NTH(_cont, _nth_it)  (&(*(_cont)->itarr)[(_nth_it)])

// ccarr
#define ELEM(_cont)               (ELEM_NTH(_cont, 0))
#define ELEM_NTH(_cont, _nth_el)  ((_cont).arr[(_nth_el)])


#endif
