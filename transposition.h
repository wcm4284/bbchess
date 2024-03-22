#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include "bit.h"
#include <stdio.h>

#define table_size 0x400000

#define hashfEXACT 0
#define hashfALPHA 1
#define hashfBETA  2

#define no_entry 1000000

struct Entry {
    u64 key;
    int depth;
    int flag;
    int score;
};

class TranspositionTable {

    private: 
        int size;
        Entry* tt;

    public:
        TranspositionTable() {

            size = table_size / sizeof(Entry);

            tt = new Entry[size];
        }

        void add(u64 hash, int depth, int flag, int score) {

            // get index
            int index = hash % size;

            // update values of entry
            tt[index].key = hash;
            tt[index].depth = depth;
            tt[index].flag = flag;
            tt[index].score = score;

        }

        int find(u64 hash, int depth, int alpha, int beta) {

            // get index
            int index = hash % size;

            Entry* position = &tt[index];

            if (position->key == hash) {

                // make sure we get the correct depth
                if (position->depth >= depth) {
                    
                    if (position->flag == hashfEXACT) {
                        return position->score;
                    }

                    if (position->flag == hashfALPHA && position->score <= alpha) {
                        return alpha;
                    }

                    if (position->flag == hashfBETA && position->score >= beta) {
                        return beta;
                    }
                }
            }
            return no_entry;
        }
};

TranspositionTable table {};

#endif

