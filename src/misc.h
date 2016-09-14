#ifndef _CC_MISC_H_
#define _CC_MISC_H_


/* generic pointer */

typedef void* link_t;


/* compiler pragma */

#define PRAGMA_NORMAL_BGN
#define PRAGMA_NORMAL_END

#define PRAGMA_PACKED_BGN  _Pragma("pack(push, 1)")
#define PRAGMA_PACKED_END  _Pragma("pack(pop)"    )


/* syntax wrappers */

#define STATEMENT_(...)  do {__VA_ARGS__} while (0)
#define VOID_EXPR_(...)  ((__VA_ARGS__), ((void)0))


/* append __LINE__ */

#define ADDID APPENDLINE
#define CONCATLINE(N, L) N ## L
#define EXPANDLINE(N, L) CONCATLINE(N, L)
#define APPENDLINE(NAME) EXPANDLINE(NAME, __LINE__)


/* number of elem. */

#define ELEMOF_ARR(PARR) ((int)(sizeof(PARR) / sizeof(PARR[0])))


#endif
