#ifndef GAME_H
#define GAME_H

#include <map>
#include <string>
#include "bit.h"
#include "hash.h"

// useful fen strings for debugging/testing
extern const char* empty_board;
extern const char* start_position;
extern const char* tricky_position;
extern const char* killer_position;

// board squares
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

// colors
enum { white, black, both };

// bishop and rook
enum { rook, bishop };

// castling flags
enum { wk = 1, wq = 2, bk = 4, bq = 8 };

//encode pieces
enum { P, N, B, R, Q, K, p, n, b, r, q, k };

// converts an enum square to a string coordinate
extern const char* square_to_coord[];

// ASCII pieces
extern const char ASCII_pieces[13];

// unicode pieces
extern const char* unicode_pieces[13];

// ascii to unicode converter
extern std::map<char, int> char_pieces;

// piece bitboard 
extern u64 bitboards[12];

// occupancy bitboards
extern u64 occupancies[3];

// side to move
extern int side;

// en passant square
extern int en_passant;

/* castling rights defined as follows
     bin  dec 

    0001    1   white king can castle to the king side
    0010    2   white king can castle to the queen side
    0100    3   black king can castle to the king side
    1000    4   black king can castle to the queen side
*/
extern int castle;

#define copy_board()                                                    \
    u64 bitboards_copy[12], occupancies_copy[3], hkey_copy;             \
    int side_copy, enpassant_copy, castle_copy;                         \
    memcpy(bitboards_copy, bitboards, sizeof(bitboards));               \
    memcpy(occupancies_copy, occupancies, sizeof(occupancies));         \
    side_copy = side, enpassant_copy = en_passant,                      \
    hkey_copy = hkey, castle_copy = castle;                             \

#define restore_board()                                                 \
    memcpy(bitboards, bitboards_copy, sizeof(bitboards));               \
    memcpy(occupancies, occupancies_copy, sizeof(occupancies));         \
    side = side_copy, en_passant = enpassant_copy,                      \
    hkey = hkey_copy, castle = castle_copy;                             \


#endif
