/**
 * @file search.h
 * @brief Contains classes and structs used for performing a search for the best move
*/

#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "moveorder.h"

#include <string>
#include <vector> 

namespace Engine {

class ThreadPool;
class TranspositionTable;

template <typename T>
class MoveQueue;

namespace Search {

/**
 * @struct SharedState
 * @brief Stores search state shared between threads 
*/
struct SharedState {

    SharedState(TranspositionTable& table) : tt(table) {}

    TranspositionTable& tt;
};

/**
 * @struct SearchLimits
 * @brief Stores information limited to how the search will be performed
*/
struct SearchLimits {

    SearchLimits() : perft(0), depth(0) {}

    Depth perft; ///< Depth for perft search
    Depth depth; ///< Depth for bestmove search
};

/**
 * @struct PerftMove
 * @brief Container for perft root moves
*/
struct PerftMove {

    PerftMove(Move m) : move(m), node_count(0) {}
    
    Move move; ///< Root move to make
    uint64_t node_count; ///< Nodes found after searching this move
};

/**
 * @class Worker
 * @brief Contains search logic and data
*/
class Worker {
    public:

        Worker() = default;
        Worker(SharedState& ss) : tt(ss.tt) {}
    
        /**
         * @brief Start the bestmove search 
        */
        void start_searching();

    private:
        void iterative_deepening();
        /**
         * @brief Start the perft search
        */
        void perft(PerftMoves&);
        uint64_t perft(Position&, Depth); 
        Value search(Position&, int, int, Depth, int);
        Value qsearch(Position&, int, int, int);
    
        uint64_t nodes; ///< Number of nodes searched

        SearchLimits limits;
        Position     rootPos;
        Move pv_table[MAX_PLY][MAX_PLY]; ///< Table used to store the best line found

        TranspositionTable& tt;

    friend class Engine::ThreadPool;
};
    
} // namespace Search

} // namespace Engine

#endif
