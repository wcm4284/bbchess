#include <map>
#include <stdio.h>
#include <stdlib.h>
#include "attack_table.h"
#include "move.h"

std::map<int, char> promoted_pieces = {
    {Q, 'Q'},
    {R, 'R'},
    {B, 'B'},
    {N, 'N'},
    {q, 'q'},
    {r, 'r'},
    {b, 'b'},
    {n, 'n'}
};

// add move to move list 
void add_move(moves* move_list, int move) {

    // store move
    move_list->moves[move_list->count] = move;

    // increment count
    move_list->count++;

    return;
}

// print move
void print_move(int move) {
    printf("%s%s%c", square_to_coord[get_move_source(move)],
                       square_to_coord[get_move_target(move)],
                       get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ');
}

// print move list
void print_move_list(moves* move_list) {
    
    printf("\n     move  piece  capture  double  enpassant  castling\n\n");

    // loop over moves
    for (int i = 0; i < move_list->count; i++) {

        // init move
        int move = move_list->moves[i];

        #ifdef _WIN64
            // print move
            printf("    %s%s%c  %5c  %7d  %6d  %9d  %8d\n", square_to_coord[get_move_source(move)],
                                                            square_to_coord[get_move_target(move)],
                                                            get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ',
                                                            ASCII_pieces[get_move_piece(move)],
                                                            get_move_capture(move) ? 1 : 0,
                                                            get_move_double(move) ? 1 : 0,
                                                            get_move_enpassant(move) ? 1 : 0,
                                                            get_move_castle(move) ? 1 : 0);
        #else
            // print move
            printf("    %s%s%c  %5s  %7d  %6d  %9d  %8d\n", square_to_coord[get_move_source(move)],
                                                            square_to_coord[get_move_target(move)],
                                                            get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ',
                                                            unicode_pieces[get_move_piece(move)],
                                                            get_move_capture(move) ? 1 : 0,
                                                            get_move_double(move) ? 1 : 0,
                                                            get_move_enpassant(move) ? 1 : 0,
                                                            get_move_castle(move) ? 1 : 0);
        #endif

    }

    // print total moves 
    printf("\nTotal number of moves: %d\n", move_list->count);
}

// castling rights constants
const int castling_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

// make move
int make_move(int move, int move_flag) {

    // quiet moves
    if (move_flag == all_moves) {

        // preserve state
        copy_board();

        // parse move
        int source_sq = get_move_source(move);
        int target_sq = get_move_target(move);
        int piece = get_move_piece(move);
        int promoted = get_move_promoted(move);



        // move piece
        pop_bit(bitboards[piece], source_sq);
        set_bit(bitboards[piece], target_sq);

        // handle capture
        if (get_move_capture(move)) {

            // pick a range for piece check
            int index = side == white ? p : P;
            int endIndex = side == white ? k : K;

            // find correct piece index
            for (;index <= endIndex; index++) {
                if (get_bit(bitboards[index], target_sq)) {
                    bitboards[index] ^= (1ull << target_sq);
                    break;
                }
            }
        }

        if (get_move_promoted(move)) {

            // remove pawn
            pop_bit(bitboards[piece], target_sq);

            // add promoted piece
            set_bit(bitboards[promoted], target_sq);

        }

        if (get_move_enpassant(move)) {

            // remove captured pawn
            (side == white) ? pop_bit(bitboards[p], target_sq + 8) :
                              pop_bit(bitboards[P], target_sq - 8);

        }

        if (get_move_castle(move)) {

            switch (target_sq) {
                case g1:
                    // white king castles king side
                    pop_bit(bitboards[R], h1);
                    set_bit(bitboards[R], f1);
                    break;
                case c1:
                    // white king castles queen side
                    pop_bit(bitboards[R], a1);
                    set_bit(bitboards[R], d1);
                    break;
                case c8:
                    // black king castles queen side
                    pop_bit(bitboards[r], a8);
                    set_bit(bitboards[r], d8);
                    break;
                case g8:
                    // black king castles king side
                    pop_bit(bitboards[r], h8);
                    set_bit(bitboards[r], f8);
                    break;
            }
        }

        // reset en passant square
        en_passant = (get_move_double(move)) ? (target_sq + ((side == white) ? 8 : -8)) : no_sq;

        // update castling rights
        castle &= castling_rights[source_sq];
        castle &= castling_rights[target_sq];

        // update occupancies
        occupancies[white] = bitboards[P] | bitboards[N] | bitboards[B] | bitboards[R] | bitboards[Q] | bitboards[K];
        occupancies[black] = bitboards[p] | bitboards[n] | bitboards[b] | bitboards[r] | bitboards[q] | bitboards[k];
        occupancies[both] = occupancies[white] | occupancies[black];

        // get white or black king
        int king = (side == white) ? K : k;

        // change side
        side ^= 1;

        // check if king is in check
        if (is_square_attacked(get_lsb_index(bitboards[king]), side)) {

            // turn back move
            restore_board();

            return 0;
        }

        return 1;

    } else {
        // make sure move is capture
        if (get_move_capture(move)) {
            make_move(move, all_moves);
        } else return 0; // only make move if its a capture

    }

    return 0;
}

