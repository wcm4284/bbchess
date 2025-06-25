#pragma once
#ifndef MOVEGEN_H_INCLUDED
#define MOVEGEN_H_INCLUDED

#include "types.h"
#include "position.h"

namespace Engine {

enum GenType {
	EVASIONS,
	NON_EVASIONS,
	QUIET,
	CAPTURES,
	LEGAL
};

struct ExtMove : public Move {

	void operator=(Move m) { data = m.raw(); }
	operator float() const = delete;

	int value;

};

template <GenType T>
ExtMove* generate(const Position&, ExtMove*);

template <GenType T>
class MoveList {
	
	// for debugging purposes, print all moves in the list
	friend std::ostream& operator<<(std::ostream& os, const MoveList& l) {
		
		const ExtMove* curr = l.list;
		int idx = 0;
		while (curr != l.last) {
			if (l.verbose) {
				Bitboard b = move_bb(*curr);
				os << Bitboards::pretty(b) << std::endl;}

			os << "idx " << idx++ << " "; 
			os << *curr++ << std::endl;}



		return os;
	}

	public:
		MoveList() = default;
		MoveList(const Position& pos) : 
			last(generate<T>(pos, list)), verbose(false) {}

		MoveList(const Position& pos, bool v) :
			last(generate<T>(pos, list)), verbose(v) {}

		Move& operator[](int i) { return list[i]; }

	private:
		ExtMove list[MAX_MOVES], *last;
		bool verbose;

};

}

#endif
