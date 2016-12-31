# C Container Collection

## Synopsis

A collection of generic type-safe containers implemented by C macros.

## Containers

CCXLL : (E)XOR Linked List \[ [Source](https://github.com/kevin-dong-nai-jia/C-Container-Collection/blob/master/src/ccxll.h) / [Graph](https://github.com/kevin-dong-nai-jia/C-Container-Collection/blob/master/tool/call-graph.pdf) \]

CCDLL : Doubly Linked List

CCBST : Binary Search Tree

## Examples

```c
//  An example of how to use ccxll container without CCC_STRICT defined

ccxll(int) list;                        //  declare a list to store int
ccxll_init(list);                       //  initialize the list created

for (int cnt = 8; cnt-- > 0; )
    ccxll_push_back(list, rand());      //  insert 8 numbers at the end

ccxll_sort(list);                       //  sort the list in asc. order

CCXLL_INCR_DREF(pnum, ITER(list))       //  traverse the list from left
    printf("%d ", *pnum);               //  deref the pointer to number

ccxll_free(list);                       //  free the list just as usual
```

See [ccxll test case](https://github.com/kevin-dong-nai-jia/C-Container-Collection/blob/master/test/ccxll-test.c) for more fascinating examples!

## Description

C Container Collection is a set of macros and it runs faster than most of the C++ STL implementations since all macros are expanded at compile time. This project puts much emphasis on very low memory usage and high performance, and therefore the ccxll container is implemented by XOR doubly linked list with its own memory pool to avoid frequent memory allocation. Besides, each macro is strictly conformed to the C standard and fully tested under Valgrind for memory leak detection and other static code analysis tools for an out of bound check.


## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>> - 2015 / 2016 / 2017

## License

This project is distributed under [the MIT License](https://github.com/kevin-dong-nai-jia/C-Container-Collection/blob/master/LICENSE).
