#include "evaluate.h"

namespace Engine {

Value static_eval(const Position& pos) {
	Value eval = 0;

	for (Color c : {WHITE, BLACK} ) {
		for (PieceType pt : { PAWN, KNIGHT, BISHOP, ROOK, QUEEN } ) {
			eval += popcnt(pos.pieces(c, pt)) * PieceValues[make_piece(pt, c)];}}

	return eval;

}

} // namespace Engine
