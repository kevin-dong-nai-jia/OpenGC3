# C Container Collection

## Synopsis

A collection of **high-performance** containers implemented by standard C macros.

## Description

C Container Collection is a set of macros and runs faster than most of the C++ STL implementations since all of the macros are expanded at compile time. This project puts emphasis on very low memory usage and high performance, and therefore the ccxll container is implemented by XOR doubly linked list with its own memory pool to avoid frequent memory allocation. Besides, each macro is strictly conformed to the C standard and fully tested under Valgrind.

## List of Macros

* [`ccxll`](http://people.cs.nctu.edu.tw/~dongnj/C-Container-Collection/doc/macros-list.html) - xor linked list container

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>>

## License

This project is distributed under the MIT license.
