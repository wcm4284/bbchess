#include <cstring>
#include "attack_table.h"
#include "evaluation.h"
#include "move.h"
#include "movegen.h"
#include "time.h"
#include "uci.h"

// attacker vs captured piece value lookup table
// mvv_lva [attacker] [victim]
static int mvv_lva[12][12] = {
    105, 205, 305, 405, 505, 605,   105, 205, 305, 405, 505, 605,
    104, 204, 304, 404, 504, 604,   104, 204, 304, 404, 504, 604,
    103, 203, 303, 403, 503, 603,   103, 203, 303, 403, 503, 603,
    102, 202, 302, 402, 502, 602,   102, 202, 302, 402, 502, 602,
    101, 201, 301, 401, 501, 601,   101, 201, 301, 401, 501, 601,
    100, 200, 300, 400, 500, 600,   100, 200, 300, 400, 500, 600,

    105, 205, 305, 405, 505, 605,   105, 205, 305, 405, 505, 605,
    104, 204, 304, 404, 504, 604,   104, 204, 304, 404, 504, 604,
    103, 203, 303, 403, 503, 603,   103, 203, 303, 403, 503, 603,
    102, 202, 302, 402, 502, 602,   102, 202, 302, 402, 502, 602,
    101, 201, 301, 401, 501, 601,   101, 201, 301, 401, 501, 601,
    100, 200, 300, 400, 500, 600,   100, 200, 300, 400, 500, 600
};

// killer moves [id] [ply]
int killer_moves[2][64];

// history moves [piece] [square]
int history_moves[12][64];

// PV length [ply]
int pv_length[64];

// PV table [ply] [ply]
int pv_table[64][64];

// init search and score PV flags
bool search_pv, score_pv;

int ply = 0;

int nodes = 0;

// max int
const int INFINITY = 0x7FFFFFFF;


static inline void enable_pv_scoring(moves* move_list) { 
    // disable search pv
    search_pv = false;

    // loop over moves
    for (int i = 0; i < move_list->count; i++) {

        // make sure pv move is here
        if (pv_table[0][ply] == move_list->moves[i]) {

            // enable pv scoring
            score_pv = true;

            // enable search pv
            search_pv = true;
        }
    }
}

static inline int move_score(int move) {

    if (score_pv) { 
        // check pv move
        if (pv_table[0][ply] == move) {

            // disable score pv
            score_pv = false;

            // return high score
            return 1000000;
        }
    }

    // init score
    int score = 0;

    // check if capture move
    if (get_move_capture(move)) {

        // update score for capture
        score = 10000;

        // init victim piece
        int victim;

        int index = side == white ? p : P;
        int endIndex = side == white ? k : K;

        // extract target sq
        int target_sq = get_move_target(move);

        // find correct piece index
        for (;index <= endIndex; index++) {
            if (get_bit(bitboards[index], target_sq)) {
                score += mvv_lva[get_move_piece(move)][index];
                break;
            }
        }
        
        // add to score if protected by a pawn

    }
    // score quiet move 
    else {
        

        // score first killer move
        if (killer_moves[0][ply] == move) { return 9000; }
        
        // score second killer move
        if (killer_moves[1][ply] ==  move) { return 8000; }

        // score history move
        return history_moves[get_move_piece(move)][get_move_target(move)];

    }

    return score;
}


void sort_moves(moves* move_list) {
    // generate move scores
    int scores[move_list->count];

    for (int i = 0; i < move_list->count; i++) {
        scores[i] = move_score(move_list->moves[i]);
    }

    // loop over current move
    for (int current_move = 0; current_move < move_list->count; current_move++) {
        // loop over next move
        for (int next_move = current_move + 1; next_move < move_list->count; next_move++) {
            // compare scores
            if (scores[current_move] < scores[next_move]) {
                // switch moves
                int temp = move_list->moves[current_move];
                move_list->moves[current_move] = move_list->moves[next_move];
                move_list->moves[next_move] = temp;
                temp = scores[current_move];
                scores[current_move] = scores[next_move];
                scores[next_move] = temp;
            }
        }
    }
}


static inline int quiescence(int alpha, int beta) {

    if ((nodes & 2047) == 0)
    {
        communicate();
    }

    nodes++;

    // evaluate position
    int eval = evaluate();

    if (eval >= beta) {
        return beta;
    }

    // found better move
    if (eval > alpha)
        alpha = eval;
    
    // init moves list
    moves* move_list = new moves;

    generate_moves(move_list);

    sort_moves(move_list);

    // loop over moves
    for (int i = 0; i < move_list->count; i++) {

        // copy board state
        copy_board();

        if (make_move(move_list->moves[i], only_captures)) {

            // increase ply
            ply++;

            int score = -quiescence(-beta, -alpha);

            // decrement ply
            ply--;
            
            // restore board state
            restore_board();

            if (stopped) return 0;

            // beta cutoff
            if (score >= beta) {
                return beta;
            }

            if (score > alpha) {
                alpha = score;
            }
        }
    }
    return alpha;
}

const int full_depth = 6;
const int reduction_limit = 3;

static inline int alpha_beta_search(int alpha, int beta, int depth) {

    if ((nodes & 2047) == 0)
    {
        communicate();
    }

    // init PV length
    pv_length[ply] = ply;

    if (depth == 0) {
        return quiescence(alpha, beta);
    }

    nodes++;

    if (ply > 63) { return evaluate(); }

    // find out if king is in check
    bool in_check = is_square_attacked((side == white) ? get_lsb_index(bitboards[K]) : 
                                                        get_lsb_index(bitboards[k]),
                                                        side ^ 1);
    
    // if we are in check, increase depth
    if (in_check) { depth++; } 

    // init score
    int score;

    // apply null move pruning
    if (depth >= reduction_limit && !in_check && ply) {
        // preserve board state
        copy_board();

        // switch side to emulate no move
        side ^= 1;

        // reset en passant square
        en_passant = no_sq;

        // move search
        score = -alpha_beta_search(-beta, -beta + 1, depth - 1 - 2);

        restore_board();

        if (stopped)
        {
            return 0;
        }

        // beta cutoff
        if (score >= beta) {
            return beta;
        }
    }


    // init moves list
    moves* move_list = new moves();

    // generate moves
    generate_moves(move_list);

    // pv scoring (experimental, may remove)
    if ( search_pv ) {
        enable_pv_scoring(move_list);
    }

    // sort moves
    sort_moves(move_list);

    // init variable to check for checkmates
    bool move_played = false;

    // init moves searched
    int moves_searched = 0;

    // iterate over moves
    for (int i = 0; i < move_list->count; i++) {

        // save board state
        copy_board();

        // make legal moves
        if (!make_move(move_list->moves[i], all_moves)) {

            continue;

        } else {
            
            // increment ply
            ply++;

            // move was played
            move_played = true;

            // first move searched
            if (moves_searched == 0) {
                // do normal recursive search
                score = -alpha_beta_search(-beta, -alpha, depth - 1);
            } else {
                // late move reduction (lmr)
                if (moves_searched >= full_depth && depth >= reduction_limit && !in_check
                && !get_move_capture(move_list->moves[i]) && !get_move_promoted(move_list->moves[i])) {
                    // reduced move search
                    score = -alpha_beta_search(-alpha - 1, -alpha, depth - 2);
                } else {
                    // trick to ensure that full move search is done
                    score = alpha + 1;
                }

                if ( score > alpha ) {
                    score = -alpha_beta_search(-alpha - 1, -alpha, depth - 1);

                    // check for failure of reduced check
                    if ((score > alpha) && (score < beta)) {
                        score = -alpha_beta_search(-beta, -alpha, depth - 1);
                    }
                }
            }
            
            // decrement ply
            ply--;

            // undo move
            restore_board();

            if (stopped) return 0;

            // increment moves searched counter
            moves_searched++;

            // move fails high
            if (score >= beta) {

                if (get_move_capture(move_list->moves[i]) == 0) {

                    killer_moves[1][ply] = killer_moves[0][ply];
                    killer_moves[0][ply] = move_list->moves[i];

                }

                return beta; 
            }

            // best score so far
            if (score > alpha) {

                // store history moves
                if (get_move_capture(move_list->moves[i]) == 0) {
                    
                    history_moves[get_move_piece(move_list->moves[i])][get_move_target(move_list->moves[i])] += depth;

                }

                // update alpha
                alpha = score;

                //write PV move
                pv_table[ply][ply] = move_list->moves[i];

                // loop over the next ply
                for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++) {
                    // copy move from deeper ply to current ply
                    pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
                }
                pv_length[ply] = pv_length[ply + 1];
            }
        }
    }

    // no legal moves in position
    if (!move_played) {

        // check king in check
        if (in_check) {
            // checkmate
            return ~INFINITY + ply + 100;
        } else {
            // stalemate
            return 0;
        }
    }

    // move fails low
    return alpha;

}


void search_position(int depth) {

    // reset arrays needed in search
    memset(killer_moves, 0, sizeof(killer_moves));
    memset(history_moves, 0, sizeof(history_moves));
    memset(pv_table, 0, sizeof(pv_table));
    memset(pv_length, 0, sizeof(pv_length));
    
    // reset necessary variables
    nodes = 0;
    search_pv = false;
    score_pv = false;
    stopped = false;

    // define alpha and beta 
    int alpha = -INFINITY;
    int beta = INFINITY;

    // define window adjustment 
    int adj_window = 50;

    int start = get_time_ms();

    for (int cdepth = 1; cdepth <= depth; cdepth++) {
        
        // if time is up
        if (stopped)
        {
            // stop calculating and return best move so far
            break;
        }

        // enable search_pv
        search_pv = true;

        int score = alpha_beta_search(alpha, beta, cdepth);

        int stop = get_time_ms();

        /* 
        ASPIRATION WINDOW TECHNIQUE, NOT SURE IF I WILL USE

        // fell outside the window, so retry the search with initial window
        if (score <= alpha || score >= beta) {
            printf("score: %d, alpha: %d, beta: %d\n", score, alpha, beta);
            alpha = -INFINITY;
            beta = INFINITY;
            score = alpha_beta_search(alpha, beta, cdepth);
        }

        alpha = score - adj_window;
        beta = score + adj_window;

        */

        printf("<info> score: %d depth: %d nodes: %ld line: ", score, cdepth, nodes);

        for (int i = 0; i < pv_length[0]; i++) {
            print_move(pv_table[0][i]);
            printf(" ");
        }
        printf("\ntime elapsed (ms): %d", stop - start);
        printf("\n");

        start = stop;

    }

    printf("best move: ");
    print_move(pv_table[0][0]);
    printf("\n");

    return;

}

