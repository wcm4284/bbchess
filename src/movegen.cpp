#include "movegen.h"

#include <unordered_map>
#include <array>

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
	
	constexpr Direction up = push_dir(us);
	constexpr Bitboard rank_7 = us == WHITE ? Rank7 : Rank2;
	constexpr Bitboard rank_3 = us == WHITE ? Rank3 : Rank6;
	Bitboard pawns_on_7 = pos.pieces(us, PAWN) & rank_7;
	Bitboard pawns_off_7 = pos.pieces(us, PAWN) & ~rank_7;
	Bitboard empty_sqs = ~pos.pieces() & target;
	Bitboard attackable_sqs = pos.pieces(~us) & target;
	Square ep_sq = pos.en_passant();

	// promotions
	while (pawns_on_7) {
		Square curr_sq = pop_lsb(pawns_on_7);

		// generate attacking promos
		Bitboard moves = attacks_bb<PAWN>(curr_sq, us) & attackable_sqs;
		while (moves) {
			generate_promotions(pop_lsb(moves), curr_sq, list);}
		
		Bitboard promo;
		if ((promo = shift<up>(square_bb(curr_sq)) & empty_sqs)) {
			generate_promotions(pop_lsb(promo), curr_sq, list);}	
	}

	Bitboard b1 = shift<up>(pawns_off_7) & empty_sqs;
	Bitboard b2 = shift<up>(b1 & rank_3) & empty_sqs;

	while (b1) {
		Square sq = pop_lsb(b1);
		*list++ = Move::make<NORMAL>(sq, sq - up);}
	
	while (b2) {
		Square sq = pop_lsb(b2);
		*list++ = Move::make<NORMAL>(sq, sq - up - up);}

	// captures
	while (pawns_off_7) {
		Square curr_sq = pop_lsb(pawns_off_7);

		Bitboard atts = attacks_bb<PAWN>(curr_sq, us) & attackable_sqs;
		while (atts) {
			*list++ = Move::make<NORMAL>(pop_lsb(atts), curr_sq);}
		
		if (ep_sq != SQ_NONE) {
			Bitboard ep = attacks_bb<PAWN>(curr_sq, us) & ep_sq;
			while (ep) {
				// en passant capture
				*list++ = Move::make<ENPASSANT>(pop_lsb(ep), curr_sq);}
		}
	}

	return list;
}

template <Color us, GenType T>
ExtMove* generate_all(const Position& pos, ExtMove* list) {

	Square ksq = pos.king_on(us);
	Bitboard checkers = pos.checkers();

	// if we are in double check we can skip any non king moves
	if ((T != EVASIONS) || !more_than_one(checkers)) {

		Bitboard target		 =   T == EVASIONS ? line_bb(ksq, lsb(checkers)) | lsb(checkers)
							   : T == NON_EVASIONS ? ~pos.pieces(us)
							   : T == CAPTURES ? pos.pieces(~us)
							   : ~pos.pieces();

		#ifdef DEBUG
			std::cout << "Move target BB\n";
			std::cout << Bitboards::pretty(target) << std::endl;
		#endif


		list = generate_pawn_moves<us, T>(target, pos, list);
		list = generate_moves<us, KNIGHT>(target, pos, list);
		list = generate_moves<us, BISHOP>(target, pos, list);
		list = generate_moves<us, ROOK>(target, pos, list);
		list = generate_moves<us, QUEEN>(target, pos, list);

	}

	Bitboard king_moves = attacks_bb<KING>(ksq) & ~pos.pieces(us) & ~pos.attacked_squares(~us);


	while (king_moves) {
		Square dst = pop_lsb(king_moves);
		*list++ = Move::make<NORMAL>(dst, ksq);}

	// castling moves! 
	// gotta figure out the quickest way to check if we're castling across a check
	// for now, we will ignore that
	
	// we can't castle if we're in check
	if constexpr (T != EVASIONS) {

		std::unordered_map<CastlingRights, Square> m = {
			{WHITE_OO, SQ_G1},
			{WHITE_OOO, SQ_C1},
			{BLACK_OO, SQ_G8},
			{BLACK_OOO, SQ_C8}
		};

		constexpr std::array<CastlingRights, 2> crs = (us == WHITE)
						? std::array<CastlingRights, 2>{WHITE_OO, WHITE_OOO}
						: std::array<CastlingRights, 2>{BLACK_OO, BLACK_OOO};

		for (CastlingRights cr : crs) {
			if (pos.can_castle(cr)) {
				*list++ = Move::make<CASTLING>(m[cr], ksq);}}
	}



	
	return list;
}

template <GenType T>
ExtMove* generate(const Position& pos, ExtMove* list) {

	Color us = pos.to_play();


	return us == WHITE ? generate_all<WHITE, T>(pos, list)
					   : generate_all<BLACK, T>(pos, list);
}

template ExtMove* generate<NON_EVASIONS>(const Position&, ExtMove*);
template ExtMove* generate<CAPTURES>(const Position&, ExtMove*);
template ExtMove* generate<EVASIONS>(const Position&, ExtMove*);
template ExtMove* generate<QUIET>(const Position&, ExtMove*);

template <>
ExtMove* generate<LEGAL>(const Position& pos, ExtMove* list) {
	Color us = pos.to_play();
	Bitboard pinned = pos.pinned(us);
	Bitboard checkers = pos.checkers();

	ExtMove* cur = list;

	list = checkers ? generate<EVASIONS>(pos, list) : generate<NON_EVASIONS>(pos, list);

	while (cur != list) {
		if ( ((pinned & cur->from_sq()) || (cur->type() == ENPASSANT)) && !pos.legal_move(cur)) {
			
			*cur = *(--list);}
		else {
			++cur;}}


	return list;
}

} // namespace Engine
