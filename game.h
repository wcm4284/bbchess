#ifndef GAME_H
#define GAME_H

#include <map>

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

const char* square_to_coord[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1" 
};

// ASCII pieces
const char ASCII_pieces[13] = "PNBRQKpnbrqk";

// unicode pieces
const char* unicode_pieces[13] = {"♟", "♞", "♝", "♜", "♛", "♚", "♙", "♘", "♗", "♖", "♕", "♔"};

// ascii to unicode converter
std::map<char, int> char_pieces = {
    {'P', P},
    {'N', N},
    {'B', B},
    {'R', R},
    {'Q', Q},
    {'K', K},
    {'p', p},
    {'b', b},
    {'n', n},
    {'r', r},
    {'q', q},
    {'k', k}
};

// piece bitboard 
u64 bitboards[12] = {
    0xFF000000000000,
    0x2400000000000000,
    0x4200000000000000,
    0x8100000000000000,
    0x0800000000000000,
    0x1000000000000000,
    0xFF00,
    0x42,
    0x24,
    0x81,
    0x8,
    0x10
};

// occupancy bitboards
u64 occupancies[3];

// side to move
int side = 0;

// en passant square
int en_passant = no_sq;

/* castling rights defined as follows
     bin  dec 

    0001    1   white king can castle to the king side
    0010    2   white king can castle to the queen side
    0100    3   black king can castle to the king side
    1000    4   black king can castle to the queen side
*/
int castle = 0xF;

#endif
