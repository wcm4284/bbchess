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
		Position() = default;
		Position(const Position&) = delete;

		void set_fen(std::string_view); // maybe a string is fine
		std::string fen() const;
		
		// used for move generation
		Bitboard pieces(PieceType pt = ALL_PIECES) const;
		template<typename... PieceTypes>
		Bitboard pieces(PieceType pt, PieceTypes...) const;
		Bitboard pieces(Color) const;
		template<typename... PieceTypes>
		Bitboard pieces(Color, PieceTypes...) const;

		bool capture(Move*) const;
		bool empty(Square) const;
		
		// used for printing to console
		constexpr Piece piece_on(Square) const;
		constexpr Color to_play() const;
		constexpr Square en_passant() const;
		
		// determines if any pieces are pinned to PieceType
		// will be primarily used for evaluation, but can
		// also probably be used for move ordering
		// @returns pieces of Color that are pinned to PieceType
		Bitboard pinned(Color, PieceType = KING) const;
		
		// returns a bitboard filled with pieces that are currently attacking a king.
		// since both kings cannot be in check at the same time, we have no need to
		// pass a color through
		Bitboard checkers() const;

		bool can_castle(CastlingRights) const;


		void put_piece(Piece, Square);
		void move_piece(Square, Square);
		Piece remove_piece(Square);
		

		void do_move(Move*);
		void undo_move();

		inline Square king_on(Color) const;

		void init();

		struct Info {
			Piece capturedPiece;
			Square enPassant;
			Move lastMove; 
			Info *prev;
		};


	private:
		Piece board[SQUARE_NB];
		Bitboard byType[PIECE_TYPE_NB];
		Bitboard byColor[COLOR_NB];
		Color sideToMove;
		Square enPassant;
		int fiftyMoveCount;
		int gamePly;
		int CastlingRight;
		Info *st;

};

std::ostream& operator<<(std::ostream& os, const Position& pos);

inline Bitboard Position::pieces(PieceType pt) const { return byType[pt]; }

template<typename... PieceTypes>
inline Bitboard Position::pieces(PieceType pt, PieceTypes... pts) const { return pieces(pt) | pieces(pts...); }

inline Bitboard Position::pieces(Color c) const { return byColor[c]; }

template<typename... PieceTypes>
inline Bitboard Position::pieces(Color c, PieceTypes... pts) const { return pieces(c) & pieces(pts...); }

constexpr Piece Position::piece_on(Square s) const { return board[s]; }
constexpr Color Position::to_play() const { return sideToMove; }
constexpr Square Position::en_passant() const { return enPassant; }

inline Square Position::king_on(Color us) const { return lsb(pieces(us, KING)); }

inline bool Position::capture(Move *m) const { return !empty(m->to_sq()); }
inline bool Position::empty(Square s) const { return board[s] == NO_PIECE; }

} // namespace Engine

#endif
