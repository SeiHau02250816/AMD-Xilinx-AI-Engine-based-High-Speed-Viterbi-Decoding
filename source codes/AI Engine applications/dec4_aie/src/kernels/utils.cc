#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <adf.h>
#include "include.h"

/*
This function converts a decimal number to a binary number, stored as an array MSB first, having a 
specified number of bits with leading zeroes if necessary.
*/
void dec2bin(int32 d, int32 size, int32 b[]) {
    for (int i=0; i<size; i++) {
        b[i] = 0;
    }

    b[size - 1] = d & 0x01;
    for (int i = size - 2; i>=0; i--) {
        d = d >> 1;
        b[i] = d & 0x01;
    }
}

/*
This function converts a binary number having a specified number of bits to the corresponding decimal number.
*/
int32 bin2dec(int32 b[], int32 size) {
    int32 d = 0;
    for (int i=0; i<size; i++) {
        d += (b[i] << (size - i - 1));
    }

    return d;
}

/*
This function perform pow in terms of int32
*/
int32 pow_int32(int32 data, int32 factor) {
    int32 d = 1;
    while (factor > 0) {
        d *= data;
        factor--;
    }

    return d;
}