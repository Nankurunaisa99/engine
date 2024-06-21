#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "define.h"

int engine_move;

/*
    RANDOM MOVE ENGINE: i pezzi si muovono in modo casuale, evitando se possibile
    di muovere il re
*/
void random_move_engine(){
    srand(time(NULL));
    moves move_list[1];
    generate_moves(move_list);
    int return_move;
    int king_flag;
    int only_king_count = 0;
    int number_of_moves = move_list->count;
    do{
        only_king_count++;
        king_flag = 1;
        return_move = move_list->moves[rand() % number_of_moves];
        if(get_move_piece(return_move) == k || get_move_piece(return_move) == K) king_flag = 0;     
    } while(king_flag == 0 && only_king_count < 10);
    engine_move = return_move;
}

/*
    PSEUDO RANDOM ENGINE: in base alla potenza dell'engine, deciderà se fare una mossa casuale
    o la mossa migliore (depth = 4)
*/
void pseudo_random_engine(int strength){
    srand(time(NULL));
    int random_number = rand() % 100;
    if(random_number > strength){
        random_move_engine();
        
    }
    else{
        search_position(4);
        engine_move = pv_table[0][0];
    }
}

/*
    ANGRY ENGINE: i pezzi si muovono in modo casuale, ma se c'è una cattura disponibile la eseguono
    (preferendo le catture di pezzi di valore maggiore secondo la regola mvv_lva)
*/
void angry_engine(){
    int is_a_capture = 0;
    int capture_moves_number = 0;
    int target_piece = P;
    moves move_list[1];
    generate_moves(move_list);
    //Controllo se il re è in check, nel caso lo fosse, cerco la mossa migliore possibile
    if(is_square_attacked(get_less_significant_bit_index(bitboards[k]), white) || is_square_attacked(get_less_significant_bit_index(bitboards[K]), black)){
        search_position(3);
        engine_move = pv_table[0][0];
        return;
    }
    //Prendo nota del numero di mosse di cattura disponibili
    for(int move_number = 0; move_number < move_list->count; move_number++){
        if(get_move_capture(move_list->moves[move_number])){
            capture_moves_number++;
        }
    }
    printf("\nNumber of captures available in the position: %d\n", capture_moves_number);
    if(capture_moves_number == 0){
        random_move_engine();
    }
    else{
        int index = 0;
        int start_piece, end_piece;
        capture_moves captures[capture_moves_number];
        for(int move_number = 0; move_number < move_list->count; move_number++){
            if(get_move_capture(move_list->moves[move_number])){
                int start_piece, end_piece;
                if(side == white){
                    start_piece = p;
                    end_piece = k;
                }
                else{
                    start_piece = P;
                    end_piece = K;
                }
                for(int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++){
                    if(get_bit(bitboards[bb_piece], get_move_target(move_list->moves[move_number]))){
                        target_piece = bb_piece;
                        break;
                    }
                }
                captures[index].move = move_list->moves[move_number];
                captures[index].capture_value = mvv_lva[target_piece][get_move_piece(move_list->moves[move_number])];
                printf("Capture move added: ");
                print_move(captures[index].move);
                printf("\n");
                index++;
            }
        }
        int return_move;
        int max_score = -9999;
        for(int i = 0; i < capture_moves_number; i++){
            if(captures[i].capture_value > max_score){
                return_move = captures[i].move;
                max_score = captures[i].capture_value;
            }
        }
        if((get_move_piece(return_move) == k || get_move_piece(return_move) == K)){
            search_position(3);
            engine_move = pv_table[0][0];
        }
        else engine_move = return_move;
    }    
}