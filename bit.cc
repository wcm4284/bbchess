#include "bit.h"

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