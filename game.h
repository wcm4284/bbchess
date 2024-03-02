#ifndef GAME_H
#define GAME_H

#include <map>
#include <string>
#include "bit.h"

// useful fen strings for debugging/testing
const char* empty_board = "8/8/8/8/8/8/8/8 w - - ";
const char* start_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ";
const char* tricky_position = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
const char* killer_position = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";

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

#define copy_board()                                                    \
    u64 bitboards_copy[12], occupancies_copy[3];                        \
    int side_copy, enpassant_copy, castle_copy;                         \
    memcpy(bitboards_copy, bitboards, sizeof(bitboards));               \
    memcpy(occupancies_copy, occupancies, sizeof(occupancies));         \
    side_copy = side, enpassant_copy = en_passant, castle_copy = castle \

#define restore_board()                                                 \
    memcpy(bitboards, bitboards_copy, sizeof(bitboards));               \
    memcpy(occupancies, occupancies_copy, sizeof(occupancies));         \
    side = side_copy, en_passant = enpassant_copy, castle = castle_copy \


#endif
