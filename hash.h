#ifndef HASH_H
#define HASH_H

void init_hash_keys();
void hash();

extern u64 piece_keys[12][64];
extern u64 en_passant_keys[64];
extern u64 castling_keys[16];
extern u64 side_key;

extern u64 hkey;

#endif
