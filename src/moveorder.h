#ifndef MOVEORDER_H_INCLUDED
#define MOVEORDER_H_INCLUDED


#include <mutex>

#include "movegen.h"

namespace Engine {

class Position;

void sort(const Position&, ExtMove*, ExtMove*);


// all this does is sort the MoveList
template <GenType T>
class MoveOrder : public MoveList<T> {

    public:
        MoveOrder(const Position& pos) : 
            MoveList<T>(pos) 
            
            { sort(pos, this->list, this->last); }

};


// this is a wrapper for the MoveOrder class
// functionally, it only exists to lock the next_move function
template <typename T>
class MoveQueue {

    public:
        MoveQueue(const Position& pos) : moves(pos) {}
        inline Move* next() {
            std::lock_guard<std::mutex> lock(mutex);
            return moves.next();
        }

        size_t size() const { return moves.size(); }


    private:
        
        T moves;
        std::mutex mutex;

};

using PerftMoves = MoveQueue<MoveList<LEGAL>>;
using RootMoves = MoveQueue<MoveOrder<LEGAL>>;

} // namespace Engine
    
#endif
