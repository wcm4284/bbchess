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

	byType[ALL_PIECES] = byColor[WHITE] | byColor[BLACK];

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

Bitboard Position::pinned(Color us, PieceType pinnedTo) const {

	Bitboard pinned(0);
	Color them          = ~us;
	Bitboard pinnedToBB = pieces(us, pinnedTo);
	
	while (pinnedToBB) {

		// I call this king because it is the important square here, 
		// not necessarily because it is referring to the king, although
		// it can refer to the king
		Square king = pop_lsb(pinnedToBB);
		
		for (PieceType pt : { BISHOP, ROOK, QUEEN }) {
			
			Bitboard pinners = pieces(them, pt);

			while (pinners) {
				
				// to figure out if a piece is pinned, we draw a line from where we are to the king
				// if it hits the king, we need to find out if there are any pieces between us
				// and the king. if so, add to pinned

				// I thought about doing it without one of the two bitboards, but I decided that:
				// 
				// I need line, because PseudoAttacks could hit the king, and in the next step
				// accidentally decide a piece is pinned. Maybe this could be fixed with a tweak
				// to the next step but that is a brain exercise for another day
				// TODO: read above

				// I need PseudoAttacks because line could generate a hit, but be a false positive
				// in the case that a rook is on the same diagonal as the king, or vice versa.
				// can't use attack_bb because that would stop at the pinned piece
				Square pinner      = pop_lsb(pinners);
				Bitboard line      = line_bb(pinner, king);
				Bitboard attacks   = PseudoAttacks[pt][pinner];

				if (attacks & king) {

					// we hit the king! now use line
					// since a piece belonging to them could also be blocking,
					// we probably need to use the whole occupancy to check.
					// then use & with what we have left to see if pieces are white.
					Bitboard occ = pieces();
					if (more_than_one(line &= occ)) {
						return Bitboard(0);}
					
					if (!more_than_one(line &= pieces(us))) {
						pinned |= pop_lsb(line);}
				}
			}
		}
	}
	return pinned;
}

// returns all pieces currently checking a king.
// should ideally only be able to include pieces
// of the opposite color to who is moving, since
// that is how the rules of the game shape out
// however, until I test and confirm, we will
// act as though that is not the case
Bitboard Position::checkers() const {
	
	Bitboard checkers(0);
	Bitboard occ = pieces();


	for (Color color : { WHITE, BLACK }) {
		
		Square ksq = king_on(color);
		Color them = ~color;

		for (PieceType pt : { BISHOP, ROOK, QUEEN }) {
			
			Bitboard attacks = attacks_bb(ksq, occ, pt);
			
			// I think this is okay, it shouldn't be possible
			// for two rooks/bishops/queens to be checking
			// the king at the same time. TODO: confirm
			if (attacks &= pieces(them, pt)) {
				checkers |= attacks;}

		}

		// knight
		Bitboard knights = pieces(them, KNIGHT);
		if (knights &= PseudoAttacks[KNIGHT][ksq]) {
			checkers |= knights;}

		// pawn
		Bitboard pawns = pieces(them, PAWN);
		if (pawns &= PawnAttacks[color][ksq]) {
			checkers |= pawns;}


		
	}
	


	return checkers;

}
	
} // namespace Engine
