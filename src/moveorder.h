#pragma once

#include "movegen.h"

namespace Engine {

class Position;

template <GenType T>
class MoveOrder {

	public:
		MoveOrder(const Position& p) : ptr(new MoveList<T>(p)), first(ptr->begin()), last(ptr->end()) {}

		~MoveOrder() { delete ptr; }

		inline Move* next_move() { return first == last ? nullptr : first++; }
		size_t size() const { return ptr->size(); }


	private:
		MoveList<T>* ptr;
		ExtMove *first, *last;

};

} // namespace Engine
		
