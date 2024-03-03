#ifndef BIT_H
#define BIT_H

typedef unsigned long long u64;

#define get_bit(bitboard, square) ((bitboard) & (1ull << (square)))
#define set_bit(bitboard, square) ((bitboard) |= 1ull << (square))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))


// taken from https://www.reddit.com/r/C_Programming/comments/jpt7dw/what_is_the_proper_way_to_count_bits_which_are/
#if defined(__GNUC__) || defined(__clang__)
  static inline u64 cpopcount64(u64 x) {
    return __builtin_popcountll(x);
  }
#elif defined(_MSC_VER) && defined(_WIN64)
  #include <intrin.h>
  static inline u64 cpopcount64(u64 x) {
    return __popcnt64(x);
  }
#else
  /* http://en.wikipedia.org/wiki/Hamming_weight */
  static inline u64 cpopcount64(u64 x) {
    x -= (x >> 1) & 0x5555555555555555;
    x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
    return (x * 0x0101010101010101) >> 56;
  }
#endif

int get_lsb_index(u64 bb);

#endif
