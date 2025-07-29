Welcome to the codebase. Here's what each file does:

## types.h

* contains certain variable aliases as well as most enumerations used
* contains some helpful functions for move generation/square manipulation
* contains the Move class

## bitboard.cpp/h

* contains bitboard related functions that are largely used for move generation

## position.cpp/h

* contains position class and related functions

## movegen.cpp/h

* contains move generation logic
* contains templated MoveList class
* supports generation of multiple types of moves
    * LEGAL
    * CAPTURES
    * EVASIONS
    * NON EVASIONS
    * QUIET

## moveorder.cpp/h 

* contains MoveOrder and MoveQueue class
* MoveOrder is used to sort moves for a search to improve alpha/beta cutoffs
* MoveQueue is wrapped around a MoveList or MoveOrder class to provide a thread safe access

## tt.cpp/h

* contains TranspositionTable class as well as related structures
* used to access transpositions in order to speed up search
    * a transposition is when the same position is reached from different move orders

## search

* contains Worker class, which encapsulates perft and search logic

## thread

* contains Thread and ThreadPool class, which are used to manage (start/end) the search

## engine

* contains Engine class, which is used as an intermediary between UCIEngine and the threads
* Engine class contains data that is shared between threads

## uciengine

* contains UCIEngine class, which is used to interact with std::in and parse commands
