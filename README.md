# C Container Collection

## Synopsis

A collection of containers implemented by ANSI C macros.

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

Please see the header files under src/ccc/ for more details.

## Motivation

Though C++ has existed for many years, there’s still no **efficient** and **instinctive** ways to create containers in ANSI C code (using macros without any extensions provided by specific compilers).

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>>

## License

This project is distributed under GPLv2 or later.
