#include "debug.h"
#include "hash.h"

void print_bitboard(u64 bitboard) {
    for (int rank = 0; rank < 8; ++rank) {

        for (int file = 0; file < 8; ++file) {

            int square = rank * 8 + file;
            
            // print rank numbers
            if (!file) {
                printf("  %d ", 8 - rank);
            }

            printf(" %d", get_bit(bitboard, square) ? 1 : 0);

        }
        printf("\n");

    }
    // print file letters
    printf("\n     a b c d e f g h\n\n");

    // print bitboard as hex number
    printf("     Bitboard: 0x%llX\n\n", bitboard);
}

void print_board() {

    for (int rank = 0; rank < 8; ++rank) {

        for (int file = 0; file < 8; ++file) {

            int square = rank * 8 + file;
            
            // print rank numbers
            if (!file) {
                printf("  %d ", 8 - rank);
            }
            bool empty = true;
            for (int i = 0; i < 12; i++) {
                if (get_bit(bitboards[i], square)) {
                    #ifdef _WIN64
                        printf(" %c", ASCII_pieces[i]);
                    #else 
                        printf(" %s", unicode_pieces[i]);
                    #endif
                    empty = false;
                    break;
                }
            }   
            if (empty) { printf(" ."); }
        }
        printf("\n");

    }
    // print file letters
    printf("\n     a b c d e f g h\n\n");

    // print side to move
    printf("     Side:     %s\n", side % 2 ? "black" : "white");

    // print en passant square
    printf("     Enpassant: %4s\n", en_passant != no_sq ? square_to_coord[en_passant] : "none");

    // print castling rights
    printf("     Castle:    %c%c%c%c\n", (castle & wk) ? 'K' : '-',
                                         (castle & wq) ? 'Q' : '-',
                                         (castle & bk) ? 'k' : '-',
                                         (castle & bq) ? 'q' : '-');

    // print hash
    printf("     Hash: 0x%llx\n", hkey);

    
}
