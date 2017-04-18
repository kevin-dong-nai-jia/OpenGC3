#!/bin/bash

cd "$(dirname "$0")"

PREPROC="$(gcc -fpreprocessed -w -dD -E    \
           '../src/ccxll.h' |              \
           sed ':a;N;$!ba;s/\\\n//g' |     \
           grep ^#define |                 \
           awk '{$1=""; print "#" $0}' |   \
           sed 's/[^#a-zA-Z0-9_]/ /g' |    \
           tr -s ' ')"

PATTERN="$(echo "$PREPROC" |               \
           awk '{print $2}' |              \
           paste -sd '|' - |               \
           sed 's/|/\\|/g')"

BLACKLS='OPENGC3_CCXLL_H\|XOR\|'`
       `'ccxll\|CCXLL\|'`
       `'CCXLL_CONT\|CCXLL_NODE\|'`
       `'CCXLL_BLCK\|CCXLL_ITER\|'`
       `'CCXLL_HDTL\|CCXLL_ADJC'

MATCHED="$(echo -n 'ccxll ' &&             \
           echo "$PREPROC" |               \
           tr ' ' '\n' |                   \
           grep -w "#\|$PATTERN" |         \
           grep -v -w "$BLACKLS" |         \
           tr '\n' ' ' |                   \
           sed 's/# /\n/g' |               \
           sed '/^$/d' && echo '')"

BLACKLS="ccxll_append"

DOTFRMT='{printf "    { " $1 " } -> "; '`
       `'$1="{"; print $0 " }" }'

DIGRAPH="$(echo "$MATCHED" |               \
           grep -v -w "$BLACKLS" |         \
           awk  "$DOTFRMT")"

DOTFILE="$(cat 'ccxll-call.tem.dot' &&     \
           echo -e "$DIGRAPH" '\n}')"

SVGFILE="$(dot -T svg <<< "$DOTFILE")"

SVGCOMB="$(head -n -1 <<< "$SVGFILE" &&    \
           cat 'ccxll-call.tem.svg')"

SVG2PDF="$(rsvg-convert -f pdf             \
           <<< "$SVGCOMB" > 'ccxll-call')"

PDFCROP="$(pdfcrop --margins '32 32 32 32' \
           'ccxll-call' 'ccxll-call.pdf')"

rm 'ccxll-call'
