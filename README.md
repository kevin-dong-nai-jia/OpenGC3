# OpenGC<sup><sup>3</sup></sup><sub><sup> /open-'gik/</sup></sub></br><i><sup><sub><sup>Open General C Container Collections</sup></sub></sup></i>

## Containers

|  Type                             |  Description                          |
|-----------------------------------|:-------------------------------------:|
|  `ccarr(T)`                       |  Array of Bits                        |
|  `ccdll(T)`                       |  Doubly Linked List                   |
|  `ccgbt(T)`                       |  General Binary Tree                  |
| [`ccxll(T)`](tool/ccxll-call.pdf) | [XOR Linked List](doc/ccxll-list.pdf) |

## How to Use

```c
ccxll(int) list;
ccxll_init(list);

for (int cnt = 8; cnt --> 0; )
    ccxll_push_back(list, rand());

ccxll_sort(list);

CCXLL_INCR_AUTO(pnum, list)
    printf("num = %d\n", *pnum);

ccxll_free(list);
```

See [test cases](test) for more fascinating examples!

## Contributor

[Kevin Dong (Kʌ̄D)](mailto:kevin.dong.nai.jia@gmail.com) - 2015 ~ 2017

## License

This project is distributed under [the MIT License](LICENSE).



