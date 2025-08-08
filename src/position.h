/**
 * @file position.h
 * @brief Contains position class
*/


#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED

#include "bitboard.h"

namespace Engine {

// Refers to Zobrist Hash Key
using Key = uint64_t;

/// @class Position
/// @defgroup Position
/// @brief The Position class is used to store and manipulate the game state
class Position {
    public:
        Position() = default;
        Position(const Position&) = delete;
        
        /// @struct Info
        /// @brief Stores information about game state at a certain ply
        struct Info {
            Piece capturedPiece; // at this position, what piece did we capture? 
            Square ep_sq; // at this position, what is the eq_sq
            CastlingRights cr; // at this position, what was our castling rights?
        };
    
        
        /// @brief Initializes zobrist hash keys. Must be called in order for TT to function properly
        static void init();
        
        /// @brief Sets the current position to that described by the FEN string
        /// @param fen FEN string to set position to
        void set(std::string_view);
        /// @return FEN string of the current position
        std::string fen() const;

        /// @return Zobrist hash of the current position
        Key hash() const;
    
        /// @brief Dresses up the move passed through in human readable format
        /// @param move Move to encapsulate
        /// @return String of human readable move
        std::string dress_move(Move) const;
        char piece_str(Square) const;
       
        /// @defgroup PieceGetters
        /// @ingroup Position
        /// @brief Gets the requested pieces
        /// @return A bitboard of all pieces of requested type
        /// @{
        Bitboard pieces(PieceType pt = ALL_PIECES) const;
        template<typename... PieceTypes>
        Bitboard pieces(PieceType pt, PieceTypes...) const;
        Bitboard pieces(Color) const;
        template<typename... PieceTypes>
        Bitboard pieces(Color, PieceTypes...) const;
        /// @}
        
        /// @param move pointer to move getting checked
        /// @return true if move captures a piece, false otherwise
        bool capture(Move*) const;

        /// @param sq Square to check
        /// @return true if square is empty, false otherwise
        bool empty(Square) const;
       
        /// @defgroup BoardUI
        /// @ingroup Posiiotn
        /// @brief functions used to fetch data for printing board state
        /// @{
        constexpr Piece piece_on(Square) const;
        constexpr PieceType pt_on(Square) const;
        constexpr Color to_play() const;
        constexpr Square en_passant() const;
        constexpr int ply() const;
        /// @}
       
        /// @brief Checks for pinned pieces on the board
        /// @param Color side to for pinned pieces
        /// @param PieceType finds pieces pinned to this piece type 
        /// @return pieces of Color that are pinned to PieceType
        Bitboard pinned(Color, PieceType = KING) const;
        
        /// @brief Checks for pieces attacking a king
        /// @return Bitboard containing all pieces checking a king
        Bitboard checkers() const;
   
        /// @brief checks squares that a color attacks
        /// @param Color color to compute attacks for
        /// @return Bitboard of squares attacked by color
        Bitboard attacked_squares(Color) const;

        /// @brief Checks if the requested castle is available
        /// @param CastlingRight castle option to check
        /// @return true if the player can perform the requested castle, false otherwise
        bool can_castle(CastlingRights) const;
        
        void put_piece(Piece, Square);
        void move_piece(Square, Square);
        Piece remove_piece(Square);
        
        void do_move(Move*);
        void undo_move(Move*);
        
        /// @brief Checks if a move is legal in the position
        /// @param move pointer to move to check
        /// @return true if move is legal, false otherwise
        /// @note move must be pseudo-legal
        bool legal_move(Move*) const;
            
        inline Square king_on(Color) const;

    private:
        /// @brief debugging function, checks if any game state is conflicting/impossible
        /// @note will assert false if something is wrong
        void pos_is_ok() const;

        Piece board[SQUARE_NB]; ///< Board state
        Bitboard byType[PIECE_TYPE_NB]; ///< Piece positions stored by type
        Bitboard byColor[COLOR_NB]; ///< Piece positions stored by color
        Color sideToMove; ///< Side whose turn it is
        int fiftyMoveCount; ///< Move count for fifty move rule (pawn moves and captures)
        int gamePly; ///< game ply
        Key key; ///< Zorbist hash of the position
        Info history[MAX_PLY << 1], *st; ///< Important info of previous game plies, indexed by ply
};

std::ostream& operator<<(std::ostream& os, const Position& pos);

inline Key Position::hash() const { return key; }

inline Bitboard Position::pieces(PieceType pt) const { return byType[pt]; }

template<typename... PieceTypes>
inline Bitboard Position::pieces(PieceType pt, PieceTypes... pts) const { return pieces(pt) | pieces(pts...); }

inline Bitboard Position::pieces(Color c) const { return byColor[c]; }

template<typename... PieceTypes>
inline Bitboard Position::pieces(Color c, PieceTypes... pts) const { return pieces(c) & pieces(pts...); }

constexpr Piece Position::piece_on(Square s) const { return board[s]; }
constexpr PieceType Position::pt_on(Square s) const { return type_of(piece_on(s)); }
constexpr Color Position::to_play() const { return sideToMove; }
constexpr Square Position::en_passant() const { return st->ep_sq; }
constexpr int Position::ply() const { return gamePly; }

inline Square Position::king_on(Color us) const { return lsb(pieces(us, KING)); }

inline bool Position::capture(Move *m) const { return !empty(m->to_sq()); }
inline bool Position::empty(Square s) const { return board[s] == NO_PIECE; }

} // namespace Engine

#endif
