#ifndef RAND_H
#define RAND_H

#include "debug.h"

unsigned int state = 1804289383;

// generate 32 bit pseudo random numbers
unsigned int get_random_u32_number() {

    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;

    return state;

}

// generate 64 bit pseudo random numbers
u64 get_random_u64_number() {

    // define 4 random numbers
    u64 n1, n2, n3, n4;

    // initialize random numbers slicing 16 bits from msb side
    n1 = (u64) (get_random_u32_number()) & 0xFFFF;
    n2 = (u64) (get_random_u32_number()) & 0xFFFF;
    n3 = (u64) (get_random_u32_number()) & 0xFFFF;
    n4 = (u64) (get_random_u32_number()) & 0xFFFF;

    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

#endif
