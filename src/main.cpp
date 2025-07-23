#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"
#include "search.h"
#include "evaluate.h"
#include "moveorder.h"
#include "thread.h"
#include "uciengine.h"
#include "tt.h"

using namespace Engine;

int main(int argc, char** argv ) {

	Bitboards::init();
	Position::init();

	std::cout << sizeof(TTEntry) << std::endl;
	std::cout << sizeof(TTData) << std::endl;
	std::cout << "thread: " << sizeof(Thread) << std::endl;
	std::cout << "worker: " << sizeof(Search::Worker) << std::endl;

	UCIEngine uci {argc - 1, argv + 1};
	uci.loop();
	
	return 0;

}
