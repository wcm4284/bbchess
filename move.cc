#include <map>
#include <stdio.h>
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
static inline void add_move(moves* move_list, int move) {

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