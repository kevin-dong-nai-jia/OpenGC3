# C Container Collection

## Synopsis

A collection of generic type-safe containers implemented by standard C macros.

## Description

C Container Collection is a set of macros and it runs faster than most of the C++ STL implementations since all macros are expanded at compile time. This project puts much emphasis on very low memory usage and high performance, and therefore the ccxll container is implemented by XOR doubly linked list with its own memory pool to avoid frequent memory allocation. Besides, each macro is strictly conformed to the C standard and fully tested under Valgrind for memory leak detection and other static code analysis tools for an out of bound check.

## Container

CCXLL : XOR Linked List \[ [Source](https://github.com/kevin-dong-nai-jia/C-Container-Collection/blob/master/src/ccxll.h) / [Graph](https://github.com/kevin-dong-nai-jia/C-Container-Collection/blob/master/tool/call-graph.pdf) / [Usage](https://github.com/kevin-dong-nai-jia/C-Container-Collection/blob/master/doc/macros-list.pdf) (Deprecated) \]

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>> - 2015 / 2016

## License

This project is distributed under [the MIT License](https://github.com/kevin-dong-nai-jia/C-Container-Collection/blob/master/LICENSE).
