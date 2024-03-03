#include "bit.h"
#include "game.h"

const int piece_evaluation[12] = { 
    100, // white pawn
    300, // white knight
    350, // white bishop
    500, // white rook
    900, // white queen
    10000, // white king
    -100, // black pawn
    -300, // black knight
    -350, // black bishop
    -500, // black rook
    -900, // black queen
    -10000 // black king
};

// pawn positional score
const int pawn_positon[64] = {
       0,  0,  0,  0,  0,  0,  0,  0,
      40, 40, 40, 45, 45, 40, 40, 40,
      25, 25, 30, 35, 35, 30, 25, 25,
      15, 10, 20, 25, 25, 20, 10, 15,
       5,  5, 10, 20, 20, 10,  5,  5,
       5,  0,  0,  5,  5,  0,  0,  5,
       0,  0,  0, -5, -5,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0
};

// knight positional score
const int knight_position[64] = {
    -10,   0,   0,   0,   0,   0,   0, -10,
      0,   0,   5,  10,  10,   5,   0,   0,
      0,   0,  15,  30,  30,  15,   0,   0,
      0,   5,  15,  30,  30,  15,   5,   0,
      0,   5,  15,  30,  30,  15,   5,   0,
      0,   0,  15,  20,  20,  15,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,
    -10, -10, -10, -10, -10, -10, -10, -10
};

// bishop positional score
const int bishop_position[64] = {
      0,  0,   0,   0,    0,   0,   0,   0,
      0,  0,   0,   10,  10,   0,   0,   0,
      0,  0,   10,  15,  15,  10,   0,   0,
      0,  10,  20,  30,  30,  20,  10,   0,
      0,  10,  20,  30,  30,  20,  10,   0,
      0,   0,  15,  20,  20,  15,   0,   0,
      0,  20,  10,  10,  10,  10,  20,   0,
    -10, -10, -10, -10, -10, -10, -10, -10
};

// rook positional score
const int rook_position[64] = {
    50, 50, 50, 50, 50, 50, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50,
     0,  0, 10, 20, 20, 10,  0,  0,
     0,  0, 10, 20, 20, 10,  0,  0,
     0,  0, 10, 20, 20, 10,  0,  0,
     0,  0, 10, 20, 20, 10,  0,  0,
     0,  0, 10, 20, 20, 10,  0,  0,
     0,  0,  0, 20, 20,  0,  0,  0
};

// king early game positional score
const int early_king_position[64] = {
    -70, -70, -70, -70, -70, -70, -70, -70,
    -60, -60, -60, -60, -60, -60, -60, -60,
    -40, -45, -50, -50, -50, -50, -45, -40,
    -30, -30, -30, -40, -40, -30, -30, -30,
    -20, -20,  20, -30, -30, -20, -20, -20,
    -10, -10, -10, -20, -20, -10, -10, -10,
      0,   0,   0, -10, -10,   0,   0,   0,
     20,  25,  20, -10, -10,  15,  25,  20
};

// king end game positional score
const int end_king_position[64] = {
    -10, -10, -10, -10, -10, -10, -10, -10,
    -10,  -5,  -5,  -5,  -5,  -5,  -5, -10,
    -10,  -5,  10,  10,  10,  10,  -5, -10,
    -10,  -5,  10,  20,  20,  10,  -5, -10,
    -10,  -5,  10,  20,  20,  10,  -5, -10,
    -10,  -5,  10,  10,  10,  10,  -5, -10,
    -10,  -5,  -5,  -5,  -5,  -5,  -5, -10,
    -10, -10, -10, -10, -10, -10, -10, -10
};

// mirror position for black pieces
const int mirror[64] {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};


static inline int get_position_score(int piece, int square, int count = 0) {
    // define end game ratio
    const float RATIO = count / 18.0f;

    // mirror square if black piece
    if (piece >= p && piece <= k) {
        square = mirror[square];
    }

    switch (piece) {
        case P: return pawn_positon[square];
        case N: return knight_position[square];
        case B: return bishop_position[square];
        case R: return rook_position[square];
        case Q: return end_king_position[square];
        case K: return (early_king_position[square] * RATIO) + (end_king_position[square] * (1 - RATIO));
        case p: return -pawn_positon[square];
        case n: return -knight_position[square];
        case b: return -bishop_position[square];
        case r: return -rook_position[square];
        case q: return -end_king_position[square];
        case k: return -((early_king_position[square] * RATIO) + (end_king_position[square] * (1 - RATIO)));
    }

    // default return 0
    return 0;
}


int evaluate() {

    // eval score
    int score = 0;

    // init bitboard copy
    u64 bb;

    // init piece and square
    int piece, square;

    // init major piece count
    int major_pieces = 0;

    // loop over pieces
    for (piece = P; piece <= k; piece++) {

        // update bb
        bb = bitboards[piece];

        // loop over all pieces
        while (bb) {

            // increment major piece count
            if (piece != P && piece != p && piece != K && piece != k) {
                major_pieces++;
            }

            // get square
            square = get_lsb_index(bb);

            // remove piece from bb
            pop_bit(bb, square);

            // increment score by evaluation
            score += piece_evaluation[piece];

            // increment score by position
            if (piece != K && piece != k) {
                score += get_position_score(piece, square);
            }

        }

        // positional score for kings
        score += get_position_score(K, get_lsb_index(bitboards[K]), major_pieces);
        score += get_position_score(k, get_lsb_index(bitboards[k]), major_pieces);
    }

    return (side == white) ? score : -score;
}

