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

// taken from https://www.chessprogramming.org/BitScan#:~:text=A%2064%2Dbit%20De%20Bruijn,five%20hidden%20%22trailing%22%20zeros.
const int lsb_64_table[64] =
{
   63, 30,  3, 32, 59, 14, 11, 33,
   60, 24, 50,  9, 55, 19, 21, 34,
   61, 29,  2, 53, 51, 23, 41, 18,
   56, 28,  1, 43, 46, 27,  0, 35,
   62, 31, 58,  4,  5, 49, 54,  6,
   15, 52, 12, 40,  7, 42, 45, 16,
   25, 57, 48, 13, 10, 39,  8, 44,
   20, 47, 38, 22, 17, 37, 36, 26
};

int get_lsb_index(u64 bb) {
    if (bb == 0) return -1;
   unsigned int folded;
   bb ^= bb - 1;
   folded = (int) bb ^ (bb >> 32);
   return lsb_64_table[folded * 0x78291ACF >> 26];
}

#endif
