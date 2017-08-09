#ifndef OPENGC3_BASE_MISC_H
#define OPENGC3_BASE_MISC_H


/* syntax wrapper */

#define STATEMENT_(...)   do {__VA_ARGS__} while (0)
#define VOID_EXPR_(...)   ((__VA_ARGS__), ((void)0))


/* general macros */

#define MIN_2(A, B)  ((A) < (B) ? (A) : (B))
#define BITSOF(VAR)  (sizeof(VAR) * CHAR_BIT)
#define ELEMOF(ARR)  (sizeof(ARR) / sizeof(ARR[0]))
#define UMAXOF(VAR)  (~UINT64_C(0) >> (64 - BITSOF(VAR)))


/* compiler pragmas */

#define PRAGMA_NORMAL_BGN
#define PRAGMA_NORMAL_END

#define PRAGMA_PACKED_BGN  _Pragma("pack(push, 1)")
#define PRAGMA_PACKED_END  _Pragma("pack(pop)"    )


/* compiler extensions */

#ifndef CC_STRICT
    #define _unlikely(_expr)  (__builtin_expect(!!(_expr), 0))
    #define _prefetch(_addr)  (__builtin_prefetch((_addr)))
    #define _it_(_cont, _iter, _offset)  (&(_iter)[(_offset)])
    #define _co_(_cont, _base, _offset)  ((_cont)->_co[(_base) + (_offset)])
#else
    #define _unlikely(_expr)  (_expr)
    #define _prefetch(_addr)  (_addr)
    #define _it_(_cont, _base, _offset)  ((_cont)->_it[(_base) + (_offset)])
    #define _co_(_cont, _base, _offset)  ((_cont)->_co[(_base) + (_offset)])
#endif // CC_STRICT


/* pointer layout */

#define XOR  lnk[0]
#define NXT  lnk[0]
#define PRV  lnk[1]


/* append line ID */

#define ADDID APPENDLINE
#define CONCATLINE(N, L)  N ## _ ## L
#define EXPANDLINE(N, L)  CONCATLINE(N, L)
#define APPENDLINE(NAME)  EXPANDLINE(NAME, __LINE__)

#define CCDLL       ADDID(CCDLL)
#define CCDLL_CONT  ADDID(CCDLL_CONT)
#define CCDLL_NODE  ADDID(CCDLL_NODE)
#define CCDLL_BLCK  ADDID(CCDLL_BLCK)
#define CCDLL_ITER  ADDID(CCDLL_ITER)
#define CCDLL_PTRS  ADDID(CCDLL_PTRS)

#define CCSLL       ADDID(CCSLL)
#define CCSLL_CONT  ADDID(CCSLL_CONT)
#define CCSLL_NODE  ADDID(CCSLL_NODE)
#define CCSLL_BLCK  ADDID(CCSLL_BLCK)
#define CCSLL_ITER  ADDID(CCSLL_ITER)
#define CCSLL_PTRS  ADDID(CCSLL_PTRS)

#define CCXLL       ADDID(CCXLL)
#define CCXLL_CONT  ADDID(CCXLL_CONT)
#define CCXLL_NODE  ADDID(CCXLL_NODE)
#define CCXLL_BLCK  ADDID(CCXLL_BLCK)
#define CCXLL_ITER  ADDID(CCXLL_ITER)
#define CCXLL_HDTL  ADDID(CCXLL_HDTL)
#define CCXLL_PTRS  ADDID(CCXLL_PTRS)


#endif
