#pragma once
#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED

#include "bitboard.h"
#include "types.h"

#include <string>
#include <iostream>

namespace Engine {

class Position {

	public:
		Position() : sideToMove(WHITE) {}
		Position(const Position&) = delete;
		explicit Position(std::string fen) {}

		Bitboard pieces(PieceType pt = ALL_PIECES) const;
		template<typename... PieceTypes>
		Bitboard pieces(PieceType pt, PieceTypes...) const;
		Bitboard pieces(Color) const;
		template<typename... PieceTypes>
		Bitboard pieces(Color, PieceTypes...) const;


	private:
		Piece board[SQUARE_NB];
		PieceType byType[PIECE_TYPE_NB];
		Color byColor[COLOR_NB];
		Color sideToMove;

};

std::ostream& operator<<(std::ostream& os, const Position& pos);

inline Bitboard Position::pieces(PieceType pt) const { return byType[pt]; }

template<typename... PieceTypes>
inline Bitboard Position::pieces(PieceType pt, PieceTypes... pts) const { return pieces | pieces(pts...); }

inline Bitboard Position::pieces(Color c) const { return byColor[c]; }

template<typename... PieceTypes>
inline Bitboard Position::pieces(Color c, PieceTypes... pts) const { return pieces(c) & pieces(pts...); }

} // namespace Engine

#endif
