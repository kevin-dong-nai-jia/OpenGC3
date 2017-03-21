## Open C Macro Container

### Containers

|  Type                             |  Description                          |
|-----------------------------------|:-------------------------------------:|
| [`ccxll(T)`](tool/ccxll-call.pdf) | [XOR Linked List](doc/ccxll-list.pdf) |
|  `ccdll(T)`                       |  Doubly Linked List                   |
|  `ccgbt(T)`                       |  General Binary Tree                  |

### Example

```c
ccxll(int) list;                      //  declare a list of type int
ccxll_init(list);                     //  initialize the list record

for (int cnt = 8; cnt-- > 0; )        //
    ccxll_push_back(list, rand());    //  insert "rand()" to the end

ccxll_sort(list);                     //  sort with comparator: XLEQ

CCXLL_INCR_AUTO(pnum, list)           //  traverse the list forward:
    printf("num = %d\n", *pnum);      //  access elems through iters

ccxll_free(list);                     //  destroy the list after use
```

See [test cases](test) for more fascinating examples!

### Description

Open C Macro Container (libocmc) is a set of macros and it runs faster than most of the C++ STL implementations since all macros are expanded at compile time. This project puts much emphasis on very low memory usage and high performance, and therefore the ccxll container is implemented by XOR doubly linked list with its own memory pool to avoid frequent memory allocation. Besides, each macro is strictly conformed to the C standard and fully tested under Valgrind for memory leak detection and other static code analysis tools for an out of bound check.


### Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>> - 2015 ~ 2017

### License

This project is distributed under [the MIT License](LICENSE).

