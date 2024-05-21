#include <stdio.h>
#include "define.h"

//---------- MAIN ----------//

int main(){
    U64 bitboard = 0ULL; 

    print_bitboard(mask_king_attacks(h8));

    return 0;
}