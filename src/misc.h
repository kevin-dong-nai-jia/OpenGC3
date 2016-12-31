#ifndef CC_MISC_H
#define CC_MISC_H


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


/* misc  */

#define ELEMOF(ARRAY)    ((sizeof(ARRAY) / sizeof(ARRAY[0])))


#endif
