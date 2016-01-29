# C Container Collection

## Synopsis

A collection of **high-performance** containers implemented by standard C macros.

## Description

C Container Collection (CCC) is designed based on standard C macros and is fully inspired by the C++ standard containers. All CCC macros are expanded at compile time, so it's much faster than C++ because there are few function call overheads. Moreover, CCC puts emphasis on low memory usage and low resource consuming, so the dllst container is implemented by XOR linked list and uses an available space list to avoid frequent memory allocation. The usage is simple and similar to C++ to make it easier to use whether you are familiar with C++ or not. CCC aims at providing almost all container operations that C++ also provides, and therefore the naming of macros is trying to be consistent with C++.

## Macros List

* [dllst container](http://people.cs.nctu.edu.tw/~dongnj/C-Container-Collection/doc/macros%20list.html) - a doubly linked list container (under development)

* dcarr container - a dynamic cyclic array container

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>>

## License

This project is distributed under GPLv2 or later.
