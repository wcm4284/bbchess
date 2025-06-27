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
		
		while (curr != l.last) {
			os << *curr++;}

		return os;
	}

	public:
		MoveList() = default;
		MoveList(const Position& pos) : 
			last(generate<T>(pos, list)) {}

		Move& operator[](int i) { return list[i]; }

		ExtMove* begin() { return list; }
		ExtMove* end() { return last; }
		const ExtMove* begin() const { return list; }
		const ExtMove* end() const { return last; }

	private:
		ExtMove list[MAX_MOVES], *last;

};

}

#endif
