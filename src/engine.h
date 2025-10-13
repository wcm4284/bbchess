/**
 * @file engine.h
 * @brief Contains Engine class
*/

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "thread.h"
#include "tt.h"

namespace Engine {

/**
 * @class Engine
 * @brief Stores, controls threads and any related search parameters
*/
class Engine {

    public:
        /**
         * @brief Engine constructor
         * @param nThreads Number of threads to create (default = 1)
        */
        Engine(int nThreads);
        
        /**
         * @brief Set the position stored by the engine
         * @param fen Fen string to set position to
        */
        inline void set_position(std::string fen) { pos.set(fen); }

        /// @brief Begin bestmove search
        void go();

        /// @brief Begin perft search
        void perft();

        /**
         * @brief Set the limits associated with any search we perform
         * @param limits Limits to set
        */
        inline void set(Search::SearchLimits limits) { threads.set(limits, pos); }
        
        /// @brief Stop any search we are performing
        void stop();

        /// @brief Reset the values stored inside the threads' workers
        void reset();

    private:
        TranspositionTable tt; ///< Transposition Table used by all threads
        ThreadPool threads; ///< Stores and controls all threads
        Position pos; ///< Position to search from
};

}

#endif
