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

class ExtMove : public Move {

	void operator=(Move m) { data = m.raw(); }
	operator float() const = delete;

	int value;

};

template <GenType T>
ExtMove* generate(const Position&, ExtMove*);

template <GenType T>
class MoveList {

	MoveList(const Position& pos) : 
		last(generate<T>(pos, list)) {}

	private:
		ExtMove list[MAX_MOVES], *last;

};

}

#endif
