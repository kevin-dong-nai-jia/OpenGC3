# C Container Collection

## Synopsis

A collection of **high-performance** containers implemented by standard C macros.

## Description

C Container Collection (CCC) is designed based on standard C macros and is fully inspired by the C++ standard containers. All of the macros are expanded at compile time, so it's much faster than C++ because there is few function call overhead. Moreover, CCC puts emphasis on low memory usage and low resource consuming, so the dllst container is implemented by XOR linked list and uses available space list to avoid frequent memory allocation. The usage is simple and similar to C++ to make it easier to use whether you are familiar with C++ or not. CCC aims at providing almost all operations on the container that C++ also does, so the naming of macros is trying to be consistent with C++.

## Macros List

* [dllst container](http://people.cs.nctu.edu.tw/~dongnj/C-Container-Collection/doc/macros%20list.html) - under development

* dcarr container

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>>

## License

This project is distributed under GPLv2 or later.
