#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <cassert>
#include <cstdint>
#include <array>
#include <unordered_map>
#include <string>
#include <iostream>

// look at stockfish for compiler stuff?
// not sure what they're doing yet, but they define popcnt and that's important

#ifdef USE_PEXT
    #include <immintrin.h>
    #define pext(b, m) _pext_u64(b, m)
#else
    #define pext(b, m) 0
#endif

#define popcnt(x) __builtin_popcountll(x)

namespace Engine {

using Bitboard = uint64_t;
using Depth = int;

constexpr int MAX_MOVES = 256;
constexpr int MAX_PLY = 246;

enum Color {
    WHITE,
    BLACK,
    COLOR_NB = 2
};

enum CastlingRights {

    WHITE_OO  = 1,
    WHITE_OOO = WHITE_OO << 1,
    BLACK_OO  = WHITE_OO << 2,
    BLACK_OOO = WHITE_OO << 3,

    KING_SIDE      = WHITE_OO | BLACK_OO,
    QUEEN_SIDE     = WHITE_OOO | BLACK_OOO,
    WHITE_CASTLING = WHITE_OO | WHITE_OOO,
    BLACK_CASTLING = BLACK_OO | BLACK_OOO,
    ANY_CASTLING   = WHITE_CASTLING | BLACK_CASTLING,

    CASTLE_RIGHT_NB = 16
    
};

enum Bound {
    BOUND_NONE,
    BOUND_UPPER,
    BOUND_LOWER,
    BOUND_EXACT = BOUND_UPPER | BOUND_LOWER,
};



// Value is used as an alias for int in cases where
// the value refers to a position evaluation. The values
// from a search should always be in the range 
// (-VALUE_NONE, VALUE_NONE]
using Value = int;

constexpr Value VALUE_ZERO     = 0;
constexpr Value VALUE_DRAW     = 0;
constexpr Value VALUE_NONE     = 32002;
constexpr Value VALUE_INF = 32001;

constexpr Value VALUE_MATE             = 32000;
constexpr Value VALUE_MATE_IN_MAX_PLY  = VALUE_MATE - MAX_PLY;
constexpr Value VALUE_MATED_IN_MAX_PLY = -VALUE_MATE_IN_MAX_PLY;

constexpr Value VALUE_TB                 = VALUE_MATE_IN_MAX_PLY - 1;
constexpr Value VALUE_TB_WIN_IN_MAX_PLY  = VALUE_TB - MAX_PLY;
constexpr Value VALUE_TB_LOSS_IN_MAX_PLY = -VALUE_TB_WIN_IN_MAX_PLY;

constexpr Value PawnValue   = 100;
constexpr Value KnightValue = 300;
constexpr Value BishopValue = 315;
constexpr Value RookValue   = 500;
constexpr Value QueenValue  = 900;

enum PieceType {

    NO_PIECE_TYPE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
    ALL_PIECES = 0,
    PIECE_TYPE_NB = 8
};

enum Piece {
    NO_PIECE,

    W_PAWN = PAWN,     W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN = PAWN + 8, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,

    PIECE_NB = 16

};

constexpr Value PieceValues[PIECE_TYPE_NB] = {
    VALUE_ZERO, PawnValue, KnightValue, BishopValue, RookValue, QueenValue, VALUE_ZERO//, VALUE_ZERO
    //VALUE_ZERO, -PawnValue, -KnightValue, -BishopValue, -RookValue, -QueenValue, VALUE_ZERO, VALUE_ZERO
};

enum Square : int {
    
    SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
    SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
    SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
    SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
    SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
    SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
    SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
    SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
    SQ_NONE,
    
    SQUARE_ZERO = 0,
    SQUARE_NB   = 64
};

const std::array<std::string, 65> printSquare = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", 
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "-"
};

enum Direction {

    NORTH = 8,
    EAST  = 1,
    SOUTH = -NORTH,
    WEST  = -EAST,

    NORTH_EAST = NORTH + EAST,
    SOUTH_EAST = SOUTH + EAST,
    NORTH_WEST = NORTH + WEST,
    SOUTH_WEST = SOUTH + WEST,

};

// Ranks are the rows
enum Rank : int {
    
    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,

    RANK_NB = 8
};

// Files are the columns
enum File : int {

    FILE_A,
    FILE_B,
    FILE_C,
    FILE_D,
    FILE_E,
    FILE_F,
    FILE_G,
    FILE_H,

    FILE_NB = 8
};

// enum MoveType
enum MoveType {
    NORMAL,
    PROMOTION = 1 << 14,
    CASTLING  = 2 << 14,
    ENPASSANT = 3 << 14
};

namespace {

    std::unordered_map<MoveType, std::string> mt_conv = {
        {NORMAL, "normal"},
        {PROMOTION, "promotion"},
        {CASTLING, "castle"},
        {ENPASSANT, "en passant"}
    };

    std::unordered_map<PieceType, std::string> pt_conv = {
        {KNIGHT, "knight"},
        {BISHOP, "bishop"},
        {ROOK, "rook"},
        {QUEEN, "queen"}
    };

}

#define ENABLE_INC_OPERATOR_ON(T) \
        inline T& operator++(T& t) { return t = T(int(t) + 1); } \
        inline T& operator--(T& t) { return t = T(int(t) - 1); }

    ENABLE_INC_OPERATOR_ON(Square)
    ENABLE_INC_OPERATOR_ON(Rank)
    ENABLE_INC_OPERATOR_ON(File)

#undef ENABLE_INC_OPERATOR_ON

constexpr Direction operator+(Direction d1, Direction d2) { return Direction(int(d1) + int(d2)); }
constexpr Direction operator-(Direction d1, Direction d2) { return Direction(int(d1) - int(d2)); }
constexpr Direction operator*(int i, Direction d) { return Direction(i * int(d)); }

inline Square operator+(Square s, Direction d) { return Square(int(s) + int(d)); }
inline Square operator-(Square s, Direction d) { return Square(int(s) - int(d)); }
inline Square& operator+=(Square& s, Direction d) { return s = s + d; }
inline Square& operator-=(Square& s, Direction d) { return s = s - d; }


// toggle color
constexpr Color operator~(Color c) { return Color(c ^ BLACK); }

constexpr CastlingRights operator~(CastlingRights cr) { return CastlingRights(~int(cr) & 0x1f); }
inline CastlingRights& operator|=(CastlingRights& c, CastlingRights cr) { return c = CastlingRights(c | cr); }
inline CastlingRights& operator&=(CastlingRights& c, CastlingRights cr) { return c = CastlingRights(c & cr); }

constexpr Piece make_piece(PieceType p, Color c) { return Piece(int(p) + (int(c) * 8)); }

// flip rank A1 -> A8
constexpr Square flip_rank(Square s) { return Square(s ^ SQ_A8); }

// flip file A1 -> H1
constexpr Square flip_file(Square s) { return Square(s ^ SQ_H1); }

constexpr Square flip_square(Square s) { return flip_rank(flip_file(s)); }

constexpr bool is_ok(Square s) { return s >= SQ_A1 && s <= SQ_H8; }

constexpr Direction push_dir(Color c) { return c == WHITE ? NORTH : SOUTH; }

constexpr Square make_square(File f, Rank r) { return Square((r * 8) + f); }

constexpr File file_of(Square s) { return File(s & 7); }

constexpr Rank rank_of(Square s) { return Rank(s >> 3); }

constexpr Rank relative_rank(Rank r, Color c) { return Rank(c == WHITE ? r : RANK_8 - r); }

constexpr Color color_of(Piece pc) { return pc >= B_PAWN ? BLACK : WHITE; }

constexpr PieceType type_of(Piece pc) { return PieceType(pc >= B_PAWN ? pc - 8 : pc); }

constexpr Value mated_in(int ply) { return -VALUE_MATE + ply; }
constexpr Value mate_in(int ply)  { return  VALUE_MATE - ply; }

class Move {

    friend std::ostream& operator<<(std::ostream& os, const Move& mv) {
        os << printSquare[mv.from_sq()] << " to " << printSquare[mv.to_sq()] << 
        "; mt: " << mt_conv[mv.type()];

        if (mv.type() == PROMOTION) 
            return os << ", pt: " << pt_conv[mv.promote_to()] << std::endl;

        return os << std::endl;

    }

    public:
        constexpr Move() : data(0) {}
        constexpr explicit Move(uint16_t raw) : data(raw) {}

        constexpr Move(Square to, Square from) : data(from | (to << 6)) {}


        inline Square from_sq() const { return Square(data & 0x3f); }
        inline Square to_sq() const { return Square((data & 0xfc0) >> 6); }
        inline PieceType promote_to() const { return PieceType(((data & 0x3000) >> 12) + 2); }
        inline MoveType type() const { return MoveType(data & 0xc000); }
        inline bool is_ok() const { return from_sq() >= SQ_A1 && from_sq() <= SQ_H8 &&
                                    to_sq() >= SQ_A1 && to_sq() <= SQ_H8 && from_sq() != to_sq(); }

        template <MoveType T>
        constexpr static Move make(Square to, Square from, PieceType pt = KNIGHT) {
            return Move(T + ((pt - KNIGHT) << 12) + (to << 6) + from);
        }

        constexpr uint16_t raw() const { return data; }

    protected:
        uint16_t data;

};

} // namespace Engine

#endif
