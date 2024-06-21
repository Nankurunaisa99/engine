#include <stdio.h>
#include <unistd.h>
#include "define.h"

//---------- MAIN ----------//

int main(){

    init_leapers_attacks();
    init_sliders_attacks(bishop);
    init_sliders_attacks(rook);

    /*
        MODALITA': 
        pvp: player vs player
        pvc: player vs computer
        cvc: computer vs computer
        cvp: computer vs player
        uci: universal chess interface (da capire come mai non funziona il maledetto uci_loop())
    */
    uci_loop();
   
   // do{
   //     angry_engine();
   //     make_move(engine_move, all_moves);
   //     print_board();
   //     usleep(3000000);
   // }while(engine_move);
    /*
    if(mode == pvc || mode == cvc || mode == cvp){
        parse_fen(start_position);
        print_board();
        while(1){
            if(mode == pvc){
                if(side == white){
                    char input[6];
                    printf("Enter move: ");
                    scanf("%s", input);
                    int move = parse_move(input);
                    if(move == -1){
                        printf("Invalid move\n");
                        continue;
                    }
                    make_move(move, all_moves);
                    print_board();
                    old_move = move;
                }
                else{
                    search_position(4);
                    make_move(pv_table[0][0], all_moves);
                    print_board();
                }
            }
            else if(mode == cvp){
                if(side == black){
                    char input[6];
                    printf("Enter move: ");
                    scanf("%s", input);
                    int move = parse_move(input);
                    if(move == -1){
                        printf("Invalid move\n");
                        continue;
                    }
                    make_move(move, all_moves);
                    print_board();
                    old_move = move;
                }
                else{
                    search_position(4);
                    make_move(pv_table[0][0], all_moves);
                    print_board();
                }
            }
            else if(mode == cvc){
                search_position(6);
                make_move(pv_table[0][0], all_moves);
                print_board();
            }
        }
    }
    else
        uci_loop();
    */
    return 0;
}

