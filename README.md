## OpenGC<sup>3</sup>

### Containers

|  Type                             |  Description                          |
|-----------------------------------|:-------------------------------------:|
|  `ccarr(T)`                       |  Array of Bits                        |
|  `ccdll(T)`                       |  Doubly Linked List                   |
|  `ccgbt(T)`                       |  General Binary Tree                  |
| [`ccxll(T)`](tool/ccxll-call.pdf) | [XOR Linked List](doc/ccxll-list.pdf) |

### Example

```c
ccxll(int) list;                      //  declare a list of type int
ccxll_init(list);                     //  initialize the list record

for (int cnt = 8; cnt --> 0; )        //
    ccxll_push_back(list, rand());    //  insert "rand()" to the end

ccxll_sort(list);                     //  sort with comparator: XLEQ

CCXLL_INCR_AUTO(pnum, list)           //  traverse the list forward:
    printf("num = %d\n", *pnum);      //  access elems through iters

ccxll_free(list);                     //  destroy the list after use
```

See [test cases](test) for more fascinating examples!

### Contributor

[Kevin Dong (Kʌ̄D)](mailto:kevin.dong.nai.jia@gmail.com) - 2015 ~ 2017

### License

This project is distributed under [the MIT License](LICENSE).


