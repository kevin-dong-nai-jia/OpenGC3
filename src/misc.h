#ifndef OPENGC3_MISC_H
#define OPENGC3_MISC_H


/* compiler pragma */

#define PRAGMA_NORMAL_BGN
#define PRAGMA_NORMAL_END

#define PRAGMA_PACKED_BGN  _Pragma("pack(push, 1)")
#define PRAGMA_PACKED_END  _Pragma("pack(pop)"    )


/* syntax wrapper */

#define STATEMENT_(...)   do {__VA_ARGS__} while (0)
#define VOID_EXPR_(...)   ((__VA_ARGS__), ((void)0))


/* line ID append */

#define ADDID APPENDLINE
#define CONCATLINE(N, L)  N ## _ ## L
#define EXPANDLINE(N, L)  CONCATLINE(N, L)
#define APPENDLINE(NAME)  EXPANDLINE(NAME, __LINE__)


/* miscellaneous  */

#define MIN_2(A, B)  ((A) < (B) ? (A) : (B))

#define BITSOF(VAR)  (sizeof(VAR) * CHAR_BIT)
#define ELEMOF(ARR)  (sizeof(ARR) / sizeof(ARR[0]))
#define UMAXOF(VAR)  (~UINT64_C(0) >> (64 - BITSOF(VAR)))


#endif
