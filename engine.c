#include <stdio.h>
#include "define.h"

//---------- MAIN ----------//

int main(){

    init_leapers_attacks();
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);

    set_bit(bitboards[P], a2);
    print_bitboard(bitboards[P]);
    printf("\nPiece: %s\n", unicode_pieces[P]);

    return 0;

}