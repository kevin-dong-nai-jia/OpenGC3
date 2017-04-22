#!/bin/bash

cd "$(dirname "$0")"


# USER-DEFINED ARGUMENT

HDRFILE='../src/ccxll.h'
PDFFILE='ccxll-call.pdf'

TEM_DOT='ccxll-call.tem.dot'
TEM_SVG='ccxll-call.tem.svg'

RM_WORD='OPENGC3_CCXLL_H\|XOR\|ccxll\|CCXLL\|'`
       `'CCXLL_CONT\|CCXLL_NODE\|CCXLL_BLCK\|'`
       `'CCXLL_ITER\|CCXLL_HDTL\|CCXLL_ADJC\|'`
       `'ccxll_empty\|ccxll_size\|ccxll_append\|'`
       `'ccxll_front\|ccxll_back\|ccxll_comp_leq'


# CALL-GRAPH GENERATION

CURTIME="$(LANG=en_US date '+%b %Y')"

RM_LINE="$(echo "$RM_WORD" |                \
           sed 's/\\|/\n/g' |               \
           sed 's/^/# /' | paste -sd '|' |  \
           sed 's/|/\\|/g')"

PREPROC="$(gcc -fpreprocessed               \
            -w -dD -E "$HDRFILE" |          \
           sed ':a;N;$!ba;s/\\\n//g' |      \
           grep ^#define |                  \
           awk '{$1="" ; print "#" $0}' |   \
           sed 's/[^#a-zA-Z0-9_]\|##/ /g' | \
           grep -v -w "$RM_LINE")"

KEYWORD="$(echo "$PREPROC" |                \
           awk '{print $2}' && echo '#')"

echo "$KEYWORD" > 'KEYWORD'

MATCHED="$(echo "$PREPROC" |                \
           tr -s ' ' | tr ' ' '\n' |        \
           LC_ALL=C grep -w -f 'KEYWORD' |  \
           tr '\n' ' ' | sed 's/# /\n/g' |  \
           sed '/^$/d' && echo '')"

DOTFRMT='{printf $1" -> ";$1="{";print $0" }"}'

PREPEND=$'ccxll ccxll_extd\n'

DIGRAPH="$(echo "$PREPEND" "$MATCHED" | awk "$DOTFRMT")"

DOTFILE="$(m4 "$TEM_DOT" && echo -e "$DIGRAPH" '\n } ')"

SVGFILE="$(dot -Tsvg <<< "$DOTFILE" | head -n -1 &&   \
           cat "$TEM_SVG" | sed "s/\[CURTIME\]/$CURTIME/g")"

SVG2PDF="$(rsvg-convert -f 'pdf' <<< "$SVGFILE" >'PDF_RAW')"

PDFCROP="$(pdfcrop --margins '64' 'PDF_RAW' "$PDFFILE")"

rm 'KEYWORD' 'PDF_RAW'
