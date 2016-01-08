# C Container Collection

## Synopsis

A collection of containers implemented by ANSI C macros.

## Example

```C
cc_deque(msg, char*);

cc_deque_push_back(msg, "Hello everyone:\n");
cc_deque_push_back(msg, "Welcome to C Container Collection Project!\n");

cc_deque_iter(msg_iter, msg);
cc_deque_trav(msg_iter, msg)  printf("%s", **msg_iter);

cc_deque_dealloc(msg);
```

Please see the header files under folder ccc/ for more details.

## History

2016 Jan 09 - Passing the same element type to each function is not needed.

## Motivation

Though C++ has existed for many years, there’s still no **efficient** and **instinctive** ways to create containers in ANSI C code (using macros without any extensions provided by specific compilers).

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>>

## License

This project is distributed under GPLv2 or later.
