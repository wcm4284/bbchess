#ifndef MOVEGEN_H_INCLUDED
#define MOVEGEN_H_INCLUDED

#include "types.h"
#include "position.h"

namespace Engine {

enum GenType {
    EVASIONS,
    NON_EVASIONS,
    QUIET,
    CAPTURES,
    LEGAL
};

struct ExtMove : public Move {

    void operator=(Move m) { data = m.raw(); }
    bool operator<(const ExtMove& e) const { return value < e.value; }
    bool operator>(const ExtMove& e) const { return value > e.value; }
    operator float() const = delete;

    uint16_t value;

};

template <GenType T>
ExtMove* generate(const Position&, ExtMove*);

template <GenType T>
class MoveList {
    
    // for debugging purposes, print all moves in the list
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
        MoveList(const Position& pos) : idx(0), 
            last(generate<T>(pos, list)) {}

        Move& operator[](int i) { return list[i]; }


        ExtMove* begin() { return list; }
        ExtMove* end() { return last; }
        const ExtMove* begin() const { return list; }
        const ExtMove* end() const { return last; }
        Move* next() { return &list[idx] == last ? nullptr : &list[idx++]; }
        size_t size() const { return last - list; }

    private:
        int idx;

    protected:
        ExtMove list[MAX_MOVES], *last;

};

}

#endif
