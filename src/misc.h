#ifndef CC_MISC_H
#define CC_MISC_H


/* generic pointer */

typedef void* link_t;


/* compiler pragma */

#define PRAGMA_NORMAL_BGN
#define PRAGMA_NORMAL_END

#define PRAGMA_PACKED_BGN  _Pragma("pack(push, 1)")
#define PRAGMA_PACKED_END  _Pragma("pack(pop)"    )


/* line ID append */

#define ADDID APPENDLINE
#define CONCATLINE(N, L)  N ## L
#define EXPANDLINE(N, L)  CONCATLINE(N, L)
#define APPENDLINE(NAME)  EXPANDLINE(NAME, __LINE__)


/* syntax wrapper */

#define STATEMENT_(...)  do {__VA_ARGS__} while (0)
#define VOID_EXPR_(...)  ((__VA_ARGS__), ((void)0))


/* miscellaneous  */

#define ELEMOF(ARRAY)  ((sizeof(ARRAY) / sizeof(ARRAY[0])))


#endif
