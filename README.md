# C Container Collection

## Synopsis

A collection of **high-performance** containers implemented by C macros.

## Example

```C
cc_dllst(msg, char*);
cc_dllst_iter(msg_iter, msg);

cc_dllst_push_back(msg, "Hello everyone:\n");
cc_dllst_push_back(msg, "Welcome to C Container Collection Project!\n");

cc_dllst_trav(msg_iter, msg)
    printf("%s", ***msg_iter);

cc_dllst_dealloc(msg);
```

## Macros List

* [dllst container macros list](http://cdn.rawgit.com/kevin-dong-nai-jia/C-Container-Collection/master/doc/macros%20list.html)

## Motivation

Though C++ has existed for many years, there’s still no **efficient** and **instinctive** ways to create containers in C99 standard.

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>>

## License

This project is distributed under GPLv2 or later.
