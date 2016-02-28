# C Container Collection

## Synopsis

A collection of **high-performance** containers implemented by standard C macros.

## Description

C Container Collection is a set of function-like macros that strictly conform to standard C (ISO/IEC 9899:1999). Since all of the macros are expanded at compile time, it's much faster than C++ because of no function call overheads. This project puts emphasis on very low memory usage and very low resource consumption, and therefore the dllst container is implemented by XOR linked list and has its own memory pool in order to avoid frequent memory allocation. This is not a replacement for C++ Standard Template Library. Instead, it provides a fully new method to create containers in C.

## List of Macros

* <a href="http://people.cs.nctu.edu.tw/~dongnj/C-Container-Collection/doc/macros-list.html" target="_blank">dllst container</a> - a doubly linked list container (under development)

* dcarr container - a dynamic cyclic array container

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>>

## License

This project is distributed under GPLv2 or later.
