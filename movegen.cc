#include "attack_table.h"
#include "bit.h"
#include "game.h"
#include "move.h"

const u64 white_promotion = 0xFF;
const u64 white_start = 0xFF000000000000;
const u64 black_promotion = 0xFF00000000000000;
const u64 black_start = 0xFF00;

// pseudo legal move generation function
// input => empty moves list
// output => moves list populated with all pseudo-legal moves in a given position
static inline void generate_moves(moves* move_list) {

    // define source and target square
    int source_sq, target_sq;

    // define piece
    int piece;

    // define current piece bitboard and attack
    u64 bb, attacks;

    // generate pawn moves
    piece = side == white ? P : p;
    bb = bitboards[piece];

    // iterate over all pawns
    while (bb) {
        
        // initialize source square
        source_sq = get_lsb_index(bb);

        // init target square
        target_sq = side == white ? source_sq - 8 : source_sq + 8;

        // generate quiet pawn moves
        if (target_sq >= a8 && target_sq <= h1 && !get_bit(occupancies[both], target_sq)) {
            
            // check promotion
            if (get_bit(((side == white) ? white_promotion : black_promotion), target_sq)) {
                
                // add promotion move into move list
                add_move(move_list, encode_move(source_sq, target_sq, piece, ((side == white) ? Q : q), 0, 0, 0, 0));
                add_move(move_list, encode_move(source_sq, target_sq, piece, ((side == white) ? R : r), 0, 0, 0, 0));
                add_move(move_list, encode_move(source_sq, target_sq, piece, ((side == white) ? B : b), 0, 0, 0, 0));
                add_move(move_list, encode_move(source_sq, target_sq, piece, ((side == white) ? N : n), 0, 0, 0, 0));

            } else {

                // one square move
                add_move(move_list, encode_move(source_sq, target_sq, piece, 0, 0, 0, 0, 0));

                // two square move
                target_sq = side == white ? target_sq - 8 : target_sq + 8;
                
                if (target_sq >= 0 && get_bit(((side == white) ? white_start : black_start), source_sq) && !get_bit(occupancies[both], target_sq))
                    add_move(move_list, encode_move(source_sq, target_sq, piece, 0, 0, 1, 0, 0));

            }
        }

        // check pawn captures
        attacks = pawn_attacks[side][source_sq];

        // en passant
        if (en_passant != no_sq && get_bit(attacks, en_passant)) {
            // update target sq
            target_sq = en_passant;

            // add move to list
            add_move(move_list, encode_move(source_sq, target_sq, piece, 0, 1, 0, 1, 0));

        }

        attacks &= occupancies[((side == white) ? black : white)];

        while (attacks) {
            
            // init target square
            target_sq = get_lsb_index(attacks);

            // check if capture promotion
            if (get_bit(side == white ? white_promotion : black_promotion, target_sq)) {
                // add moves
                add_move(move_list, encode_move(source_sq, target_sq, piece, ((side == white) ? Q : q), 1, 0, 0, 0));
                add_move(move_list, encode_move(source_sq, target_sq, piece, ((side == white) ? R : r), 1, 0, 0, 0));
                add_move(move_list, encode_move(source_sq, target_sq, piece, ((side == white) ? N : n), 1, 0, 0, 0));
                add_move(move_list, encode_move(source_sq, target_sq, piece, ((side == white) ? B : b), 1, 0, 0, 0));
            } else {
                // regular capture
                add_move(move_list, encode_move(source_sq, target_sq, piece, 0, 1, 0, 0, 0));
            }

            // remove attack
            pop_bit(attacks, target_sq);
            
        }

        // pop pawn
        pop_bit(bb, source_sq);

    }


    // generate knight moves
    piece = side == white ? N : n;
    bb = bitboards[piece];

    // iterate over all knights
    while (bb) {
        
        // initialize source square
        source_sq = get_lsb_index(bb);

        // init attacks
        attacks = knight_attacks[source_sq];

        // remove moves onto same color pieces
        attacks &= ~occupancies[side];

        // iterate over moves
        while (attacks) {

            // init target square
            target_sq = get_lsb_index(attacks);

            // add move to list
            add_move(move_list, encode_move(source_sq, target_sq, piece, 0, (get_bit(occupancies[((side == white) ? black : white)], target_sq) ? 1 : 0), 0, 0, 0));          

            // remove attack
            pop_bit(attacks, target_sq);

        }
        
        // pop knight
        pop_bit(bb, source_sq);

    }
    

    // generate bishop moves
    piece = side == white ? B : b;
    bb = bitboards[piece];
    
    // iterate over all bishops
    while (bb) {
        
        // initialize source square
        source_sq = get_lsb_index(bb);

        // init attacks
        attacks = get_bishop_attacks(source_sq, occupancies[both]);

        // remove moves onto same color pieces
        attacks &= ~occupancies[side];

        // iterate over moves
        while (attacks) {

            // init target square
            target_sq = get_lsb_index(attacks);

            // add move to list
            add_move(move_list, encode_move(source_sq, target_sq, piece, 0, (get_bit(occupancies[((side == white) ? black : white)], target_sq) ? 1 : 0), 0, 0, 0));

            // remove attack
            pop_bit(attacks, target_sq);

        }
        
        // pop bishop
        pop_bit(bb, source_sq);

    }

    // generate rook moves
    piece = side == white ? R : r;
    bb = bitboards[piece];

    // iterate over all rooks
    while (bb) {
        
        // initialize source square
        source_sq = get_lsb_index(bb);

        // init attacks
        attacks = get_rook_attacks(source_sq, occupancies[both]);

        // remove moves onto same color pieces
        attacks &= ~occupancies[side];

        // iterate over moves
        while (attacks) {

            // init target square
            target_sq = get_lsb_index(attacks);

            // add move to list
            add_move(move_list, encode_move(source_sq, target_sq, piece, 0, (get_bit(occupancies[((side == white) ? black : white)], target_sq) ? 1 : 0), 0, 0, 0));

            // remove attack
            pop_bit(attacks, target_sq);

        }
        
        // pop rook
        pop_bit(bb, source_sq);

    }

    // generate queen moves
    piece = side == white ? Q : q;
    bb = bitboards[piece];

    // iterate over all queens
    while (bb) {

        // initialize source square
        source_sq = get_lsb_index(bb);

        // init attacks
        attacks = get_queen_attacks(source_sq, occupancies[both]);

        // init attacks & remove moves onto same color pieces
        attacks &= ~occupancies[side];

        // iterate over moves
        while (attacks) {

            // init target square
            target_sq = get_lsb_index(attacks);

            // add move to list
            add_move(move_list, encode_move(source_sq, target_sq, piece, 0, (get_bit(occupancies[((side == white) ? black : white)], target_sq) ? 1 : 0), 0, 0, 0));

            // remove attack
            pop_bit(attacks, target_sq);

        }
        
        // pop queen
        pop_bit(bb, source_sq);

    }

    // generate king moves
    piece = side == white ? K : k;
    bb = bitboards[piece];

    if (bb) {

        // source square update
        source_sq = get_lsb_index(bb);

        // init attacks
        attacks = king_attacks[source_sq];

        // remove moves onto same color pieces
        attacks &= ~occupancies[side];

        // iterate over moves
        while (attacks) {

            // init target square
            target_sq = get_lsb_index(attacks);

            // add move to list
            if (!is_square_attacked(target_sq, ((side == white) ? black : white)))
                // add move
                add_move(move_list, encode_move(source_sq, target_sq, piece, 0, (get_bit(occupancies[((side == white) ? black : white)], target_sq) ? 1 : 0), 0, 0, 0));

            // remove attack
            pop_bit(attacks, target_sq);

        }

        // white castle moves
        if (side == white) {

            // kingside castle available
            if (castle & wk) {

                // check if f1 and g1 squares are empty
                if (!get_bit(occupancies[both], f1) && !get_bit(occupancies[both], g1)) {

                    // check if king, f1, or g1 is under attack
                    if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black) && !is_square_attacked(g1, black)) {

                        // set target move
                        target_sq = g1;

                        // add castle move
                        add_move(move_list, encode_move(source_sq, target_sq, piece, 0, 0, 0, 0, 1));

                    }
                }
            }

            // queen side castle available
            if (castle & wq) {

                // check if b1, c1 and d1 squares are empty
                if (!get_bit(occupancies[both], b1) && !get_bit(occupancies[both], c1) && !get_bit(occupancies[both], d1)) {

                    // check if c1 or d1 squares are under attack
                    if (!is_square_attacked(c1, black) && !is_square_attacked(d1, black) && !is_square_attacked(e1, black)) {

                        // set target move
                        target_sq = c1;

                        // add castle move
                        add_move(move_list, encode_move(source_sq, target_sq, piece, 0, 0, 0, 0, 1));
                    }
                }
            }
        }

        // black castle moves 
        if (side == black) {

            // kingside castle available
            if (castle & bk) {

                // check if f1 and g1 squares are empty
                if (!get_bit(occupancies[both], f8) && !get_bit(occupancies[both], g8)) {

                    // check if king, f1, or g1 is under attack
                    if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white) && !is_square_attacked(g8, white)) {
                        
                        // set target move
                        target_sq = g8;

                        // add castle move
                        add_move(move_list, encode_move(source_sq, target_sq, piece, 0, 0, 0, 0, 1));

                    }
                }
            }
            
            // queen side castle available
            if (castle & bq) {

                // check if b8, c8 and d8 squares are empty
                if (!get_bit(occupancies[both], b8) && !get_bit(occupancies[both], c8) && !get_bit(occupancies[both], d8)) {

                    // check if c8 or d8 or e8squares are under attack
                    if (!is_square_attacked(c8, white) && !is_square_attacked(d8, white) && !is_square_attacked(e8, white)) {

                        // set target move
                        target_sq = c8;

                        // add castle move
                        add_move(move_list, encode_move(source_sq, target_sq, piece, 0, 0, 0, 0, 1));
                    }
                }
            }
        }
    }

    return;
        
}

