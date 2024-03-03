#include "bit.h"
#include "game.h"
#include "magic.h"

// bishop relevant occupancy bit count
const int bishop_relevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

// rook relevant occupancy bit count
const int rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

// pawn attack table [side][square]
const u64 pawn_attacks[2][64] = {
{   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
    0x2, 0x5, 0xA, 0x14, 0x28, 0x50, 0xA0, 0x40,
    0x200, 0x500, 0xA00, 0x1400, 0x2800, 0x5000, 0xA000, 0x4000,
    0x20000, 0x50000, 0xA0000, 0x140000, 0x280000, 0x500000, 0xA00000, 0x400000,
    0x2000000, 0x5000000, 0xA000000, 0x14000000, 0x28000000, 0x50000000, 0xA0000000, 0x40000000,
    0x200000000, 0x500000000, 0xA00000000, 0x1400000000, 0x2800000000, 0x5000000000, 0xA000000000, 0x4000000000,
    0x20000000000, 0x50000000000, 0xA0000000000, 0x140000000000, 0x280000000000, 0x500000000000, 0xA00000000000, 0x400000000000,
    0x2000000000000, 0x5000000000000, 0xA000000000000, 0x14000000000000, 0x28000000000000, 0x50000000000000, 0xA0000000000000, 0x40000000000000 
}, 
{   0x200, 0x500, 0xA00, 0x1400, 0x2800, 0x5000, 0xA000, 0x4000,
    0x20000, 0x50000, 0xA0000, 0x140000, 0x280000, 0x500000, 0xA00000, 0x400000,
    0x2000000, 0x5000000, 0xA000000, 0x14000000, 0x28000000, 0x50000000, 0xA0000000, 0x40000000,
    0x200000000, 0x500000000, 0xA00000000, 0x1400000000, 0x2800000000, 0x5000000000, 0xA000000000, 0x4000000000,
    0x20000000000, 0x50000000000, 0xA0000000000, 0x140000000000, 0x280000000000, 0x500000000000, 0xA00000000000, 0x400000000000,
    0x2000000000000, 0x5000000000000, 0xA000000000000, 0x14000000000000, 0x28000000000000, 0x50000000000000, 0xA0000000000000, 0x40000000000000,
    0x200000000000000, 0x500000000000000, 0xA00000000000000, 0x1400000000000000, 0x2800000000000000, 0x5000000000000000, 0xA000000000000000, 0x4000000000000000,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 
}};

// knight attack table [square]
const u64 knight_attacks[64] = 
{   0x20400, 0x50800, 0xA1100, 0x142200, 0x284400, 0x508800, 0xA01000, 0x402000,
    0x2040004, 0x5080008, 0xA110011, 0x14220022, 0x28440044, 0x50880088, 0xA0100010, 0x40200020,
    0x204000402, 0x508000805, 0xA1100110A, 0x1422002214, 0x2844004428, 0x5088008850, 0xA0100010A0, 0x4020002040,
    0x20400040200, 0x50800080500, 0xA1100110A00, 0x142200221400, 0x284400442800, 0x508800885000, 0xA0100010A000, 0x402000204000,
    0x2040004020000, 0x5080008050000, 0xA1100110A0000, 0x14220022140000, 0x28440044280000, 0x50880088500000, 0xA0100010A00000, 0x40200020400000,
    0x204000402000000, 0x508000805000000, 0xA1100110A000000, 0x1422002214000000, 0x2844004428000000, 0x5088008850000000, 0xA0100010A0000000, 0x4020002040000000,
    0x400040200000000, 0x800080500000000, 0x1100110A00000000, 0x2200221400000000, 0x4400442800000000, 0x8800885000000000, 0x100010A000000000, 0x2000204000000000,
    0x4020000000000, 0x8050000000000, 0x110A0000000000, 0x22140000000000, 0x44280000000000, 0x88500000000000, 0x10A00000000000, 0x20400000000000
};

// king attack table [square]
const u64 king_attacks[64] = 
{   0x302, 0x705, 0xE0A, 0x1C14, 0x3828, 0x7050, 0xE0A0, 0xC040,
    0x30203, 0x70507, 0xE0A0E, 0x1C141C, 0x382838, 0x705070, 0xE0A0E0, 0xC040C0,
    0x3020300, 0x7050700, 0xE0A0E00, 0x1C141C00, 0x38283800, 0x70507000, 0xE0A0E000, 0xC040C000,
    0x302030000, 0x705070000, 0xE0A0E0000, 0x1C141C0000, 0x3828380000, 0x7050700000, 0xE0A0E00000, 0xC040C00000,
    0x30203000000, 0x70507000000, 0xE0A0E000000, 0x1C141C000000, 0x382838000000, 0x705070000000, 0xE0A0E0000000, 0xC040C0000000,
    0x3020300000000, 0x7050700000000, 0xE0A0E00000000, 0x1C141C00000000, 0x38283800000000, 0x70507000000000, 0xE0A0E000000000, 0xC040C000000000,
    0x302030000000000, 0x705070000000000, 0xE0A0E0000000000, 0x1C141C0000000000, 0x3828380000000000, 0x7050700000000000, 0xE0A0E00000000000, 0xC040C00000000000,
    0x203000000000000, 0x507000000000000, 0xA0E000000000000, 0x141C000000000000, 0x2838000000000000, 0x5070000000000000, 0xA0E0000000000000, 0x40C0000000000000,
};

// bishop attack masks [square]
u64 bishop_masks[64];

// roop attack masks [square]
u64 rook_masks[64];

//bishop attacks table [square][occupancies]
u64 bishop_attacks[64][512];

// rook attacks table [square][occupancies]
u64 rook_attacks[64][4096];

u64 mask_bishop_attacks(int square) {

    u64 attacks = 0ull;

    // initialize rank and file vars
    int r, f;

    // initialize target vars
    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; ++r, ++f) attacks |= (1ull << (r * 8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; ++r, --f) attacks |= (1ull << (r * 8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; --r, ++f) attacks |= (1ull << (r * 8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; --r, --f) attacks |= (1ull << (r * 8 + f));


    return attacks;

}

u64 mask_rook_attacks(int square) {

    u64 attacks = 0ull;

    // initialize rank and file vars
    int r, f;

    // initialize target vars
    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf; r <= 6; ++r) attacks |= (1ull << (r * 8 + f));
    for (r = tr - 1, f = tf; r >= 1; --r) attacks |= (1ull << (r * 8 + f));
    for (r = tr, f = tf + 1;f <= 6; ++f) attacks |= (1ull << (r * 8 + f));
    for (r = tr, f = tf - 1;f >= 1; --f) attacks |= (1ull << (r * 8 + f));


    return attacks;
}

u64 bishop_attacks_otf(int square, u64 block) {
    
    u64 attacks = 0ull;

    // initialize rank and file vars
    int r, f;

    // initialize target vars
    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; ++r, ++f) {
        attacks |= (1ull << (r * 8 + f));
        if ((1ull << (r * 8 + f)) & block) break;
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; ++r, --f) {
        attacks |= (1ull << (r * 8 + f));
        if ((1ull << (r * 8 + f)) & block) break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; --r, ++f) {
        attacks |= (1ull << (r * 8 + f));
        if ((1ull << (r * 8 + f)) & block) break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; --r, --f) {
        attacks |= (1ull << (r * 8 + f));
        if ((1ull << (r * 8 + f)) & block) break;
    }

    return attacks;

}

u64 rook_attacks_otf(int square, u64 block) {

    u64 attacks = 0ull;

    // initialize rank and file vars
    int r, f;

    // initialize target vars
    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf; r <= 7; ++r) {
        attacks |= (1ull << (r * 8 + f));
        if ((1ull << (r * 8 + f)) & block) break;
    }
    for (r = tr - 1, f = tf; r >= 0; --r) {
        attacks |= (1ull << (r * 8 + f));
        if ((1ull << (r * 8 + f)) & block) break;
    }
    for (r = tr, f = tf + 1;f <= 7; ++f) {
        attacks |= (1ull << (r * 8 + f));
        if ((1ull << (r * 8 + f)) & block) break;
    }
    for (r = tr, f = tf - 1;f >= 0; --f) {
        attacks |= (1ull << (r * 8 + f));
        if ((1ull << (r * 8 + f)) & block) break;
    }

    return attacks;
}

u64 set_occupancy(int index, int bits_in_mask, u64 attack_mask) {
    u64 occupancy = 0ull;

    for (int count = 0; count < bits_in_mask; ++count) {

        int square = get_lsb_index(attack_mask);

        pop_bit(attack_mask, square);

        if (index & (1 << count)) {
            occupancy |= (1ull << square);
        }
    }

    return occupancy;
}

// get bishop attacks
u64 get_bishop_attacks(int square, u64 occupancy) {

    // get bishop attacks assuming current board occupancy
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= (64 - bishop_relevant_bits[square]);

    return bishop_attacks[square][occupancy];
}

// get rook attacks
u64 get_rook_attacks(int square, u64 occupancy) {

    // get rook attacks assuming current board occupancy
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= (64 - rook_relevant_bits[square]);

    return rook_attacks[square][occupancy];
}

// get queen attacks
u64 get_queen_attacks(int square, u64 occupancy) {

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

// check is square is attacked
int is_square_attacked(int square, int side) {
    
    // check white pawn
    if ((side == white) && (pawn_attacks[black][square] & bitboards[P])) return 1;

    // check black pawn
    if ((side == black) && (pawn_attacks[white][square] & bitboards[p])) return 1;
    
    // check knight
    if (knight_attacks[square] & ((side == white) ? bitboards[N] : bitboards[n])) return 1;
    
    // check bishop
    if (get_bishop_attacks(square, occupancies[both]) & ((side == white) ? bitboards[B] : bitboards[b])) return 1;

    // check rook
    if (get_rook_attacks(square, occupancies[both]) & ((side == white) ? bitboards[R] : bitboards[r])) return 1;

    // check queen
    if (get_queen_attacks(square, occupancies[both]) & ((side == white) ? bitboards[Q] : bitboards[q])) return 1;
    
    // check king
    if (king_attacks[square] & ((side == white) ? bitboards[K] : bitboards[k])) return 1; 

    // default return 0
    return 0;
}

