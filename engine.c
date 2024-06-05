#include <stdio.h>
#include "define.h"

//---------- MAIN ----------//

int main(){

    init_leapers_attacks();
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);

    parse_fen(tricky_position_enpassant);
    print_board();

    int move = parse_move("d5c6");
    printf("Move: %d\n", move);
    if(move){
        make_move(move, all_moves);
        print_board();
    }
    else
        printf("Illegal move\n");
    return 0;

}