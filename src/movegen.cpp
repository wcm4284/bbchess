#include "movegen.h"

namespace Engine {

template <PieceType pt>
ExtMove* generate_moves(Bitboard target, const Position& pos, ExtMove* list) {
	// shouldn't ever be here
	assert(false);

	return list;
}

ExtMove* generate_promotions(Square to, Square from, ExtMove* list) {
	
	for (PieceType pt : { KNIGHT, BISHOP, ROOK, QUEEN }) {
		*list++ = Move::make<PROMOTION>(to, from, pt);}

	return list;
}

template <>
ExtMove* generate_moves<PAWN>(Bitboard target, const Position& pos, ExtMove* list) {

	Color us = pos.to_play();
	Square en_pass = pos.en_passant();
	Bitboard pawns = pos.pieces(us, PAWN);
	Bitboard empty_squares = ~pos.pieces();
	Direction up = us == WHITE ? NORTH : SOUTH;

	while (pawns) {

		Square curr_sq = pop_lsb(pawns);
		Bitboard curr_bb = square_bb(curr_sq);

		
		// promotions
		Bitboard promotion_rank = us == WHITE ? Rank7 : Rank2;
		

		// captures
		Bitboard attacks = PawnAttacks[us][curr_sq] & pos.pieces(~us);			

		while (attacks) {
			// there is a special case that needs to be fixed here
			// in the case that a capture is also a promotion
			// should be resolved by doing promotions first and
			// then returning
			*list++ = Move::make<CAPTURE>(pop_lsb(attacks), curr_sq);}

		if (attacks = (PawnAttacks[us][curr_sq] & en_pass)) {
			*list++ = Move::make<ENPASSANT>(pop_lsb(attacks), curr_sq);}



		// normal moves

		
				
	}
	return list;
}

template <>
ExtMove* generate_moves<KNIGHT>(Bitboard target, const Position& pos, ExtMove* list) {
	target = 1;
	pos.pieces();
	return list;
}

template <>
ExtMove* generate_moves<BISHOP>(Bitboard target, const Position& pos, ExtMove* list) {
	target = 0;
	pos.pieces();
	return list;
}

template <>
ExtMove* generate_moves<ROOK>(Bitboard target, const Position& pos, ExtMove* list) {
	target = 0;
	pos.pieces();
	return list;
}

template <>
ExtMove* generate_moves<QUEEN>(Bitboard target, const Position& pos, ExtMove* list) {
	target = 0;
	pos.pieces();
	return list;
}

template <>
ExtMove* generate_moves<KING>(Bitboard target, const Position& pos, ExtMove* list) {
	target = 0;
	pos.pieces();
	return list;
}


ExtMove* generate_all_moves(Bitboard target, const Position& pos, ExtMove* list) {
	
		
	list = generate_moves<PAWN>(target, pos, list);
	list = generate_moves<KNIGHT>(target, pos, list);
	list = generate_moves<BISHOP>(target, pos, list);
	list = generate_moves<ROOK>(target, pos, list);
	list = generate_moves<QUEEN>(target, pos, list);
	list = generate_moves<KING>(target, pos, list);

	return list;
}

template <GenType T>
ExtMove* generate(const Position& pos, ExtMove* list) {

	Color us = pos.to_play();
	Square ksq = pos.king_on(us);
	Bitboard target		 =   T == EVASIONS ? line_bb(ksq, pos.checkers())
						   : T == NON_EVASIONS ? ~pos.pieces(us)
						   : T == CAPTURES ? pos.pieces(~us)
						   : ~pos.pieces();

	list = generate_all_moves(target, pos, list);
	return list;
}
} // namespace Engine
