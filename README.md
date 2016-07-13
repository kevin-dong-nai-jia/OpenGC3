# C Container Collection

## Synopsis

A collection of **high-performance** containers implemented by standard C macros.

## Description

C Container Collection is a large set of function-like macros that strictly conform to the C standard (C99, ISO/IEC 9899:1999). It runs faster than C++ STL since all macros are expanded at compile time. This project puts emphasis on low memory usage and low time consumption, and therefore the ccxll container is implemented by XOR doubly linked list with its memory pool to avoid frequent memory (de)allocation. This does not aim at being a replacement for STL. Instead, it provides a new way to create high-performance containers with iterators in C.

## List of Macros

* [`ccxll`](http://people.cs.nctu.edu.tw/~dongnj/C-Container-Collection/doc/macros-list.html) - xor linked list container

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>>

## License

This project is distributed under the MIT license.
