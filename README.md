# OpenGC<sup>3</sup><sub><sup>/open-'gik/</sup></sub></br><i><sup><sub><sup>Open General C Container Collections</sup></sub></sup></i>

## Containers

|  Type                            |  Description                          |
|----------------------------------|:-------------------------------------:|
|  `ccarr(T)`                      |  Array of Bits                        |
| [`ccxll(T)`](doc/ccxll-call.pdf) | [XOR Linked List](doc/ccxll-list.pdf) |
|  `ccsll(T)`                      |  Singly Linked List                   |
|  `ccdll(T)`                      |  Doubly Linked List                   |
|  `ccgbt(T)`                      |  General Binary Tree                  |

## Example

```c
#include "ccxll.h"

// Create
ccxll(int) list;

// Initialize
ccxll_init(list);

// Modify
for (int cnt = 8; cnt --> 0; )
    ccxll_push_back(list, rand());

// Operate
ccxll_sort(list);

// Traverse
CCXLL_INCR_AUTO(pnum, list)
    printf("num = %d\n", *pnum);

// Destroy
ccxll_free(list);
```

See [test cases](test) for more fascinating examples!

## Contributor

[Kevin Dong (Kʌ̄D)](mailto:kevin.dong.nai.jia@gmail.com) - 2015 ~ 2017

## Distribution

This project is distributed under [the MIT License](LICENSE).



