#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "../../src/array/ccarr.h"

int main(void)
{
    // Test 0
    // or_val / incr_val

    printf("Test 0: ");

    {
        ccarr(8, 5) arr = CCARR_INIT(arr);

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
        ccarr(16, 2) arr_a = CCARR_INIT(arr_a),
                     arr_b = CCARR_INIT(arr_b), arr_c;

        ccarr_or_val(arr_a, 0x3377BBFF);
        ccarr_or_val(arr_b, 0xFFBB7733);

        ccarr_xor_arr(arr_a, arr_b, &arr_c);

        for (int i = 1; i >= 0; i--)
            printf("0x%04X = ", ELEM_NTH(arr_c, i));

        ccarr_ham_arr(arr_a, arr_b,  &dist);

        printf("0xCCCC (%zu = 16)\n", dist);
    }

    // Test 2
    // set_bit / check_bit

    printf("\n\nTest 2: ");

    {
        ccarr(32, 4) arr = CCARR_INIT(arr);

        ccarr_set_bit(arr, 64);

        printf("%s\n", (ccarr_check_bit(arr, 64) == 1) ? "SUCCESS" : "FAILURE");
    }


    return 0;
}
