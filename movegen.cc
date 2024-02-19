#include "movegen.h"

// get bishop attacks
static inline u64 get_bishop_attacks(int square, u64 occupancy) {

    // get bishop attacks assuming current board occupancy
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= (64 - bishop_relevant_bits[square]);

    return bishop_attacks[square][occupancy];
}

// get rook attacks
static inline u64 get_rook_attacks(int square, u64 occupancy) {

    // get rook attacks assuming current board occupancy
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= (64 - rook_relevant_bits[square]);

    return rook_attacks[square][occupancy];
}

// get queen attacks
static inline u64 get_queen_attacks(int square, u64 occupancy) {

    u64 bishop_occupancy = occupancy;

    u64 rook_occupancy = occupancy;

    // get rook attacks assuming current board occupancy
    rook_occupancy &= rook_masks[square];
    rook_occupancy *= rook_magic_numbers[square];
    rook_occupancy >>= (64 - rook_relevant_bits[square]);

    // get bishop attacks assming current board occupancy
    bishop_occupancy &= bishop_masks[square];
    bishop_occupancy *= bishop_magic_numbers[square];
    bishop_occupancy >>= (64 - bishop_relevant_bits[square]);
    
    // bitor rook and bishop attacks together for queen attacks
    return rook_attacks[square][rook_occupancy] | bishop_attacks[square][bishop_occupancy];
}
