#include <stdio.h>
#include "define.h"

//---------- MAIN ----------//

int main(){

    init_leapers_attacks();
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);

   // parse_fen("R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNNK1B1 w - - 0 1");
   parse_fen(tricky_position);
    print_board();

    moves move_list[1];
    move_list->count = 0;
    generate_moves(move_list);
    print_move_list(move_list);

    return 0;

}