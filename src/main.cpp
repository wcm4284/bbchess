#include <chrono>

#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"
#include "search.h"
#include "evaluate.h"
#include "moveorder.h"
#include "thread.h"
#include "uciengine.h"

using namespace Engine;

int main() {

	Bitboards::init();

/*	for (int i = 1; i <= 0; ++i) {
		auto start = std::chrono::high_resolution_clock::now();
		Search::iterative_deepening(p, i);
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "Time taken " << duration.count() << " ms\n\n";
	}*/

	UCIEngine uci {};
	uci.loop();
	
	return 0;

}
