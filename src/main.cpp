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

	UCIEngine uci {};
	uci.loop();
	
	return 0;

}
