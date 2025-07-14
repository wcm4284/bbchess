#include <sstream>
#include <iomanip>
#include "evaluate.h"

namespace Engine {


namespace {

	Value pawnSquareValue[SQUARE_NB] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, -5, -5, 0, 5, 5,
		10, 5, 5, 10, 10, 5, 5, 10,
		0, 5, 10, 15, 15, 10, 5, 0,
		10, 10, 10, 10, 10, 10, 10, 10,
		75, 75, 75, 75, 75, 75, 75, 75,
		150, 150, 150, 150, 150, 150, 150, 150,
		0, 0, 0, 0, 0, 0, 0, 0
	};

	Value knightSquareValue[SQUARE_NB] = {
		-20, -20, -20, -20, -20, -20, -20, -20,
		-5, -5, -5, -5, -5, -5, -5, -5,
		-5, 0, 10, 5, 5, 10, 0, -5,
		-5, 10, 15, 15, 15, 15, 10, -5,
		-5, 10, 15, 15, 15, 15, 10, -5,
		-5, 10, 15, 15, 15, 15, 10, -5
		-5, 5, 10, 10, 10, 10, 5, -5,
		-5, -5, -5, -5, -5, -5, -5, -5
	};

	Value pieceContributions[SQUARE_NB];

}

Value evaluate(const Position& pos) {

	Value eval = 0;
	Bitboard occ = pos.pieces();

	for (PieceType pt : { PAWN, KNIGHT, BISHOP, ROOK, QUEEN } ) 
		eval += (popcnt(pos.pieces(WHITE, pt)) - popcnt(pos.pieces(BLACK, pt))) * PieceValues[pt];

	Bitboard w_pawns = pos.pieces(WHITE, PAWN);
	Bitboard b_pawns = pos.pieces(BLACK, PAWN);

	while (w_pawns) {
		Square sq = pop_lsb(w_pawns);
		eval += pieceContributions[sq] = pawnSquareValue[sq];
	}

	while (b_pawns) {
		Square sq = pop_lsb(b_pawns);
		eval += pieceContributions[sq] = -pawnSquareValue[flip_rank(sq)];
	}	

	Bitboard w_knights = pos.pieces(WHITE, KNIGHT);
	Bitboard b_knights = pos.pieces(BLACK, KNIGHT);

	while (w_knights) {
		Square sq = pop_lsb(w_knights);
		Value vis = popcnt(PseudoAttacks[KNIGHT][sq]) * 3;
		Value sq_val = knightSquareValue[sq];
		eval += pieceContributions[sq] = vis + sq_val;
	}

	while (b_knights) {
		Square sq = pop_lsb(b_knights);
		Value vis = popcnt(PseudoAttacks[KNIGHT][sq]) * 3;
		Value sq_val = knightSquareValue[flip_rank(sq)];
		eval += pieceContributions[sq] = -vis - sq_val;
	}

	Bitboard w_bishops = pos.pieces(WHITE, BISHOP);
	Bitboard b_bishops = pos.pieces(BLACK, BISHOP);

	while (w_bishops) {
		Square sq = pop_lsb(w_bishops);
		Value vis = popcnt(attacks_bb<BISHOP>(sq, occ)) * 3;
		eval += pieceContributions[sq] = vis;
	}

	while (b_bishops) {
		Square sq = pop_lsb(b_bishops);
		Value vis = popcnt(attacks_bb<BISHOP>(sq, occ)) * 3;
		eval += pieceContributions[sq] = -vis;
	}



	return eval;

}

std::string Eval::pretty(const Position& p) {

	// reset and populate array
	for (Value& v : pieceContributions)
		v = 0;

	Value v = evaluate(p);

	std::stringstream out;
	
	out << std::showpoint << std::showpos << std::fixed << std::setprecision(2);

	for (Rank r = RANK_8; r >= RANK_1; --r) {

		out << "+-------+-------+-------+-------+-------+-------+-------+-------+\n";

		for (File f = FILE_A; f <= FILE_H; ++f) {
			
			out << "|   " << (p.piece_str(make_square(f, r))) << "   ";

		}
		
		out << "|\n";

		for (File f = FILE_A; f <= FILE_H; ++f) {

			PieceType pt = type_of(p.piece_on(make_square(f, r)));
			out << "| "; 
			if ( pt == KING || pt == NO_PIECE_TYPE)
				out << "      ";
			else
				out << float(pieceContributions[make_square(f, r)]) / 100 << " ";
		}

	out << "|\n";
	}

	out << "+-------+-------+-------+-------+-------+-------+-------+-------+\n\n";

	out << "Evaluation: " << float(v) / 100 << "\n";

	return out.str();
}

} // namespace Engine
