#include <stdio.h>
#include "define.h"

//---------- MAIN ----------//

int main(){
    init_leapers_attacks();

    print_bitboard(mask_knight_attacks(c2));
    print_bitboard(mask_knight_attacks(h3));

    return 0;

}