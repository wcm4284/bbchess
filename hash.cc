#include <stdio.h>
#include "bit.h"
#include "game.h"
#include "rand.h"

u64 hkey;

// random piece keys [piece] [square]
u64 piece_keys[12][64];

// random en passant keys [square] (includes no square)
u64 en_passant_keys[64];

// random castling keys
u64 castling_keys[16];

// random side key
u64 side_key;

bool initialized = false;

// initialize all keys to random numbers
void init_hash_keys() {

    initialized = true;

    state = 1804289383;

    // loop over pieces in piece_keys
    for (int i = 0; i < 12; i++) {
        
        // loop over squares in piece_keys
        for (int j = 0; j < 64; j++) {

            // initialize to random number
            piece_keys[i][j] = get_random_u64_number();
        }
    }

    // loop over en_passant_keys
    for (int i = 0; i < 64; i++) {
        // initialize to random numbers
        en_passant_keys[i] = get_random_u64_number();
    }

    // loop over castling keys
    for (int i = 0; i < 16; i++) {
        // initialize to random numbers
        castling_keys[i] = get_random_u64_number();
    }

    // init side key
    side_key = get_random_u64_number();
}

int hash() {

    if (!initialized) printf("ERROR!");

    // initialize hash number
    int key = 0;

    // xor by castle
    key ^= castling_keys[castle];

    // xor by side to move
    if (side == white) key ^= side_key;

    // xor by en passant square
    key ^= en_passant_keys[en_passant];

    for (int i = P; i <= k; i++)
    {
        u64 bb = bitboards[i];

        while (bb)
        {
            int lsb = get_lsb_index(bb);

            key ^= piece_keys[i][lsb];

            pop_bit(bb, lsb);
        }
    }

    return key;
}

