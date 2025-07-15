#pragma once

#include "thread.h"

namespace Engine {

class Engine {

	public:
		Engine(int nThreads);


	private:
		
		ThreadPool threads;
		

};

}
