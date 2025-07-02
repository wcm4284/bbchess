#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "movegen.h"
#include "search.h"
#include "evaluate.h"

using namespace Engine;

int main() {

	Bitboards::init();

	std::string fen = "rnbqkbnr/pppppppp/1P6/8/8/8/PPPPPP1P/RNBQKBNR w KQkq - 0 1";
	
	Position p;
	p.init();
	std::cout << p << std::endl;

	std::cout << evaluate(p) << std::endl;

	std::cout << p.ply() << std::endl;

	Search::iterative_deepening(p, 5);

	return 0;

}
