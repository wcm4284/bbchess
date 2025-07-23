#ifndef EVALUATE_H_INCLUDED
#define EVALUATE_H_INCLUDED


#include "types.h"
#include "position.h"

namespace Engine {

Value evaluate(const Position&);

namespace Eval {

std::string pretty(const Position&);

}

} // namespace Engine

#endif
