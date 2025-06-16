#include "movegen.h"

namespace Engine {

template <GenType T>
ExtMove* generate(const Position& pos, ExtMove* list) {

	// Bitboard checkers = pos.checkers(); TODO write checkers
	Color us = pos.to_play();
//	Bitboard target		 =   T == EVASIONS ? line_bb(ksq, checkers)
//						   : T == NON_EVASIONS ? ~pos.pieces(us)
//						   : T == CAPTURES ? pos.pieces(~us)
//						   : ~pos.pieces();
	
	return nullptr;
}
} // namespace Engine
