#include <stdio.h>
#include "debug.h"
#include "game.h"
#include "hash.h"
#include "move.h"
#include "movegen.h"
#include "time.h"
#include "search.h"

#ifdef _WIN64
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

// parse fen string method
void parse_FEN(const char* fen) {
    // reset game state
    side = 0;
    en_passant = no_sq;
    castle = 0;
    occupancies[0] = 0ull;
    occupancies[1] = 0ull;
    occupancies[2] = 0ull;
    for (int i = 0; i < 12; i++) {
        bitboards[i] = 0ull;
    }


    int sq = 0;

    // parse string
    while (sq < 64) {
        if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z')) {
            set_bit(bitboards[char_pieces[*fen]], sq);
            sq++;
        } else if (*fen >= '1' && *fen <= '8') sq += (*fen - '0'); // 48 is a constant (ascii value of '1' is 49)
        
        fen++;
    }
    fen++; // should be a space
    side = (*fen == 'w') ? white : black;
    fen += 2; // skip w/b and space
    while (*fen != ' ') {
        switch (*fen) {
            case ('K'):
                castle |= wk;
                break;
            case ('Q'):
                castle |= wq;
                break;
            case ('k'):
                castle |= bk;
                break;
            case ('q'):
                castle |= bq;
                break;
            default:
                break;
        }
            fen++;
    }

    fen++;
    if (*fen == '-') { fen ++; }
    else {
        char file = *fen - 'a';
        fen++;
        char rank = -((*fen - '0') - 8);

        en_passant = rank * 8 + file;
        fen++;
    }
    fen++;

    // update occupancies
    for (int i = 0; i < 12; i++) {
        if (i <= 5) {
            occupancies[white] |= bitboards[i];
        } else {
            occupancies[black] |= bitboards[i];
        }
    }
    occupancies[both] = occupancies[white] | occupancies[black];

    hkey = hash();

    return; // don't know what to do with last ints yet
    
}

int parse_move(const char* move_string) { // ex: a7a8q
    
    // define source and target square
    int source_sq, target_sq;

    // init move list
    moves move_list {};

    // generate moves
    generate_moves(&move_list);

    // extract file
    int file = move_string[0] - 'a';

    // extract rank
    int rank = 8 - (move_string[1] - '0');

    // init source square
    source_sq = file + rank * 8;

    // get rank and file for target square
    file = move_string[2] - 'a';
    rank = 8 - (move_string[3] - '0');

    // init target square
    target_sq = file + rank * 8;

    // init promoted piece
    int promoted = 0;

    for (int i = 0; i < move_list.count; i++) {


        // check if move is in list
        if (source_sq == get_move_source(move_list.moves[i]) && target_sq == get_move_target(move_list.moves[i])) {

            if (get_move_promoted(move_list.moves[i])) {
                // update promoted
                int promoted = get_move_promoted(move_list.moves[i]);

                if (ASCII_pieces[promoted] == move_string[4]) {
                    // return move
                    return move_list.moves[i];
                }

            } else {
                // return move
                return move_list.moves[i];
            }
        }
    }

    // illegal move
    return 0;

}


// parse position function
void parse_position(char* command) {

    // check for position command
    if (strncmp(command, "position", 8) == 0) {

        // skip to beginning of next token
        command += 9;

    } else {

        printf("\ninvalid position string\n\n");
        return;

    }

    // parse next token
    if (strncmp(command, "startpos", 8) == 0) {

        // initialize board state
        parse_FEN(start_position);

    } else if (strncmp(command, "fen", 3) == 0) {

        // skip to fen string
        command += 4;

        // parse fen string
        parse_FEN(command);

    } else {

        printf("\ninvalid position string\n\n");
        return;

    }
    
    // parse moves after fen string
    command = strstr(command, "moves");

    // string was found
    if (command) {
        // increment to beginning of next token
        command += 6;
    }

    // loop over moves
    while (command) {

        // parse move
        int move = parse_move(command);

        // valid move
        if (move) {

            // play move
            make_move(move, all_moves);

        } else {
            // invalid move
            break;
        }

        // increment to next move
        if (get_move_promoted(move)) 
            command += 6;
        else
            command += 5;

    }

    print_board();

    return;
}

void parse_go(char* command) {

    // init depth
    int depth = -1;

    // init pointer to null
    char* argument = NULL;

    // infinite search
    if ((argument = strstr(command,"infinite"))) {}

    // match UCI "binc" command
    if ((argument = strstr(command,"binc")) && side == black)
        // parse black time increment
        inc = atoi(argument + 5);

    // match UCI "winc" command
    if ((argument = strstr(command,"winc")) && side == white)
        // parse white time increment
        inc = atoi(argument + 5);

    // match UCI "wtime" command
    if ((argument = strstr(command,"wtime")) && side == white)
        // parse white time limit
        time = atoi(argument + 6);

    // match UCI "btime" command
    if ((argument = strstr(command,"btime")) && side == black)
        // parse black time limit
        time = atoi(argument + 6);

    // match UCI "movestogo" command
    if ((argument = strstr(command,"movestogo")))
        // parse number of moves to go
        movestogo = atoi(argument + 10);

    // match UCI "movetime" command
    if ((argument = strstr(command,"movetime")))
        // parse amount of time allowed to spend to make a move
        movetime = atoi(argument + 9);

    // match UCI "depth" command
    if ((argument = strstr(command,"depth")))
        // parse search depth
        depth = atoi(argument + 6);

    // if move time is not available
    if(movetime != -1)
    {
        // set time equal to move time
        time = movetime;

        // set moves to go to 1
        movestogo = 1;
    }

    // init start time
    starttime = get_time_ms();

    // init search depth
    depth = depth;

    // if time control is available
    if(time != -1)
    {
        // flag we're playing with time control
        timeset = 1;

        // set up timing
        time /= movestogo;
        time -= 50;
        stoptime = starttime + time + inc;
    }

    // if depth is not available
    if(depth == -1)
        // set depth to 64 plies (takes ages to complete...)
        depth = 64;

    // print debug info
    printf("time:%d start:%d stop:%d depth:%d timeset:%d\n",
    time, starttime, stoptime, depth, timeset);

    // search position
    search_position(depth);

}


// main UCI loop
void uci_loop() {

    // reset in & out buffers
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    // define user input buffer
    char input[2000];

    // print engine info
    printf("id name bb engine\n");
    printf("id name Willcm\n");
    printf("uciok\n");

    // begin uci loop
    while (1) {

        // reset user input buffer
        memset(input, 0, sizeof(input));
        
        // make sure output reaches the gui
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) {
            continue;
        }

        // make sure input is available
        if (input[0] == '\n') {
            continue;
        }
        
        // parse isready command
        if (strncmp(input, "isready", 7) == 0) {
            printf("readyok\n");
            continue;
        }

        else if (strncmp(input, "position", 8) == 0) {
            parse_position(input);
            continue;
        }

        else if (strncmp(input, "ucinewgame", 10) == 0) {
            // new game
            char pos[18] = "position startpos";
            parse_position(pos);
        }

        else if (strncmp(input, "go", 2) == 0) {
            parse_go(input);
        }

        else if (strncmp(input, "quit", 4) == 0) {
            // exit engine
            break;
        }

        else if (strncmp(input, "uci", 3) == 0) {
            printf("id name bb engine\n");
            printf("id name Willcm\n");
            printf("uciok\n");
        }
    }
}
