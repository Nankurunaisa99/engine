#include <stdio.h>
#include "define.h"

void print_bitboard(U64 bitboard){
    printf("\n");

    for(int rank = 0; rank < 8; rank++){
        for(int file = 0; file < 8; file++){
            int square = rank * 8 + file;
            
            if(!file) printf(" %d  ", 8 - rank);

            printf(" %d ", get_bit(bitboard, square) ? 1 : 0);
        }
        printf("\n");
    }

    printf("\n     A  B  C  D  E  F  G  H\n\n");
    printf("     Bitboard: %llX\n\n", bitboard);
}