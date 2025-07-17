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
		void go(int = MAX_PLY);
		inline void set(Search::SearchLimits limits) { threads.set(limits); }

		void stop();


	private:
		
		ThreadPool threads;
		Position pos;
		

};

}
