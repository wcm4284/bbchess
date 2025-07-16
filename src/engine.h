#pragma once

#include "thread.h"
#include "position.h"

namespace Engine {

class Engine {

	public:
		Engine(int nThreads);
		
		// this function isn't very versatile, and will likely lead to a seg fault
		// if the fen string isn't formatted correctly
		inline void set_position(std::string fen) { pos.set(fen); }
		void perft(int = MAX_PLY);
		void go(int = MAX_PLY);


	private:
		
		ThreadPool threads;
		Position pos;
		

};

}
