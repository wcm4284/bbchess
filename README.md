Welcome to my chess engine! I've been doing this in an attempt to learn proper c++ style, so forgive me if it's a bit of a mess.
I've been referring to Stockfish for inspiration, so it mirrors it's style and algorithm a fair bit.

I've been running WSL Ubuntu on my machine, so that is what it builds for. I cannot guarantee this that will work on your machine,
but I sure hope it does. In order to run this, just download the code locally, cd into the src directory, and run '''make''' and
'''./engine'''. This engine does support multithreading. In order to specify the number of threads that you want to be running the
engine, just tack it on to the end of '''./engine'''. For example, if you want to run with two threads searching, run '''./engine 2'''
in your terminal. 


The engine supports UCI protocol. It initializes with the start position. In order to set the engine to a different position,
run '''position fen "fen string"'''. You can find all sorts of fen strings online, but in order for this to work the fen string must be
proper. The handling for this does not manage bad fen strings and the program will either not work correctly or crash. From any position,
you can run perft tests or find the best move at a certain depth. To run perft tests, type '''go perft "depth"''', where depth is the depth you want the engine to search to. For example, '''go perft 2''' would run a perft test on the current position to a depth of 2. To find the best move at a position, run '''go depth "depth"''', where depth is again the depth we want to search to. So, for example, '''go depth 2''' will find the best move in the current position while searching up to 2 ply. Eventually you should be able to play a game against the engine, but that isn't supported yet.

I hope you enjoy!
