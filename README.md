# OpenGenCC

OpenGenCC (libogcc) is a collection consisting of considerable essential containers (and their adaptors) for programming in C. Every function is implemented as a C preprocessor function-like macro strictly conforming to the C99+ standards, and therefore it performs better under most of the circumstances, since it is expaned at compile time and aggressively optimized by compilers.


## Containers

|  Type                             |  Description                          |
|-----------------------------------|:-------------------------------------:|
| [`ccxll(T)`](tool/ccxll-call.pdf) | [XOR Linked List](doc/ccxll-list.pdf) |
|  `ccdll(T)`                       |  Doubly Linked List                   |
|  `ccgbt(T)`                       |  General Binary Tree                  |

## Example

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

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>> - 2015 ~ 2017

## License

This project is distributed under [the MIT License](LICENSE).

