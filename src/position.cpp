#include "position.h"

#include <iostream>
#include <string_view>

namespace Engine {

namespace {

	constexpr std::string_view PieceToChar(" PNBRQK  pnbrqk");

	const std::string PrintSquare[65] = {
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
}
	
std::ostream& operator<<(std::ostream& os, const Position& pos) {

	os << "+---+---+---+---+---+---+---+---+\n";

	for (Rank r = RANK_8; r >= RANK_1; --r) {
		
		for (File f = FILE_A; f <= FILE_H; ++f) 
			os << "| " << PieceToChar[pos.piece_on(make_square(f, r))] << " ";

		os << "| " << std::to_string(r + 1) << "\n+---+---+---+---+---+---+---+---+\n";

	}

	os << "  a   b   c   d   e   f   g   h\n\n";

	os << "FEN String: " << pos.fen();

	return os << ((pos.to_play() == WHITE) ? "White" : "Black") << " to move\n";

}

	
void Position::init() {

	sideToMove = WHITE;
	byType[PAWN]   = 0xFF00000000FF00;
	byType[BISHOP] = 0x2400000000000024;
	byType[KNIGHT] = 0x4200000000000042;
	byType[ROOK]   = square_bb(SQ_A8) | square_bb(SQ_A1) | square_bb(SQ_H8) | square_bb(SQ_H1);
	byType[QUEEN]  = square_bb(SQ_D1) | square_bb(SQ_D8);
	byType[KING]   = square_bb(SQ_E1) | square_bb(SQ_E1);

	byColor[WHITE] = 0xFFFF;
	byColor[BLACK] = byColor[WHITE] << 48;

	board[SQ_H8] = board[SQ_A8] = B_ROOK;
	board[SQ_G8] = board[SQ_B8] = B_KNIGHT;
	board[SQ_F8] = board[SQ_C8] = B_BISHOP;
	board[SQ_D8] = B_QUEEN;
	board[SQ_E8] = B_KING;

	board[SQ_A1] = board[SQ_H1] = W_ROOK;
	board[SQ_B1] = board[SQ_G1] = W_KNIGHT;
	board[SQ_C1] = board[SQ_F1] = W_BISHOP;
	board[SQ_D1] = W_QUEEN;
	board[SQ_E1] = W_KING;

	for (File f = FILE_A; f <= FILE_H; ++f) 

		board[make_square(f, RANK_2)] = W_PAWN;

	for (File f = FILE_A; f <= FILE_H; ++f)
		
		board[make_square(f, RANK_7)] = B_PAWN;

	for (File f = FILE_A; f <= FILE_H; ++f) {

		for (Rank r = RANK_3; r <= RANK_6; ++r) 

			board[make_square(f, r)] = NO_PIECE;

	}

	gamePly = 0;
	enPassant = SQ_NONE;
	fiftyMoveCount = 0;
	CastlingRight = ANY_CASTLING;

}

std::string Position::fen() const {

	std::string s;

	for (Rank r = RANK_8; r >= RANK_1; --r) {

		int spaces = 0;

		for (File f = FILE_A; f <= FILE_H; ++f) {

			Piece p = piece_on(make_square(f, r));

			if (p == NO_PIECE) {

				++spaces;

			} else {

				if ( spaces ) {

					s      += std::to_string(spaces);
					spaces = 0;

				}

				s += PieceToChar[p];

			}

		}

		if (spaces)
			s += std::to_string(spaces);
		
		if (r != RANK_1)
			s += "/";

	}
	s += (sideToMove == WHITE) ? " w" : " b";
	s += " ";
	if (!CastlingRight) {
		s += "-";
	} else {
		if (CastlingRight & WHITE_OO)
			s += "K";
		if (CastlingRight & WHITE_OOO)
			s += "Q";
		if (CastlingRight & BLACK_OO)
			s += "k";
		if (CastlingRight & BLACK_OOO)
			s += "q";
	}
	s += " " + PrintSquare[enPassant];
	s += " " + std::to_string(fiftyMoveCount);
	s += " " + std::to_string((gamePly / 2) + 1);

	return s += "\n";
}



} // namespace Engine
