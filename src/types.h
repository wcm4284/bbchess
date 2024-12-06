#pragma once
#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <cassert>
#include <cstdint>
#include <iostream>

// look at stockfish for compiler stuff?
// not sure what they're doing yet, but they define popcnt and that's important



namespace Engine {

using Bitboard = uint64_t;

constexpr int MAX_MOVES = 256;
constexpr int MAX_PLY = 246;

enum Color {
	WHITE,
	BLACK,
	COLOR_NB = 2
};

enum CastlingRights {

	WHITE_OO,
	WHITE_OOO = WHITE_OO << 1,
	BLACK_OO  = WHITE_OO << 2,
	BLACK_OOO = WHITE_OO << 3,

	KINGS_SIDE     = WHITE_OO | BLACK_OO,
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
constexpr Value VALUE_INFINITE = 32001;

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

constexpr Value PieceValues[PIECE_NB] = {
	VALUE_ZERO, PawnValue, KnightValue, BishopValue, RookValue, QueenValue, VALUE_ZERO, VALUE_ZERO,
	VALUE_ZERO, PawnValue, KnightValue, BishopValue, RookValue, QueenValue, VALUE_ZERO, VALUE_ZERO
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

// Ranks are the horizontal rows
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

// Files are the vertical rows
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
	CAPTURE   = 2 << 14,
	ENPASSANT = 3 << 14
	
};

constexpr bool is_ok(Square s) { return s >= SQ_A1 && s <= SQ_H8; }

// class Move

class Move {

	friend std::ostream& operator<<(std::ostream& os, const Move& mv) {
		return os << "src: " << mv.src << ", dst: " << mv.dst << ", pc: "
		          << (mv.pc + 2) << ", mt: " << mv.mt << "\n";
	}

	public:
		constexpr explicit Move(uint16_t data) 
			: src(data & 0x3F),
			  dst((data >> 6) & 0x3F),
			  pc((data >> 12) & 0x3),
			  mt((data >> 14) & 0x3) {}

		constexpr Move(Square to, Square from) : src(to), dst(from), pc(0), mt(2) {}

		template <MoveType T>
		constexpr static Move make(Square to, Square from, PieceType pt = KNIGHT) {
			return Move(T + ((pt - KNIGHT) << 12) + (to << 6) + from);
		}

	protected:
		unsigned int src : 6;
		unsigned int dst : 6;
		unsigned int  pc : 2;	
		unsigned int  mt : 2;

};

} // namespace Engine

#endif
