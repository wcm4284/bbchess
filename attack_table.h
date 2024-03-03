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
extern u64 bishop_masks[64];

// roop attack masks [square]
extern u64 rook_masks[64];

//bishop attacks table [square][occupancies]
extern u64 bishop_attacks[][512];

// rook attacks table [square][occupancies]
extern u64 rook_attacks[][4096];

u64 mask_bishop_attacks(int square);

u64 mask_rook_attacks(int square);

u64 bishop_attacks_otf(int square, u64 block);

u64 rook_attacks_otf(int square, u64 block);

u64 set_occupancy(int index, int bits_in_mask, u64 attack_mask);

u64 get_bishop_attacks(int square, u64 occupancy);

u64 get_rook_attacks(int square, u64 occupancy);

u64 get_queen_attacks(int square, u64 occupancy);

int is_square_attacked(int square, int side);

#endif