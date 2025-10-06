/**
 * @file movegen.h
 * @brief Contains classes that wrap move generation
 *
*/

#ifndef MOVEGEN_H_INCLUDED
#define MOVEGEN_H_INCLUDED

#include "position.h"

namespace Engine {

/// @enum GenType
/// @brief Move generation types
enum GenType {
    EVASIONS, ///< Moves that block / stop check
    NON_EVASIONS, ///< Any move (pseudolegal)
    QUIET, ///< Non-captures (pseudolegal)
    CAPTURES, ///< Captures (pseudolegal)
    LEGAL ///< Legal moves
};

/// @struct ExtMove
/// @brief Extention of the move class that contains a value to sort by -- useful for move ordering
struct ExtMove : public Move {
    
    /// @brief Sets the raw data if this move equal to what is passed in
    void operator=(Move m) { data = m.raw(); }
    
    /// @brief Override compare operator for sorting
    bool operator<(const ExtMove& e) const { return value < e.value; }
    /// @brief Override compare operator for sorting
    bool operator>(const ExtMove& e) const { return value > e.value; }
    operator float() const = delete;

    uint16_t value; ///< Value used for move ordering

};

/// @brief Creates a list of moves available in the position
/// @tparam T The type of moves to generate (LEGAL, CAPTURES, etc.)
/// @param position The position to generate moves for
/// @param list Pointer to the beginning of the list that will store moves
/// @return Pointer to the end of the list of generated moves
template <GenType T>
ExtMove* generate(const Position& position, ExtMove* list);

/// @class MoveList
/// @brief Contains all available moves of a certain type in a position
/// @tparam T Type of move to generate (LEGAL, CAPTURES, etc.)
template <GenType T>
class MoveList {
    
    /// @brief Print all moves in the list
    /// @note Useful for debugging
    friend std::ostream& operator<<(std::ostream& os, const MoveList& l) {

        const ExtMove* curr = l.list;

        std::cout << "Printing ML of type ";
        if constexpr(T == EVASIONS)
            os << "EVASIONS";
        else if constexpr(T == CAPTURES)
            os << "CAPTURES";
        else if constexpr(T == NON_EVASIONS)
            os << "NON_EVASIONS";
        else if constexpr(T == QUIET)
            os << "QUIET";
        else
            os << "LEGAL";
        os << std::endl;

        int c = 1;
        
        while (curr != l.last) 
            os << c++ << ": " << *curr++;

        return os;
    }

    public:
        MoveList() = default;

        /// @brief Constructor generates move list
        /// @param pos Position to generate moves for 
        MoveList(const Position& pos) : idx(0), 
            last(generate<T>(pos, list)) {}
        
        /// @brief Access an element of the list
        /// @param i index of element
        /// @return Move accessed
        Move& operator[](int i) { return list[i]; }

        ExtMove* begin() { return list; }
        ExtMove* end() { return last; }
        const ExtMove* begin() const { return list; }
        const ExtMove* end() const { return last; }
        /// @brief Access next move in the move list
        /// @return Next move in the list, nullptr if list is exhausted
        Move* next() { return &list[idx] == last ? nullptr : &list[idx++]; }
        size_t size() const { return last - list; }

    private:
        int idx; ///< Used to determine which item of the list to return next

    protected:
        ExtMove list[MAX_MOVES], *last; ///< List used to store moves

};

}

#endif
