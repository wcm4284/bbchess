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


struct SharedState {

    SharedState(TranspositionTable& table) : tt(table) {}

    TranspositionTable& tt;

};

struct SearchLimits {

    SearchLimits() : perft(0), depth(0) {}

    // depth for perft search
    Depth perft;
    // depth for regular search
    Depth depth;
};

struct PerftMove {

    PerftMove(Move m) : move(m), node_count(0) {}
    
    Move move; 
    uint64_t node_count;

};



class Worker {


    public:

        Worker() = default;
        Worker(SharedState& ss) : tt(ss.tt) {}

        void start_searching();



    private:
        
        void iterative_deepening();
        void perft(PerftMoves&);
        uint64_t perft(Position&, Depth); 
        Value search(Position&, int, int, Depth, int);
        Value qsearch(Position&, int, int, int);
    

        uint64_t nodes;
        

        SearchLimits limits;
        Position     rootPos;
        Move pv_table[MAX_PLY][MAX_PLY];

        TranspositionTable& tt;


    friend class Engine::ThreadPool;

};
    
    
} // namespace Search

} // namespace Engine

#endif
