#include "evaluate.h"

namespace Engine {


namespace {

	Value pawnSquareValue[SQUARE_NB] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		5, 5, 5, 0, 0, 5, 10, 10,
		10, 5, 5, 15, 15, 5, 5, 10,
		0, 0, 0, 0, 0, 0, 0, 0,
		10, 10, 10, 10, 10, 10, 10, 10,
		75, 75, 75, 75, 75, 75, 75, 75,
		150, 150, 150, 150, 150, 150, 150, 150,
		0, 0, 0, 0, 0, 0, 0, 0
	};

}

Value static_eval(const Position& pos) {
	Value eval = 0;

	for (Color c : {WHITE, BLACK} ) {
		for (PieceType pt : { PAWN, KNIGHT, BISHOP, ROOK, QUEEN } ) {
			eval += popcnt(pos.pieces(c, pt)) * PieceValues[make_piece(pt, c)];}}

	return eval;

}

} // namespace Engine
