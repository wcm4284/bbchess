#include "evaluate.h"

namespace Engine {


namespace {

	Value pawnSquareValue[SQUARE_NB] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		5, 5, 0, -5, -5, 0, 10, 10,
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
		eval += pawnSquareValue[sq];}

	while (b_pawns) {
		Square sq = pop_lsb(b_pawns);
		eval -= pawnSquareValue[flip_square(sq)];}

	Bitboard w_knights = pos.pieces(WHITE, KNIGHT);
	Bitboard b_knights = pos.pieces(BLACK, KNIGHT);

	while (w_knights) {
		Square sq = pop_lsb(w_knights);
		eval += popcnt(PseudoAttacks[KNIGHT][sq]) * 3;
		eval += knightSquareValue[sq];}

	while (b_knights) {
		Square sq = flip_square(pop_lsb(b_knights));
		eval -= popcnt(PseudoAttacks[KNIGHT][sq]) * 3;
		eval -= knightSquareValue[sq];}

	Bitboard w_bishops = pos.pieces(WHITE, BISHOP);
	Bitboard b_bishops = pos.pieces(BLACK, BISHOP);

	while (w_bishops) {
		Square sq = pop_lsb(w_bishops);
		eval += popcnt(attacks_bb<BISHOP>(sq, occ)) * 3;}

	while (b_bishops) {
		Square sq = flip_square(pop_lsb(b_bishops));
		eval -= popcnt(attacks_bb<BISHOP>(sq, occ)) * 3;}


	return eval;

}

} // namespace Engine
