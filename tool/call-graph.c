#define _XOPEN_SOURCE

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define  PATH_CCXLL "../src/ccxll.h"
#include PATH_CCXLL

int is_name(char);
void shrink(char*);

int main(void)
{
    FILE *fp = popen("gcc -fpreprocessed -P -w -dD -E ../src/ccxll.h", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    struct stat st;
    stat(PATH_CCXLL, &st);
    size_t sz_ccxll = (size_t)st.st_size;

    char *token = malloc(sz_ccxll * 2);
    if (token == NULL)
        exit(EXIT_SUCCESS);


    ccxll(char*) ls_token;
    ccxll_init(ls_token);


    int curr = 0, prev = -1, len = 0;
    while ((curr = fgetc(fp)) != EOF)
    {
        if ((!!is_name((char)curr)) ^ (!!is_name((char)prev)))
        {
            token[len++] = '\0';
            ccxll_push_back(ls_token, &token[len]);
        }
        token[len++] = (char)(prev = curr);
    }
    token[len] = '\0';

    CCXLL_INCR(ITER(ls_token))
    {
        shrink(DREF(ITER(ls_token)));

        if (!*(DREF(ITER(ls_token))))
            ccxll_erase(ITER(ls_token));
    }


    int first = 1;
    ccxll_extd(char*, 2, NORMAL) ls_macro;
    ccxll_init(ls_macro);


    CCXLL_INCR(ITER(ls_token))
        if (!(ccxll_iter_at_begin(ITER(ls_token))) &&
            strcmp(XREF_PREV(ITER(ls_token)), "#define") == 0 &&
            (!( '_' == (DREF(ITER(ls_token))[0]) &&
                isupper(DREF(ITER(ls_token))[1]))))
            ccxll_push_back(ls_macro, DREF(ITER(ls_token)));

    ccxll_iter_begin(ITER(ls_macro));

    printf("digraph\n{\n");

    printf("    rankdir=LR\n");
    printf("    node [ shape=\"rect\" style=\"rounded,filled\" ");
    printf("fillcolor=\"none\" fontname=\"noto\" ]\n\n");

    printf("    { rank=\"sink\" XOR2 XOR3 }\n\n");

    CCXLL_INCR(ITER(ls_token))
    {
        if (DREF(ITER(ls_token)) == DREF(ITER(ls_macro)))
        {
            if (!first)
                printf(" }\n");
            first = 0;

            printf("    { %s%s%s } -> {", DREF(ITER(ls_token)),
                   (DREF(ITER(ls_token))[0] == '_') ?
                   " [ color=gray50 fontcolor=gray50 ]" : "",
                   (isupper(DREF(ITER(ls_token))[0])) ?
                   " [ style=\"\" ]" : "");

            ccxll_iter_incr(ITER(ls_macro));
            continue;
        }

        CCXLL_INCR(ITER_NTH(ls_macro, 1))
        {
            if (!(ccxll_iter_at_end(ITER(ls_token))) &&
                strcmp(DREF(ITER    (ls_token)),
                       DREF(ITER_NTH(ls_macro, 1))) == 0 &&
                XREF_NEXT(ITER(ls_token))[0] == '(')
                printf(" %s", DREF(ITER(ls_token)));
        }
    }

    printf(" }\n}");

    ccxll_free(ls_token);
    ccxll_free(ls_macro);

    pclose(fp);

    return 0;
}

int is_name(char ch)
{
    return isalpha(ch) || isdigit(ch) || ch == '#' || ch == '_';
}

void shrink(char *str)
{
    char *paste = str--;

    while (*++str)
        if (!isspace(*str))
            *paste++ = *str;

    *paste = '\0';
}
