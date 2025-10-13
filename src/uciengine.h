/**
 * @file uci.h
 * @brief Contains UCIEngine Class
*/

#ifndef UCI_H_INCLUDED
#define UCI_H_INCLUDED

#include <sstream>

#include "engine.h"

namespace Engine {

/**
 * @class UCIEngine
 * @brief Class that interacts with the user's input and directs the engine per uci protocol
*/
class UCIEngine {
    public:
        UCIEngine() = default;
        UCIEngine(int, char**); 
            
        /**
         * @brief Loop where the engine reads user input and determines action
         * @note The UCIEngine only leaves this loop when we exit/shutdown the engine
        */
        void loop();

    private:
        void position(std::istringstream&);
        void go(std::istringstream&);
        Search::SearchLimits parse_limits(std::istringstream&);
        void perft();
        void start_searching();
        
        Engine engine;

};

} // namespace Engine

#endif
