#include <stdio.h>
#include "define.h"

void generate_moves(){
    int source_square, target_square;
    U64 bitboard, attacks;

    for(int piece = P; piece <= k; piece++){
        bitboard = bitboards[piece]; 

        if(side == white){ //Qui si generano le mosse delle pedine e gli arrocchi, che sono dipendenti dal colore
            if(piece == P){
                while(bitboard){
                    source_square = get_less_significant_bit_index(bitboard); //Si parte dalla prima pedina trovata
                    target_square = source_square - 8; //avanzamento in avanti della pedina

                    if(!(target_square < a8) && !get_bit(occupancies[both], target_square)){
                        //Ora, dopo aver verificato che la casella di destinazione è valida e non è occupata
                        //si procede con il discriminare se la mossa è una promozione, una mossa di due caselle o una mossa normale

                        //Promozione
                        if(source_square >= a7 && source_square <= h7){
                            printf("Pawn promotion: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("Pawn promotion: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("Pawn promotion: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("Pawn promotion: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        }
                        else{
                            //Mossa normale
                            printf("Pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            //Mossa di due caselle
                            if(source_square >= a2 && source_square <= h2 && !get_bit(occupancies[both], target_square - 8)) //-8 perchè si muove avanti di 2 caselle
                                printf("Pawn double push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square - 8]);
                        }
                    }

                    attacks = pawn_attacks[white][source_square] & occupancies[black];

                    while(attacks){
                        target_square = get_less_significant_bit_index(attacks);

                            //Si procede con il discriminare se la cattura porta ad una promozione, è una catura normale o un enpassant

                            //Promozione
                        if(source_square >= a7 && source_square <= h7){
                           printf("Pawn capture and promotion: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                           printf("Pawn capture and promotion: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                           printf("Pawn capture and promotion: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                           printf("Pawn capture and promotion: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        }
                        else{
                            //Mossa normale
                            printf("Pawn capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        }
                        clear_bit(attacks, target_square);
                    }

                    if(enpassant != no_sqr){
                        U64 enpassant_attacks = pawn_attacks[white][source_square] & (1ULL << enpassant); //L'unico enpassant possibile
                        if(enpassant_attacks){
                            int target_enpassant = get_less_significant_bit_index(enpassant_attacks);
                            printf("Pawn enpassant: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_enpassant]);
                        }
                    
                    }
                    clear_bit(bitboard, source_square);

                   
                }
            }

            //Arrocco
            if (piece == K){
                if (castle & wk){
                    // make sure square between king and king's rook are empty
                    if (!get_bit(occupancies[both], f1) && !get_bit(occupancies[both], g1)){
                        // make sure king and the f1 squares are not under attacks
                        if (!is_square_attacked(e1, black) && !is_square_attacked(f1, black))
                            printf("castling move: e1g1\n");
                    }
                }
                
                if (castle & wq){
                    // make sure square between king and queen's rook are empty
                    if (!get_bit(occupancies[both], d1) && !get_bit(occupancies[both], c1) && !get_bit(occupancies[both], b1)){
                        // make sure king and the d1 squares are not under attacks
                        if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black))
                            printf("castling move: e1c1\n");
                    }
                }
            }
        }
        else if(side == black){ //uguale al bianco, ma per il nero
            if(piece == p){
                while(bitboard){
                    source_square = get_less_significant_bit_index(bitboard); //Si parte dalla prima pedina trovata
                    target_square = source_square + 8; //avanzamento in avanti della pedina
                    if(!(target_square > h1) && !get_bit(occupancies[both], target_square)){
                    //Ora, dopo aver verificato che la casella di destinazione è valida e non è occupata
                    //si procede con il discriminare se la mossa è una promozione, una mossa di due caselle o una mossa normale
                    //Promozione
                        if(source_square >= a2 && source_square <= h2){
                            printf("Pawn promotion: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("Pawn promotion: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("Pawn promotion: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("Pawn promotion: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        }
                        else{
                            //Mossa normale
                            printf("Pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            //Mossa di due caselle
                            if(source_square >= a7 && source_square <= h7 && !get_bit(occupancies[both], target_square + 8)) //+ 8 perchè si muove avanti di 2 caselle
                                printf("Pawn double push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square + 8]);
                        }
                    }
                    attacks = pawn_attacks[black][source_square] & occupancies[white];
                    while(attacks){
                        target_square = get_less_significant_bit_index(attacks);
                            //Si procede con il discriminare se la cattura porta ad una promozione, è una catura normale o un enpassant
                            //Promozione
                        if(source_square >= a2 && source_square <= h2){
                           printf("Pawn capture and promotion: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                           printf("Pawn capture and promotion: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                           printf("Pawn capture and promotion: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                           printf("Pawn capture and promotion: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        }
                        else{
                            //Mossa normale
                            printf("Pawn capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        }
                        clear_bit(attacks, target_square);
                    }
                    if(enpassant != no_sqr){
                        U64 enpassant_attacks = pawn_attacks[black][source_square] & (1ULL << enpassant); //L'unico enpassant possibile
                        if(enpassant_attacks){
                            int target_enpassant = get_less_significant_bit_index(enpassant_attacks);
                            printf("Pawn enpassant: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_enpassant]);
                        }
                    }
                    clear_bit(bitboard, source_square);
                }
            }
            if (piece == k){
                if (castle & bk){
                    // make sure square between king and king's rook are empty
                    if (!get_bit(occupancies[both], f8) && !get_bit(occupancies[both], g8)){
                        // make sure king and the f8 squares are not under attacks
                        if (!is_square_attacked(e8, white) && !is_square_attacked(f8, white))
                            printf("castling move: e8g8\n");
                    }
                }

                if (castle & bq){
                    if (!get_bit(occupancies[both], d8) && !get_bit(occupancies[both], c8) && !get_bit(occupancies[both], b8)){
                        // make sure king and the d8 squares are not under attacks
                        if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white))
                            printf("castling move: e8c8\n");
                    }
                }
            }
        }
        //Ora genero le mosse per tutti gli altri pezzi che non dipendono dal colore
        //Knight moves
        if((side == white) ? piece == N : piece == n){
            while(bitboard){
                source_square = get_less_significant_bit_index(bitboard);
                attacks = knight_attacks[source_square] & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

                while(attacks){
                    target_square = get_less_significant_bit_index(attacks);
                    //Ora devo differenziare le mosse normali con le catture

                    //Mossa Normale
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                        printf("Knight normal move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    //Cattura
                    else printf("Knight capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    clear_bit(attacks, target_square);
                }

                clear_bit(bitboard, source_square);
            }
        }
        //Bishop moves
        if((side == white) ? piece == B : piece == b){
            while(bitboard){
                source_square = get_less_significant_bit_index(bitboard);
                attacks = get_bishop_attacks(source_square, occupancies[both]) & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

                while(attacks){
                    target_square = get_less_significant_bit_index(attacks);
                    //Ora devo differenziare le mosse normali con le catture

                    //Mossa Normale
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                        printf("Bishop normal move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    //Cattura
                    else printf("Bishop capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    clear_bit(attacks, target_square);
                }

                clear_bit(bitboard, source_square);
            }
        }
        //Rook moves
        if((side == white) ? piece == R : piece == r){
            while(bitboard){
                source_square = get_less_significant_bit_index(bitboard);
                attacks = get_rook_attacks(source_square, occupancies[both]) & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

                while(attacks){
                    target_square = get_less_significant_bit_index(attacks);
                    //Ora devo differenziare le mosse normali con le catture

                    //Mossa Normale
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                        printf("Rook normal move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    //Cattura
                    else printf("Rook capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    clear_bit(attacks, target_square);
                }

                clear_bit(bitboard, source_square);
            }
        }
        //Queen moves
        if((side == white) ? piece == Q : piece == q){
            while(bitboard){
                source_square = get_less_significant_bit_index(bitboard);
                attacks = get_queen_attacks(source_square, occupancies[both]) & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

                while(attacks){
                    target_square = get_less_significant_bit_index(attacks);
                    //Ora devo differenziare le mosse normali con le catture

                    //Mossa Normale
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                        printf("Queen normal move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    //Cattura
                    else printf("Queen capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    clear_bit(attacks, target_square);
                }

                clear_bit(bitboard, source_square);
            }
        }
        //King moves
        if((side == white) ? piece == K : piece == k){
            while(bitboard){
                source_square = get_less_significant_bit_index(bitboard);
                attacks = king_attacks[source_square] & ((side == white) ? ~occupancies[white] : ~occupancies[black]);

                while(attacks){
                    target_square = get_less_significant_bit_index(attacks);
                    //Ora devo differenziare le mosse normali con le catture

                    //Mossa Normale
                    if (!get_bit(((side == white) ? occupancies[black] : occupancies[white]), target_square))
                        printf("King normal move: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    //Cattura
                    else printf("King capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    clear_bit(attacks, target_square);
                }

                clear_bit(bitboard, source_square);
            }
        }
    }
}

void print_move(int move){
    printf("%s%s%c\n", square_to_coordinates[get_move_source(move)],
                        square_to_coordinates[get_move_target(move)],
                        promoted_pieces[get_promoted_piece(move)]);
}