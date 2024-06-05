#include <stdio.h>
#include "define.h"

int parse_move(char *move_string){
    moves move_list[1];
    generate_moves(move_list);

    int source_square = (move_string[0] - 'a') + (8 - (move_string[1] - '0')) * 8;
    int target_square = (move_string[2] - 'a') + (8 - (move_string[3] - '0')) * 8;
    printf("Source square: %s\n", square_to_coordinates[source_square]);
    printf("Target square: %s\n", square_to_coordinates[target_square]);

    for(int move_count = 0; move_count < move_list->count; move_count++){
        int parsed_move = move_list->moves[move_count];
        if(source_square == get_move_source(parsed_move) && target_square == get_move_target(parsed_move)){
            int promoted_piece = get_move_promoted(parsed_move);
            if(promoted_piece){
                if((promoted_piece == Q || promoted_piece == q) && move_string[4] == 'q')
                    return parsed_move;
                else if((promoted_piece == R || promoted_piece == r) && move_string[4] == 'r')
                    return parsed_move;
                else if((promoted_piece == B || promoted_piece == b) && move_string[4] == 'b')
                    return parsed_move;
                else if((promoted_piece == N || promoted_piece == n) && move_string[4] == 'n')
                    return parsed_move;
                continue; //nel caso strano in cui la promozione avvenga ad un pezzo strano (es: e7e9k)
            }
            return parsed_move;
        }
    }
    return 0; //Illegal move
}