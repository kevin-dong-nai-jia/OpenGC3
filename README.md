# C Container Collection

## Synopsis

A collection of **high-performance** containers implemented by standard C macros.

## Description

C Container Collection is a large set of function-like macros that strictly conform to the C standard (C99, ISO/IEC 9899:1999). It runs faster than C++ STL since all macros are expanded at compile time. This project puts emphasis on low memory usage and low time consumption, and therefore the dllst container is implemented by XOR doubly linked list with its memory pool to avoid frequent memory (de)allocation. This does not aim at being a replacement for C++ STL. Instead, it provides a fully new method to create high-performance containers with iterators in C.

## List of Macros

* [`dllst container`](http://people.cs.nctu.edu.tw/~dongnj/C-Container-Collection/doc/macros-list.html) - a doubly linked list container

* `dcarr container` - a dynamic cyclic array container (planning)

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>>

## License

This project is distributed under the MIT license.
