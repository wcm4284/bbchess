#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED

#include "bitboard.h"
#include "types.h"

#include <string>
#include <iostream>

namespace Engine {

// Refers to Zobrist Hash Key
using Key = uint64_t;

class Position {
	public:
		Position() = default;
		Position(const Position&) = delete;

		struct Info {
			Piece capturedPiece; // at this position, what piece did we capture? 
			Square ep_sq; // at this position, what is the eq_sq
			CastlingRights cr; // at this position, what was our castling rights?
		};
	
		
		// initializes the zobrist hashing keys
		static void init();

		void set(std::string_view); // maybe a string is fine
		std::string fen() const;
		Key hash() const;

		std::string dress_move(Move) const;
		char piece_str(Square) const;
		
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
		constexpr PieceType pt_on(Square) const;
		constexpr Color to_play() const;
		constexpr Square en_passant() const;
		constexpr int ply() const;
		
		// determines if any pieces are pinned to PieceType
		// will be primarily used for evaluation, but can
		// also probably be used for move ordering
		// @returns pieces of Color that are pinned to PieceType
		Bitboard pinned(Color, PieceType = KING) const;
		
		// returns a bitboard filled with pieces that are currently attacking a king.
		// since both kings cannot be in check at the same time, we have no need to
		// pass a color through
		Bitboard checkers() const;
	
		// returns a bitboard containing all the squares attacked by color.
		// used for castle checks and king move checks
		Bitboard attacked_squares(Color) const;


		bool can_castle(CastlingRights) const;

		void put_piece(Piece, Square);
		void move_piece(Square, Square);
		Piece remove_piece(Square);
		
		void do_move(Move*);
		void undo_move(Move*);

		bool legal_move(Move*) const;

		inline Square king_on(Color) const;

	private:
		void pos_is_ok() const;

		Piece board[SQUARE_NB];
		Bitboard byType[PIECE_TYPE_NB];
		Bitboard byColor[COLOR_NB];
		Color sideToMove;
		int fiftyMoveCount;
		int gamePly;
		Key key;
		Info history[MAX_PLY << 1], *st;
};

std::ostream& operator<<(std::ostream& os, const Position& pos);

inline Key Position::hash() const { return key; }

inline Bitboard Position::pieces(PieceType pt) const { return byType[pt]; }

template<typename... PieceTypes>
inline Bitboard Position::pieces(PieceType pt, PieceTypes... pts) const { return pieces(pt) | pieces(pts...); }

inline Bitboard Position::pieces(Color c) const { return byColor[c]; }

template<typename... PieceTypes>
inline Bitboard Position::pieces(Color c, PieceTypes... pts) const { return pieces(c) & pieces(pts...); }

constexpr Piece Position::piece_on(Square s) const { return board[s]; }
constexpr PieceType Position::pt_on(Square s) const { return type_of(piece_on(s)); }
constexpr Color Position::to_play() const { return sideToMove; }
constexpr Square Position::en_passant() const { return st->ep_sq; }
constexpr int Position::ply() const { return gamePly; }

inline Square Position::king_on(Color us) const { return lsb(pieces(us, KING)); }

inline bool Position::capture(Move *m) const { return !empty(m->to_sq()); }
inline bool Position::empty(Square s) const { return board[s] == NO_PIECE; }

} // namespace Engine

#endif
