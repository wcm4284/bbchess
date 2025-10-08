/**
 * @file moveorder.h
 * @brief Contains classes used for sorting moves
 *
*/

#ifndef MOVEORDER_H_INCLUDED
#define MOVEORDER_H_INCLUDED


#include <mutex>

#include "movegen.h"

namespace Engine {

class Position;

/// @brief Used to sort a move list based on position
/// @param p Position moves belong to
/// @param f First move in the list
/// @param l Last move in the list
void sort(const Position& p, ExtMove* f, ExtMove* l);


/// @class MoveOrder
/// @brief Sorts generated moves upon construction
/// @tparam T Type of moves to generate (LEGAL, CAPTURES, etc.)
template <GenType T>
class MoveOrder : public MoveList<T> {

    public:
        MoveOrder(const Position& pos) : 
            MoveList<T>(pos) 
            
            { sort(pos, this->list, this->last); }

};


/// @class MoveQueue
/// @brief Exists as a wrapper around MoveOrder, serves to make next() thread-safe
/// @tparam T Type of move storage to use -> MoveOrder or MoveList
template <typename T>
class MoveQueue {

    public:
        MoveQueue(const Position& pos) : moves(pos) {}
        
        /// @brief Gives next move
        /// @return Pointer to a move
        inline Move* next() {
            std::lock_guard<std::mutex> lock(mutex);
            return moves.next();
        }

        size_t size() const { return moves.size(); }


    private:
        
        T moves; ///< Move storage
        std::mutex mutex; ///< Mutex for thread-safe access

};

using PerftMoves = MoveQueue<MoveList<LEGAL>>;
using RootMoves = MoveQueue<MoveOrder<LEGAL>>;

} // namespace Engine
    
#endif
