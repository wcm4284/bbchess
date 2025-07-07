#include <chrono>

#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"
#include "search.h"
#include "evaluate.h"
#include "moveorder.h"

using namespace Engine;

int main() {

	Bitboards::init();


	std::string fen = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
	
	Position p;
	p.init();
//	p.set_fen(fen);
	std::cout << p << std::endl;
	MoveOrder<LEGAL> mo(p);
	

	for (int i = 1; i <= 7; ++i) {
		auto start = std::chrono::high_resolution_clock::now();

		Search::iterative_deepening(p, i);

		auto end = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		std::cout << "Time taken " << duration.count() << " ms" << std::endl;

		std::cout << "\n\n\n";}

	return 0;

}
