#include <stdio.h>
#include "define.h"

//---------- MAIN ----------//

int main(){
    init_leapers_attacks();
    U64 attack_mask = mask_bishop_attacks(d5);
    for(int i = 0; i < 10; i++) print_bitboard(set_occupancy(i, count_bits(attack_mask), attack_mask));

    return 0;
}