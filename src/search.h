#pragma once

#include "position.h"

#include <string>

namespace Engine {


int perft(std::string, int);

Move recursive_search(Position&, int); 

Move iterative_deepening(Position&, int);

} // namespace Engine
