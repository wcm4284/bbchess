#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"
#include "search.h"
#include "evaluate.h"

using namespace Engine;

int main() {

	Bitboards::init();


	std::string fen = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
	
	Position p;
	p.init();
	std::cout << p << std::endl;

	Search::iterative_deepening(p, 6);

	std::cout << Search::searched << std::endl;

	std::cout << "evaluations: " << evaluations << std::endl;

	std::cout << "extra nodes searched: " << Search::extras << std::endl;


	return 0;

}
