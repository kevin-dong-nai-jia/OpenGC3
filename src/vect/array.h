#ifndef OPENGC3_VECT_ARRAY_H
#define OPENGC3_VECT_ARRAY_H

#include "../base/pool.h"
#include "../base/misc.h"


/* array operations */


#define array_sort(_array, _pswap, _lo, _hi, _cmp)                             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int  _top =   -1;                                                          \
    int *_stk = NULL;                                                          \
                                                                               \
    _safe_alloc(_stk, sizeof(int) * ((_hi) - (_lo) + 1));                      \
                                                                               \
    _stk[++_top] = (_lo);                                                      \
    _stk[++_top] = (_hi);                                                      \
                                                                               \
    while (_top >= 0)                                                          \
    {                                                                          \
        (_hi) = _stk[_top--];                                                  \
        (_lo) = _stk[_top--];                                                  \
                                                                               \
        int _pv;                                                               \
        array_sort_partition((_array), (_pswap), (_lo), (_hi), _cmp, &_pv);    \
                                                                               \
        if (_pv > (_lo))                                                       \
            _stk[++_top] = (_lo)  , _stk[++_top] = _pv;                        \
                                                                               \
        if (_pv + 1 < (_hi))                                                   \
            _stk[++_top] = _pv + 1, _stk[++_top] = (_hi);                      \
    }                                                                          \
                                                                               \
    _safe_free(_stk);                                                          \
)


#define array_sort_partition(_array, _pswap, _lo, _hi, _cmp, _ppv)             \
                                                                               \
STATEMENT_                                                                     \
(                                                                              \
    int _lf = (_lo) - 1, _rg = (_hi) + 1;                                      \
                                                                               \
    do                                                                         \
    {   while (_lf++, (_cmp((_array)[_lf], (_array)[(_lo)]) == -1));           \
        while (_rg--, (_cmp((_array)[_rg], (_array)[(_lo)]) ==  1));           \
    }                                                                          \
    while ((_lf < _rg) && ((*(_pswap)      =  (_array)[_lf],                   \
                             (_array)[_lf] =  (_array)[_rg],                   \
                             (_array)[_rg] = *(_pswap)), 1));                  \
                                                                               \
    *(_ppv) = _rg;                                                             \
)



/* array comparator */


#define ACMP array_comp_tri

#define array_comp_tri(_node_a, _node_b)                                       \
        (((_node_a).val > (_node_b).val) ?  1 :                                \
        (((_node_a).val < (_node_b).val) ? -1 : 0))



#endif
