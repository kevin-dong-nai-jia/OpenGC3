# C Container Collection

## Synopsis

A collection of **high-performance** containers implemented by standard C macros.

## Description

C Container Collection (CCC) is a set of function-like macros that strictly conform to standard C (C99, ISO/IEC 9899:1999). It's faster than C++ STL since all macros are expanded at compile time. This project puts emphasis on low memory usage and low resource consumption, and therefore the dllst container is implemented by XOR linked list and has its own memory pool to avoid frequent memory (de)allocation. This is not (and will not be) a replacement for C++ STL. Instead, it provides a new method to create high-performance containers with iterators in C. Generally, it sacrifices code reusability for maximum performance enhancement, so the size of generated executables will finally get a bit larger if it is in really heavy use.

## List of Macros

* [`dllst container`](http://people.cs.nctu.edu.tw/~dongnj/C-Container-Collection/doc/macros-list.html) - a doubly linked list container

* `dcarr container` - a dynamic cyclic array container (planning)

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>>

## License

This project is distributed under the MIT license.
