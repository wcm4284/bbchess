#ifndef MOVE_H
#define MOVE_H

#include <stdlib.h>
#include "game.h"

/*
            binary move encoding                                  hex constant

        0000 0000 0000 0000 0011 1111   source square         0x3f
        0000 0000 0000 1111 1100 0000   target square         0xfc0
        0000 0000 1111 0000 0000 0000   piece                 0xf000
        0000 1111 0000 0000 0000 0000   promoted piece        0xf0000
        0001 0000 0000 0000 0000 0000   capture flag          0x100000
        0010 0000 0000 0000 0000 0000   double push flag      0x200000
        0100 0000 0000 0000 0000 0000   en passant flag       0x400000
        1000 0000 0000 0000 0000 0000   castle flag           0x800000
*/

// encode move
#define encode_move(source, target, piece, promoted, capture, double, enpassant, castle) \
    (source) |          \
    (target << 6) |     \
    (piece << 12) |     \
    (promoted << 16) |  \
    (capture << 20) |   \
    (double << 21) |    \
    (enpassant << 22) | \
    (castle << 23)

// extract source square
#define get_move_source(move) (move & 0x3f)

// extract target square
#define get_move_target(move) ((move & 0xfc0) >> 6)

// extract piece
#define get_move_piece(move) ((move & 0xf000) >> 12)

// extract promoted piece
#define get_move_promoted(move) ((move & 0xf0000) >> 16)

// extract capture flag
#define get_move_capture(move) (move & 0x100000)

// extract double push flag
#define get_move_double(move) (move & 0x200000)

// extract enpassant flag
#define get_move_enpassant(move) (move & 0x400000)

// extract castle flag
#define get_move_castle(move) (move & 0x800000)

// move list struct
typedef struct {
    
    // moves
    int moves[256];

    // num moves
    int count = 0;

} moves;

// add move to move list 
static inline void add_move(moves* move_list, int move);

// print move
void print_move(int move);

// print move list
void print_move_list(moves* move_list);

// define enum for move types
enum { all_moves, only_captures };
static inline int make_move(int move, int move_flag);

#endif
