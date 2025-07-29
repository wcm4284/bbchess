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

    UCIEngine uci {argc - 1, argv + 1};
    uci.loop();
    
    return 0;

}
