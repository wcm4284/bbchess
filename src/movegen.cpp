#include "movegen.h"

namespace Engine {

template <Color us, PieceType pt>
ExtMove* generate_moves(Bitboard target, const Position& pos, ExtMove* list) {

	assert (pt != KING);
	assert (pt != PAWN);

	Bitboard bb = pos.pieces(us, pt);
	Bitboard occ = pos.pieces();

	while (bb) {

		Square curr_sq = pop_lsb(bb);

		Bitboard moves = attacks_bb<pt>(curr_sq, occ);
		
		// i'm going to operate under the assumption that
		// this line removes the possibility of us moving
		// onto our own piece
		moves &= target;

		while (moves) {

			Square dst = pop_lsb(moves);
			*list++ = Move::make<NORMAL>(dst, curr_sq);}

	}

	return list;
}

ExtMove* generate_promotions(Square to, Square from, ExtMove* list) {
	
	for (PieceType pt : { KNIGHT, BISHOP, ROOK, QUEEN }) {
		*list++ = Move::make<PROMOTION>(to, from, pt);}

	return list;
}

template <Color us, GenType type>
ExtMove* generate_pawn_moves(Bitboard target, const Position& pos, ExtMove* list) {

	Square en_pass = pos.en_passant();
	Bitboard pawns = pos.pieces(us, PAWN);
	Bitboard empty_squares = ~pos.pieces();
	constexpr Direction up = us == WHITE ? NORTH : SOUTH;
	constexpr Bitboard promotion_rank = us == WHITE ? Rank7 : Rank2;

	while (pawns) {

		Square curr_sq = pop_lsb(pawns);
		Bitboard curr_bb = square_bb(curr_sq);
		

		// captures
		Bitboard attacks = attacks_bb<PAWN>(curr_sq, us) & pos.pieces(~us) & target;			

		while (attacks) {
			// special case where a capture is also a promotion move
			if (promotion_rank & curr_sq) {
				list = generate_promotions(pop_lsb(attacks), curr_sq, list);} 
			else {
				*list++ = Move::make<NORMAL>(pop_lsb(attacks), curr_sq);}

		}

		if ((attacks = (attacks_bb<PAWN>(curr_sq, us) & en_pass))) {
			*list++ = Move::make<ENPASSANT>(pop_lsb(attacks), curr_sq);}



		// normal moves
		// Bitboard b1 = shift<up>(curr_sq); 
		
				
	}
	return list;
}

template <Color us, GenType T>
ExtMove* generate_all_moves(const Position& pos, ExtMove* list) {

	Square ksq = pos.king_on(us);


	if ((T != EVASIONS) || !more_than_one(pos.checkers())) {

		Bitboard target		 =   T == EVASIONS ? line_bb(ksq, lsb(pos.checkers()))
							   : T == NON_EVASIONS ? ~pos.pieces(us)
							   : T == CAPTURES ? pos.pieces(~us)
							   : ~pos.pieces();


		list = generate_pawn_moves<us, T>(target, pos, list);
		list = generate_moves<us, KNIGHT>(target, pos, list);
		list = generate_moves<us, BISHOP>(target, pos, list);
		list = generate_moves<us, ROOK>(target, pos, list);
		list = generate_moves<us, QUEEN>(target, pos, list);

	}

	return list;
}

template <GenType T>
ExtMove* generate(const Position& pos, ExtMove* list) {

	Color us = pos.to_play();


	return us == WHITE ? generate_all_moves<WHITE, T>(pos, list)
					   : generate_all_moves<BLACK, T>(pos, list);
}
} // namespace Engine
