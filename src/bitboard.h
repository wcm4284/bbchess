/**
 * @file bitboard.h
 * @brief Contains useful bitboard functions and constants
 *
 * Most constants and functions included in this file are
 * related to and used for move generation or evaluation.
 * Bitboards are how we store the position, and therefore
 * how we analyze and manipulate it.
*/


#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include "types.h"

namespace Engine {

namespace Bitboards {

/**
 * @brief Initializes and precomputes move tables.
 * @details This function should be called once on startup.
*/
void init();

/**
 * @brief Creates a human readable string for debugging
 * @note Useful for debugging.
 * @param b Bitboard to turn into string
 * @return Human readable string to be output to console
 */
std::string pretty(Bitboard b);

} // namespace Engine::Bitboards


/// @defgroup FileBitboards Bitboard File Constants
/// @brief Bitboard masks for the files (columns) of the board
/// @{
constexpr Bitboard FileA = 0x0101010101010101ULL;
constexpr Bitboard FileB = FileA << 1;
constexpr Bitboard FileC = FileA << 2;
constexpr Bitboard FileD = FileA << 3;
constexpr Bitboard FileE = FileA << 4;
constexpr Bitboard FileF = FileA << 5;
constexpr Bitboard FileG = FileA << 6;
constexpr Bitboard FileH = FileA << 7;
/// @}

/// @defgroup RankBitboards Bitboard Rank Constants
/// @brief Bitboard masks for the ranks (rows) of the board
/// @{
constexpr Bitboard Rank1 = 0xFFULL;
constexpr Bitboard Rank2 = Rank1 << (8 * 1);
constexpr Bitboard Rank3 = Rank1 << (8 * 2);
constexpr Bitboard Rank4 = Rank1 << (8 * 3);
constexpr Bitboard Rank5 = Rank1 << (8 * 4);
constexpr Bitboard Rank6 = Rank1 << (8 * 5);
constexpr Bitboard Rank7 = Rank1 << (8 * 6);
constexpr Bitboard Rank8 = Rank1 << (8 * 7);
/// @}

/// @defgroup Bitboardhelpers Rank/File Bitboard Accessors
/// @brief Helper functions to retrieve the file/rank bitboard
///     of a certain file, rank, or square
/// @{
constexpr Bitboard file_bb(File f) { return FileA << f; }
constexpr Bitboard file_bb(Square s) { return FileA << (file_of(s)); }
constexpr Bitboard rank_bb(Rank r) { return Rank1 << (8 * r); }
constexpr Bitboard rank_bb(Square s) { return Rank1 << (8 * rank_of(s)); }
/// @}

extern Bitboard Line[SQUARE_NB][SQUARE_NB]; ///< Precomputed array of lines between squares (excluding)
extern Bitboard Between[SQUARE_NB][SQUARE_NB]; ///< Precomputed array of lines between squares (including)
extern Bitboard PseudoAttacks[PIECE_TYPE_NB][SQUARE_NB]; ///< Precomputed array of possible moves from any square

// courtesy of chessprogramming.org/Magic_Bitboards
// attempting the "Fancy" approach
struct Magic {

    Bitboard mask;
    Bitboard* attacks;

#ifndef USE_PEXT

    Bitboard magic;
    int shift;

#endif

    unsigned index(Bitboard occ) const {

        #ifdef USE_PEXT

        return unsigned(pext(occ, mask));

        #else
        
        // TODO logic for not using pext
        assert(false);

        #endif
    }

    Bitboard attacks_bb(Bitboard occ) const { return attacks[index(occ)]; }
};

extern Magic Magics[SQUARE_NB][2];


constexpr Bitboard square_bb(Square s) {
    assert(is_ok(s));
    return (1ULL << s);
}

template <typename... Squares>
inline Bitboard square_bb(Square s, Squares... sqs) { return square_bb(sqs...) | s; }

inline Bitboard move_bb(const Move& m) {
    return square_bb(m.to_sq(), m.from_sq());}

inline Bitboard operator&(Bitboard b, Square s) { return b & square_bb(s); }
inline Bitboard operator|(Bitboard b, Square s) { return b | square_bb(s); }
inline Bitboard operator^(Bitboard b, Square s) { return b ^ square_bb(s); }
inline Bitboard& operator&=(Bitboard& b, Square s) { return b &= square_bb(s); }
inline Bitboard& operator|=(Bitboard& b, Square s) { return b |= square_bb(s); }
inline Bitboard& operator^=(Bitboard& b, Square s) { return b ^= square_bb(s); }

inline Bitboard operator&(Square s, Bitboard b) { return b & s; }
inline Bitboard operator|(Square s, Bitboard b) { return b | s; }
inline Bitboard operator^(Square s, Bitboard b) { return b ^ s; }

constexpr bool more_than_one(Bitboard b) { return b & (b - 1); }

inline Square lsb(Bitboard b) {

    #ifdef __GNUC__
        return Square(__builtin_ctzll(b));
    #else
        assert(false);
    #endif

}

inline Square pop_lsb(Bitboard& b) {
    assert(b != 0);
    Square s = lsb(b);
    b ^= s;
    return s;
}

inline Bitboard line_bb(Square s1, Square s2) { return Line[s1][s2]; }
inline Bitboard line_bb(Square s1, Bitboard b) {
    Bitboard line(0);
    while (b) 
        line |= line_bb(s1, pop_lsb(b));

    return line;
}

inline Bitboard between_bb(Square s1, Square s2) { return Between[s1][s2]; }
inline Bitboard between_bb(Square s1, Bitboard b) {
    Bitboard between(0);
    while (b)
        between |= between_bb(s1, pop_lsb(b));
    return between;
}


template <Direction d>
constexpr Bitboard shift(Bitboard b) {

    return   d == NORTH         ? b << 8
           : d == SOUTH         ? b >> 8
           : d == NORTH + NORTH ? b << 16
           : d == SOUTH + SOUTH ? b >> 16
           : d == EAST          ? (b & ~FileH) << 1
           : d == WEST          ? (b & ~FileA) >> 1
           : d == NORTH_EAST    ? (b & ~FileH) << 9
           : d == NORTH_WEST    ? (b & ~FileA) << 7
           : d == SOUTH_EAST    ? (b & ~FileH) >> 7
           : d == SOUTH_WEST    ? (b & ~FileA) >> 9
                                : 0;
    
}

// used to fill pawnattack array in bitboard.cpp
template <Color c>
constexpr Bitboard generate_pawn_attack(Bitboard b) {
    return c == WHITE ? shift<NORTH_EAST>(b) | shift<NORTH_WEST>(b)
                      : shift<SOUTH_EAST>(b) | shift<SOUTH_WEST>(b);
}

template<typename T = Square>
inline int distance(Square, Square);

template<>
inline int distance<File>(Square s1, Square s2) { return abs(file_of(s1) - file_of(s2)); }

template<>
inline int distance<Rank>(Square s1, Square s2) { return abs(rank_of(s1) - rank_of(s2)); }

template<>
inline int distance<Square>(Square s1, Square s2) { return std::max(distance<File>(s1, s2), distance<Rank>(s1, s2)); }

template <PieceType pt>
constexpr Bitboard attacks_bb(Square s, Color c = COLOR_NB) {
    assert((pt != PAWN) || (c < COLOR_NB));
    return pt == PAWN ? PseudoAttacks[c][s] : PseudoAttacks[pt][s];
}

template <PieceType pt>
constexpr Bitboard attacks_bb(Square s, Bitboard occupancy) {

    assert(pt != PAWN);
    assert(is_ok(s));

    switch (pt) {

        case BISHOP:
        case ROOK:
            return Magics[s][pt - BISHOP].attacks_bb(occupancy);
        case QUEEN:
            return Magics[s][0].attacks_bb(occupancy) | Magics[s][1].attacks_bb(occupancy);
        default:
            return PseudoAttacks[pt][s];
    }

    return 0;

}

inline Bitboard attacks_bb(Square s, Bitboard occupancy, PieceType pt) {
    assert(pt != PAWN);
    assert(is_ok(s));

    switch (pt) {
        case BISHOP:
        case ROOK:
            return Magics[s][pt - BISHOP].attacks_bb(occupancy);
        case QUEEN:
            return Magics[s][0].attacks_bb(occupancy) | Magics[s][1].attacks_bb(occupancy);
        default:
            return PseudoAttacks[pt][s];
    }

    return 0;
}
} // namespace Engine

#endif
