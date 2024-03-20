#ifndef HASH_H
#define HASH_H

// initialize hash keys for 
void init_hash_keys();

// hash a current position to be stored in the transposition table
// this function should really only be called once at game start
// other updates to the hash will be occurring in the make_move
// function from move.h
u64 hash();

// 8 byte quantity to store the hash value of the current board position
extern u64 hkey;

extern u64 piece_keys[12][64];

extern u64 en_passant_keys[64];

extern u64 castling_keys[16];

extern u64 side_key;

#endif
