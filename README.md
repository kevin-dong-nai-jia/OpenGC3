# OpenGC<sup>3</sup><sub><sup>/open-'gik/</sup></sub></br><i><sup><sub><sup>Open General C Container Collection</sup></sub></sup></i>

## Containers

|  Type                            |  Description                          |
|----------------------------------|:-------------------------------------:|
|  `ccarr(T)`                      |  Array of Bits                        |
| [`ccxll(T)`](doc/ccxll-call.pdf) | [XOR Linked List](doc/ccxll-list.pdf) |
|  `ccsll(T)`                      |  Singly Linked List                   |
|  `ccdll(T)`                      |  Doubly Linked List                   |

## Example

```c
#include "src/list/extd-ccdll.h"

int main(void)
{
    // Create
    ccdll(int) list;

    // Initialize
    ccdll_init(list);

    // Modify
    for (int cnt = 0; cnt < 8; cnt++)
        ccdll_push_back(list, rand());

    // Operate
    ccdll_sort_prefetch(list);

    // Traverse
    CCDLL_INCR_AUTO(pnum, list)
        printf("num = %d\n", *pnum);

    // Destroy
    ccdll_free(list);
}
```

See [test cases](test) for more fascinating examples!

## Contributors

[Kevin Dong](mailto:kevin.dong.nai.jia@gmail.com) - 2015 2016 2017 2018

[Andylinpersonal](mailto:andylinpersonal@gmail.com) - 2017

## Distribution

This project is distributed under [the MIT License](LICENSE).



