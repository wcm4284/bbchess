#include <algorithm>
#include <functional>

#include "moveorder.h"
#include "position.h"

namespace Engine {

namespace {
void rate_moves(const Position& p, ExtMove *f, ExtMove *l) {

    while ( f != l ) {
        f->value = p.pt_on(f->to_sq()) * 100 + PIECE_TYPE_NB - p.pt_on(f->from_sq());
        ++f;
    }

    return;
}
} // namespace
    
void sort(const Position& p, ExtMove *f, ExtMove *l) {
    rate_moves(p, f, l);
    std::stable_sort(f, l, std::greater<>());

    return;
}
} // namespace Engine
