#include <cstring>
#include "debug.h"
#include "magic.h"
#include "game.h"
#include "hash.h"
#include "search.h"
#include "uci.h"

int main() {

    // init attack tables for bishop and rook
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);

    // init hash keys for transposition table
    init_hash_keys();

    int debug = 1;

    if (debug) {
        parse_FEN(tricky_position);
       
        print_board();

        search_position(5);

    } else {
        uci_loop();
    }

    return 0;

}


