#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "define.h"

int parse_move(char *move_string){
    moves move_list[1];
    generate_moves(move_list);

    int source_square = (move_string[0] - 'a') + (8 - (move_string[1] - '0')) * 8;
    int target_square = (move_string[2] - 'a') + (8 - (move_string[3] - '0')) * 8;
    //printf("Source square: %s\n", square_to_coordinates[source_square]);
    //printf("Target square: %s\n", square_to_coordinates[target_square]);

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

/*
    Example UCI commands to init position on chess board
    
    // init start position
    position startpos
    
    // init start position and make the moves on chess board
    position startpos moves e2e4 e7e5
    
    // init position from FEN string
    position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 
    
    // init position from fen string and make moves on chess board
    position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 moves e2a6 e8g8
*/

void parse_position(char *command){
    // shift the pointer to the first character of the position command
    command += 9;
    char *current_char = command;

    //Primo caso di comando UCI: Posizione iniziale
    if(strncmp(command, "startpos", 8) == 0){
        parse_fen(start_position);
    }
    else{
        //Secondo caso di comando UCI: Fen string
        current_char = strstr(command, "fen");
        if(current_char == NULL)
            parse_fen(start_position);
        else{
            current_char += 4;
            parse_fen(current_char);
        }
    }

    // shift the pointer to the first character of the moves command
    current_char = strstr(command, "moves");
    if(current_char != NULL){
        current_char += 6; //Skippo la parola moves
        while(*current_char){
            int move = parse_move(current_char);
            if(move == 0)
                break;
            make_move(move, all_moves);
            while(*current_char && *current_char != ' ')
                current_char++; //Qua ci fermiamo allo spazio tra una mossa e l'altra
            if(*current_char == '\0') break;
            else
                current_char++; //Qua ci fermiamo alla prima lettera della prossima mossa se non siamo alla fine della stringa
        }
    }
    print_board();
}


/*
    Example UCI commands to make engine search for the best move
    
    // fixed depth search
    go depth 4

*/

void parse_go(char *command){
    int depth = -1;
    char *current_depth = NULL;
    if(current_depth = strstr(command, "depth"))
        depth = atoi(current_depth + 6);
    else
        depth = 6;
    search_position(depth);
}

void uci_loop(){
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char input[2000];

    printf("id name %s\n", engine_name);
    printf("id author %s\n", author_name);
    printf("uciok\n");

    while(1){
        memset(input, 0, sizeof(input));
        fflush(stdout);
        if(!fgets(input, 2000, stdin))
            continue;

        if(input[0] == '\n')
            continue;

        if(strncmp(input, "isready", 7) == 0){
            printf("readyok\n");
            continue;
        }
        else if(strncmp(input, "ucinewgame", 10) == 0) parse_position("position startpos");
        
        else if(strncmp(input, "position", 8) == 0) parse_position(input);
        
        else if(strncmp(input, "go", 2) == 0) parse_go(input);

        else if(strncmp(input, "quit", 4) == 0) break;
        
        else if(strncmp(input, "uci", 3) == 0){
            printf("id name %s\n", engine_name);
            printf("id author %s\n", author_name);
            printf("uciok\n");
            continue;
        }
    }
}