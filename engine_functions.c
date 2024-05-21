#include <stdio.h>
#include "define.h"

//---------- GLOBALS ----------//

const U64 not_a_file = 18374403900871474942ULL;
const U64 not_h_file = 9187201950435737471ULL;
const U64 not_hg_file = 4557430888798830399ULL;
const U64 not_ab_file = 1822972355519532159;


//---------- FUNCTIONS ----------//


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
    printf("     Bitboard: %llud\n\n", bitboard);
}


U64 mask_pawn_attacks(int side, int square){

    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, square);
    
    // white pawns
    if (!side)
    {
        if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
        if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    }
    
    // black pawns
    else
    {
        if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
        if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);    
    }

    return attacks;
}

U64 mask_knight_attacks(int square){
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, square);

    if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_ab_file) attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_h_file) attacks |= (bitboard >> 6);
    if ((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file) attacks |= (bitboard << 15);
    if ((bitboard << 10) & not_hg_file) attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_a_file) attacks |= (bitboard << 6);

    return attacks;
}

U64 mask_king_attacks(int square){
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, square);

    if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if ((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);
    if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if ((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);
    if ((bitboard >> 8)) attacks |= (bitboard >> 8);
    if ((bitboard << 8)) attacks |= (bitboard << 8);

    return attacks;
}

U64 mask_bishop_attacks(int square){
    U64 attacks = 0ULL;
    int init_rank, init_file;

    int target_rank = square / 8;
    int target_file = square % 8;
    
    // mask relevant bishop occupancy bits
    for (init_rank = target_rank + 1, init_file = target_file + 1; init_rank <= 6 && init_file <= 6; init_rank++, init_file++) attacks |= (1ULL << (init_rank * 8 + init_file));
    for (init_rank = target_rank - 1, init_file = target_file + 1; init_rank >= 1 && init_file <= 6; init_rank--, init_file++) attacks |= (1ULL << (init_rank * 8 + init_file));
    for (init_rank = target_rank + 1, init_file = target_file - 1; init_rank <= 6 && init_file >= 1; init_rank++, init_file--) attacks |= (1ULL << (init_rank * 8 + init_file));
    for (init_rank = target_rank - 1, init_file = target_file - 1; init_rank >= 1 && init_file >= 1; init_rank--, init_file--) attacks |= (1ULL << (init_rank * 8 + init_file));
    
    // return attack map
    return attacks;
}

U64 mask_rook_attacks(int square){
    U64 attacks = 0ULL;

    int initial_rank, initial_file;
    
    // init target rank & files
    int target_rank = square / 8;
    int target_file = square % 8;
    
    // mask relevant bishop occupancy bits
    for (initial_rank = target_rank + 1; initial_rank <= 6; initial_rank++) attacks |= (1ULL << (initial_rank * 8 + target_file));
    for (initial_rank = target_rank - 1; initial_rank >= 1; initial_rank--) attacks |= (1ULL << (initial_rank * 8 + target_file));
    for (initial_file = target_file + 1; initial_file <= 6; initial_file++) attacks |= (1ULL << (target_rank * 8 + initial_file));
    for (initial_file = target_file - 1; initial_file >= 1; initial_file--) attacks |= (1ULL << (target_rank * 8 + initial_file));
    
    // return attack map
    return attacks;
}