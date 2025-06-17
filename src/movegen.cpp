#include "movegen.h"

namespace Engine {

template <PieceType pt>
ExtMove* generate_moves(Bitboard target, const Position& pos, ExtMove* list) {

	return list;
}
template <>
ExtMove* generate_moves<PAWN>(Bitboard target, const Position& pos, ExtMove* list) {
	return list;
}

template <>
ExtMove* generate_moves<KNIGHT>(Bitboard target, const Position& pos, ExtMove* list) {
	return list;
}

template <>
ExtMove* generate_moves<BISHOP>(Bitboard target, const Position& pos, ExtMove* list) {
	return list;
}

template <>
ExtMove* generate_moves<ROOK>(Bitboard target, const Position& pos, ExtMove* list) {
	return list;
}

template <>
ExtMove* generate_moves<QUEEN>(Bitboard target, const Position& pos, ExtMove* list) {
	return list;
}

template <>
ExtMove* generate_moves<KING>(Bitboard target, const Position& pos, ExtMove* list) {
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
