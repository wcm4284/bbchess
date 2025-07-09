#pragma once

#include "types.h"
#include "position.h"

namespace Engine {

Value evaluate(const Position&);

namespace Eval {

std::string pretty(const Position&);

}

} // namespace Engine
