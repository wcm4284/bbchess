/**
 * @file evaluate.h
 * @brief Contains methods to evaluate the score of the position
*/

#ifndef EVALUATE_H_INCLUDED
#define EVALUATE_H_INCLUDED

#include "position.h"

namespace Engine {

/// @brief Evaluate the position
/// @param p Position to evaluate
/// @return Value of the position -- positive favors white, negative favors black
Value evaluate(const Position& p);

namespace Eval {

/// @brief Create a human friendly representation of the evaluation
/// @param p Position to evaluate
/// @return String representation of the position eval
std::string pretty(const Position& p);

}

} // namespace Engine

#endif
