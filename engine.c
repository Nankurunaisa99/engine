#include <stdio.h>
#include "define.h"

//---------- MAIN ----------//

int main(){

    init_leapers_attacks();
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);

    parse_fen(tricky_position);
    print_board();

    int move = encode_move(e2, e4, P, 0, 0, 0, 0, 0);
    int source_square = get_move_source(move);
    int target_square = get_move_target(move);

    printf("Source square: %s\n", square_to_coordinates[source_square]);
    printf("Target square: %s\n", square_to_coordinates[target_square]);
    generate_moves();

    return 0;

}