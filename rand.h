#ifndef RAND_H
#define RAND_H

extern unsigned int state;

// generate 32 bit pseudo random numbers
unsigned int get_random_u32_number();

// generate 64 bit pseudo random numbers
u64 get_random_u64_number();

#endif
