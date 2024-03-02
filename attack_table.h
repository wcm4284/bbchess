#ifndef ATTACK_TABLE_H
#define ATTACK_TABLE_H

#include "game.h"

extern const int bishop_relevant_bits[];
extern const int rook_relevant_bits[];

// attack tables for non-sliding pieces

// pawn attacks [side] [square]
extern const u64 pawn_attacks[][64];
// knight attacks [square]
extern const u64 knight_attacks[];
// king attacks [square]
extern const u64 king_attacks[];

// bishop attack masks [square]
u64 bishop_masks[64];

// roop attack masks [square]
u64 rook_masks[64];

//bishop attacks table [square][occupancies]
u64 bishop_attacks[64][512];

// rook attacks table [square][occupancies]
u64 rook_attacks[64][4096];

u64 mask_bishop_attacks(int square);

u64 mask_rook_attacks(int square);

u64 bishop_attacks_otf(int square, u64 block);

u64 rook_attacks_otf(int square, u64 block);

u64 set_occupancy(int index, int bits_in_mask, u64 attack_mask);

static inline u64 get_bishop_attacks(int square, u64 occupancy);

static inline u64 get_rook_attacks(int square, u64 occupancy);

static inline u64 get_queen_attacks(int square, u64 occupancy);

static inline int is_square_attacked(int square, int side);

#endif