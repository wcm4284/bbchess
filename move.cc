#include <map>
#include <stdio.h>
#include <stdlib.h>
#include "attack_table.h"
#include "hash.h"
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
// int make_move(int move, int move_flag) {

//     // quiet moves
//     if (move_flag == all_moves) {

//         // preserve state
//         copy_board();

//         // parse move
//         int source_sq = get_move_source(move);
//         int target_sq = get_move_target(move);
//         int piece = get_move_piece(move);
//         int promoted = get_move_promoted(move);



//         // move piece
//         pop_bit(bitboards[piece], source_sq);
//         set_bit(bitboards[piece], target_sq);

//         // update hash
//         hkey ^= piece_keys[piece][source_sq]; // remove piece from source in hash
//         hkey ^= piece_keys[piece][target_sq]; // remove piece from target in hash

//         // handle capture
//         // if (get_move_capture(move)) {

//         //     // pick a range for piece check
//         //     int index = side == white ? p : P;
//         //     int endIndex = side == white ? k : K;

//         //     // find correct piece index
//         //     for (;index <= endIndex; index++) {

//         //         if (get_bit(bitboards[index], target_sq)) {
                    
//         //             bitboards[index] ^= (1ull << target_sq);

//         //             hkey ^= piece_keys[index][target_sq];

//         //             break;
//         //         }
//         //     }
//         // }

//         if (get_move_capture(move))
//         {
//             // pick up bitboard piece index ranges depending on side
//             int start_piece, end_piece;
            
//             // white to move
//             if (side == white)
//             {
//                 start_piece = p;
//                 end_piece = k;
//             }
            
//             // black to move
//             else
//             {
//                 start_piece = P;
//                 end_piece = K;
//             }
            
//             // loop over bitboards opposite to the current side to move
//             for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++)
//             {
//                 // if there's a piece on the target square
//                 if (get_bit(bitboards[bb_piece], target_sq))
//                 {
//                     // remove it from corresponding bitboard
//                     pop_bit(bitboards[bb_piece], target_sq);
                    
//                     // remove the piece from hash key
//                     hkey ^= piece_keys[bb_piece][target_sq];
//                     break;
//                 }
//             }
//         }

//         // if (promoted) {

//         //     // remove pawn
//         //     pop_bit(bitboards[piece], target_sq);

//         //     // add promoted piece
//         //     set_bit(bitboards[promoted], target_sq);

//         //     hkey ^= piece_keys[piece][target_sq];
//         //     hkey ^= piece_keys[promoted][target_sq];

//         // }

//         if (promoted)
//         {
//             // erase the pawn from the target square
//             //pop_bit(bitboards[(side == white) ? P : p], target_square);
            
            
//             // white to move
//             if (side == white)
//             {
//                 // erase the pawn from the target square
//                 pop_bit(bitboards[P], target_sq);
                
//                 // remove pawn from hash key
//                 hkey ^= piece_keys[P][target_sq];
//             }
            
//             // black to move
//             else
//             {
//                 // erase the pawn from the target square
//                 pop_bit(bitboards[p], target_sq);
                
//                 // remove pawn from hash key
//                 hkey ^= piece_keys[p][target_sq];
//             }
            
//             // set up promoted piece on chess board
//             set_bit(bitboards[promoted], target_sq);
            
//             // add promoted piece into the hash key
//             hkey ^= piece_keys[promoted][target_sq];
//         }

//         // if (get_move_enpassant(move)) {

//         //     // remove captured pawn
//         //     (side == white) ? pop_bit(bitboards[p], target_sq + 8) :
//         //                       pop_bit(bitboards[P], target_sq - 8);

//         //     hkey ^= (side == white) ? piece_keys[p][target_sq + 8] :
//         //                               piece_keys[P][target_sq - 8];

//         // }

//         if (get_move_enpassant(move))
//         {
//             // erase the pawn depending on side to move
//             (side == white) ? pop_bit(bitboards[p], target_sq + 8) :
//                               pop_bit(bitboards[P], target_sq - 8);
                              
//             // white to move
//             if (side == white)
//             {
//                 // remove captured pawn
//                 pop_bit(bitboards[p], target_sq + 8);
                
//                 // remove pawn from hash key
//                 hkey ^= piece_keys[p][target_sq + 8];
//             }
            
//             // black to move
//             else
//             {
//                 // remove captured pawn
//                 pop_bit(bitboards[P], target_sq - 8);
                
//                 // remove pawn from hash key
//                 hkey ^= piece_keys[P][target_sq - 8];
//             }
//         }

//         if (get_move_castle(move)) {

//             switch (target_sq) {
//                 // case g1:
//                 //     // white king castles king side
//                 //     pop_bit(bitboards[R], h1);
//                 //     set_bit(bitboards[R], f1);
//                 //     hkey ^= piece_keys[R][h1];
//                 //     hkey ^= piece_keys[R][f1];
//                 //     break;
//                 // case c1:
//                 //     // white king castles queen side
//                 //     pop_bit(bitboards[R], a1);
//                 //     set_bit(bitboards[R], d1);
//                 //     hkey ^= piece_keys[R][a1];
//                 //     hkey ^= piece_keys[R][d1];
//                 //     break;
//                 // case c8:
//                 //     // black king castles queen side
//                 //     pop_bit(bitboards[r], a8);
//                 //     set_bit(bitboards[r], d8);
//                 //     hkey ^= piece_keys[r][a8];
//                 //     hkey ^= piece_keys[r][d8];
//                 //     break;
//                 // case g8:
//                 //     // black king castles king side
//                 //     pop_bit(bitboards[r], h8);
//                 //     set_bit(bitboards[r], f8);
//                 //     hkey ^= piece_keys[r][h8];
//                 //     hkey ^= piece_keys[r][f8];
//                 //     break;

//                 case (g1):
//                     // move H rook
//                     pop_bit(bitboards[R], h1);
//                     set_bit(bitboards[R], f1);
                    
//                     // hash rook
//                     hkey ^= piece_keys[R][h1];  // remove rook from h1 from hash key
//                     hkey ^= piece_keys[R][f1];  // put rook on f1 into a hash key
//                     break;
                
//                 // white castles queen side
//                 case (c1):
//                     // move A rook
//                     pop_bit(bitboards[R], a1);
//                     set_bit(bitboards[R], d1);
                    
//                     // hash rook
//                     hkey ^= piece_keys[R][a1];  // remove rook from a1 from hash key
//                     hkey ^= piece_keys[R][d1];  // put rook on d1 into a hash key
//                     break;
                
//                 // black castles king side
//                 case (g8):
//                     // move H rook
//                     pop_bit(bitboards[r], h8);
//                     set_bit(bitboards[r], f8);
                    
//                     // hash rook
//                     hkey ^= piece_keys[r][h8];  // remove rook from h8 from hash key
//                     hkey ^= piece_keys[r][f8];  // put rook on f8 into a hash key
//                     break;
                
//                 // black castles queen side
//                 case (c8):
//                     // move A rook
//                     pop_bit(bitboards[r], a8);
//                     set_bit(bitboards[r], d8);
                    
//                     // hash rook
//                     hkey ^= piece_keys[r][a8];  // remove rook from a8 from hash key
//                     hkey ^= piece_keys[r][d8];  // put rook on d8 into a hash key
//                     break;
//             }
//         }

//         // get rid of hash of old en passant square
//         // if (en_passant != no_sq) hkey ^= en_passant_keys[en_passant];

//         // // reset en passant square
//         // en_passant = (get_move_double(move)) ? (target_sq + ((side == white) ? 8 : -8)) : no_sq;

//         // // update hash based on enpassant square
//         // if (en_passant != no_sq) hkey ^= en_passant_keys[en_passant];

//         if (en_passant != no_sq) hkey ^= en_passant_keys[en_passant];
        
//         // reset enpassant square
//         en_passant = no_sq;
        
//         // handle double pawn push
//         if (get_move_double(move))
//         {
//             // set enpassant aquare depending on side to move
//             //(side == white) ? (enpassant = target_square + 8) :
//             //                  (enpassant = target_square - 8);
                              
//             // white to move
//             if (side == white)
//             {
//                 // set enpassant square
//                 en_passant = target_sq + 8;
                
//                 // hash enpassant
//                 hkey ^= en_passant_keys[target_sq + 8];
//             }
            
//             // black to move
//             else
//             {
//                 // set enpassant square
//                 en_passant = target_sq - 8;
                
//                 // hash enpassant
//                 hkey ^= en_passant_keys[target_sq - 8];
//             }
//         }

//         hkey ^= castling_keys[castle];

//         // update castling rights
//         castle &= castling_rights[source_sq];
//         castle &= castling_rights[target_sq];

//         hkey ^= castling_keys[castle];

//         // update occupancies
//         occupancies[white] = bitboards[P] | bitboards[N] | bitboards[B] | bitboards[R] | bitboards[Q] | bitboards[K];
//         occupancies[black] = bitboards[p] | bitboards[n] | bitboards[b] | bitboards[r] | bitboards[q] | bitboards[k];
//         occupancies[both] = occupancies[white] | occupancies[black];

//         // get white or black king
//         int king = (side == white) ? K : k;

//         hkey ^= side_key;

//         // change side
//         side ^= 1;

//         // check if king is in check
//         if (is_square_attacked(get_lsb_index(bitboards[king]), side)) {

//             // turn back move
//             restore_board();

//             return 0;
//         }

//         if (hkey != hash()) printf(":(\n");
//         else printf("good\n");

//         return 1;

//     } else {
//         // make sure move is capture
//         if (get_move_capture(move)) {
//             make_move(move, all_moves);
//         } else return 0; // only make move if its a capture

//     }

//     return 0;
// }

int make_move(int move, int move_flag)
{
    // quiet moves
    if (move_flag == all_moves)
    {
        // preserve board state
        copy_board();
        
        // parse move
        int source_square = get_move_source(move);
        int target_square = get_move_target(move);
        int piece = get_move_piece(move);
        int promoted_piece = get_move_promoted(move);
        int capture = get_move_capture(move);
        int double_push = get_move_double(move);
        int enpass = get_move_enpassant(move);
        int castling = get_move_castle(move);
        
        // move piece
        pop_bit(bitboards[piece], source_square);
        set_bit(bitboards[piece], target_square);
        
        // hash piece
        hkey ^= piece_keys[piece][source_square]; // remove piece from source square in hash key
        hkey ^= piece_keys[piece][target_square]; // set piece to the target square in hash key
        
        // handling capture moves
        if (capture)
        {
            // pick up bitboard piece index ranges depending on side
            int start_piece, end_piece;
            
            // white to move
            if (side == white)
            {
                start_piece = p;
                end_piece = k;
            }
            
            // black to move
            else
            {
                start_piece = P;
                end_piece = K;
            }
            
            // loop over bitboards opposite to the current side to move
            for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++)
            {
                // if there's a piece on the target square
                if (get_bit(bitboards[bb_piece], target_square))
                {
                    // remove it from corresponding bitboard
                    pop_bit(bitboards[bb_piece], target_square);
                    
                    // remove the piece from hash key
                    hkey ^= piece_keys[bb_piece][target_square];
                    break;
                }
            }
        }
        
        // handle pawn promotions
        if (promoted_piece)
        {
            // erase the pawn from the target square
            //pop_bit(bitboards[(side == white) ? P : p], target_square);
            
            
            // white to move
            if (side == white)
            {
                // erase the pawn from the target square
                pop_bit(bitboards[P], target_square);
                
                // remove pawn from hash key
                hkey ^= piece_keys[P][target_square];
            }
            
            // black to move
            else
            {
                // erase the pawn from the target square
                pop_bit(bitboards[p], target_square);
                
                // remove pawn from hash key
                hkey ^= piece_keys[p][target_square];
            }
            
            // set up promoted piece on chess board
            set_bit(bitboards[promoted_piece], target_square);
            
            // add promoted piece into the hash key
            hkey ^= piece_keys[promoted_piece][target_square];
        }
        
        // handle enpassant captures
        if (enpass)
        {
            // erase the pawn depending on side to move
            (side == white) ? pop_bit(bitboards[p], target_square + 8) :
                              pop_bit(bitboards[P], target_square - 8);
                              
            // white to move
            if (side == white)
            {
                // remove captured pawn
                pop_bit(bitboards[p], target_square + 8);
                
                // remove pawn from hash key
                hkey ^= piece_keys[p][target_square + 8];
            }
            
            // black to move
            else
            {
                // remove captured pawn
                pop_bit(bitboards[P], target_square - 8);
                
                // remove pawn from hash key
                hkey ^= piece_keys[P][target_square - 8];
            }
        }
        
        // hash enpassant if available (remove enpassant square from hash key )
        if (en_passant != no_sq) hkey ^= en_passant_keys[en_passant];
        
        // reset enpassant square
        en_passant = no_sq;
        
        // handle double pawn push
        if (double_push)
        {
            // set enpassant aquare depending on side to move
            //(side == white) ? (enpassant = target_square + 8) :
            //                  (enpassant = target_square - 8);
                              
            // white to move
            if (side == white)
            {
                // set enpassant square
                en_passant = target_square + 8;
                
                // hash enpassant
                hkey ^= en_passant_keys[target_square + 8];
            }
            
            // black to move
            else
            {
                // set enpassant square
                en_passant = target_square - 8;
                
                // hash enpassant
                hkey ^= en_passant_keys[target_square - 8];
            }
        }
        
        // handle castling moves
        if (castling)
        {
            // switch target square
            switch (target_square)
            {
                // white castles king side
                case (g1):
                    // move H rook
                    pop_bit(bitboards[R], h1);
                    set_bit(bitboards[R], f1);
                    
                    // hash rook
                    hkey ^= piece_keys[R][h1];  // remove rook from h1 from hash key
                    hkey ^= piece_keys[R][f1];  // put rook on f1 into a hash key
                    break;
                
                // white castles queen side
                case (c1):
                    // move A rook
                    pop_bit(bitboards[R], a1);
                    set_bit(bitboards[R], d1);
                    
                    // hash rook
                    hkey ^= piece_keys[R][a1];  // remove rook from a1 from hash key
                    hkey ^= piece_keys[R][d1];  // put rook on d1 into a hash key
                    break;
                
                // black castles king side
                case (g8):
                    // move H rook
                    pop_bit(bitboards[r], h8);
                    set_bit(bitboards[r], f8);
                    
                    // hash rook
                    hkey ^= piece_keys[r][h8];  // remove rook from h8 from hash key
                    hkey ^= piece_keys[r][f8];  // put rook on f8 into a hash key
                    break;
                
                // black castles queen side
                case (c8):
                    // move A rook
                    pop_bit(bitboards[r], a8);
                    set_bit(bitboards[r], d8);
                    
                    // hash rook
                    hkey ^= piece_keys[r][a8];  // remove rook from a8 from hash key
                    hkey ^= piece_keys[r][d8];  // put rook on d8 into a hash key
                    break;
            }
        }
        
        // hash castling
        hkey ^= castling_keys[castle];
        
        // update castling rights
        castle &= castling_rights[source_square];
        castle &= castling_rights[target_square];

        // hash castling
        hkey ^= castling_keys[castle];
        
        occupancies[white] = bitboards[P] | bitboards[N] | bitboards[B] | bitboards[R] | bitboards[Q] | bitboards[K];
        occupancies[black] = bitboards[p] | bitboards[n] | bitboards[b] | bitboards[r] | bitboards[q] | bitboards[k];
        occupancies[both] = occupancies[white] | occupancies[black];
        
        // change side
        side ^= 1;
        
        // hash side
        hkey ^= side_key;
        
        //
        // ====== debug hash key incremental update ======= //
        //
        
        // build hash key for the updated position (after move is made) from scratch
        u64 hash_from_scratch = hash();
        
        // in case if hash key built from scratch doesn't match
        // the one that was incrementally updated we interrupt execution
        if (hkey != hash())
        {
            printf("\n\nbADBADBAD\n");
            // printf("move: "); print_move(move);
            // print_board();
            // printf("hash key should be: %llx\n", hash_from_scratch);
            // getchar();
        }
        
        
        // make sure that king has not been exposed into a check
        if (is_square_attacked((side == white) ? get_lsb_index(bitboards[k]) : get_lsb_index(bitboards[K]), side))
        {
            // take move back
            restore_board();
            
            // return illegal move
            return 0;
        }
        
        //
        else
            // return legal move
            return 1;
            
    }
    
    // capture moves
    else
    {
        // make sure move is the capture
        if (get_move_capture(move))
            make_move(move, all_moves);
        
        // otherwise the move is not a capture
        else
            // don't make it
            return 0;
    }

    return 1;
}

