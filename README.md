# C Container Collection

## Synopsis

A collection of containers implemented by ANSI C macros.

## Example

### Deque

```C
cc_deque(char*) msg = cc_deque_init;

cc_deque_push_back(msg, char*, "Hello everyone:\n");
cc_deque_push_back(msg, char*, "Welcome to C Container Collection Project!\n");

cc_deque_iter(char*) msg_iter = cc_deque_iter_init;
cc_deque_trav(msg, msg_iter, char*)  printf("%s", **msg_iter);

cc_deque_dealloc(msg, char*);
```

Please see the header files under folder ccc/ for more details.

## Motivation

Though C++ has existed for many years, there’s still no **efficient** and **instinctive** ways to create containers in ANSI C code (using macros without any extensions provided by specific compilers).

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>>

## License

This project is distributed under GPLv2 or later.
