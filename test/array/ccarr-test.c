#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "../../src/ccarr.h"

int main(void)
{
    // Test 0
    // or_val / incr_val

    printf("Test 0: ");

    {
        ccarr(8, 5) arr = CCARR_INIT;

        ccarr_or_val  (arr, 0x0023456789);
        ccarr_incr_val(arr, 0x0100000000);

        for (int i = 4; i >= 0; i--)
            printf("0x%02X ", ELEM_NTH(arr, i));

        puts("");
    }

    // Test 1
    // or_val / xor_arr / ham_arr

    printf("\n\nTest 1: ");

    {
        size_t dist = 0;
        ccarr(16, 2) arr_a = CCARR_INIT, arr_b = CCARR_INIT, arr_c;

        ccarr_or_val(arr_a, 0x3377BBFF);
        ccarr_or_val(arr_b, 0xFFBB7733);

        ccarr_xor_arr(arr_a, arr_b, &arr_c);

        for (int i = 1; i >= 0; i--)
            printf("0x%04X = ", ELEM_NTH(arr_c, i));

        ccarr_ham_arr(arr_a, arr_b,  &dist);

        printf("0xCCCC (%zu = 16)\n", dist);
    }


    return 0;
}
