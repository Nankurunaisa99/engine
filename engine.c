#include <stdio.h>
#include "define.h"

//---------- GLOBALS ----------//

U64 pawn_attacks[2][64];
U64 knight_attacks[64];
U64 king_attacks[64];

//---------- FUNCTIONS ----------//

void init_leapers_attacks()
{
    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
    {
        // init pawn attacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);
        
        // init knight attacks
        knight_attacks[square] = mask_knight_attacks(square);
        
        // init king attacks
        king_attacks[square] = mask_king_attacks(square);
    }
}


//---------- MAIN ----------//

int main(){
    U64 bitboard = 0ULL; 
    //print_bitboard(mask_rook_attacks(a2));
    set_bit(bitboard, b5);
    set_bit(bitboard, e3);
    print_bitboard(rook_attacks_on_the_fly(b3, bitboard));
    printf("\nNumero di bit attivi: %d\n", count_bits(rook_attacks_on_the_fly(b3, bitboard)));

    return 0;
}