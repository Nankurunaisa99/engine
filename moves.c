#include <stdio.h>
#include "define.h"

void generate_moves(moves *move_list){
    move_list->count = 0;
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

                            add_move(move_list, encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));
                        }
                        else{
                            //Mossa normale
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                            //Mossa di due caselle
                            if((source_square >= a2 && source_square <= h2) && !get_bit(occupancies[both], target_square - 8)) //-8 perchè si muove avanti di 2 caselle
                                add_move(move_list, encode_move(source_square, target_square - 8, piece, 0, 0, 1, 0, 0));
                        }
                    }

                    attacks = pawn_attacks[white][source_square] & occupancies[black];

                    while(attacks){
                        target_square = get_less_significant_bit_index(attacks);

                            //Si procede con il discriminare se la cattura porta ad una promozione, è una catura normale o un enpassant
                            //Promozione
                        if(source_square >= a7 && source_square <= h7){

                           add_move(move_list, encode_move(source_square, target_square, piece, Q, 1, 0, 0, 0));
                           add_move(move_list, encode_move(source_square, target_square, piece, R, 1, 0, 0, 0));
                           add_move(move_list, encode_move(source_square, target_square, piece, B, 1, 0, 0, 0));
                           add_move(move_list, encode_move(source_square, target_square, piece, N, 1, 0, 0, 0));

                        }
                        else{
                            //Mossa normale
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                        }
                        clear_bit(attacks, target_square);
                    }

                    if(enpassant != no_sqr){
                        U64 enpassant_attacks = pawn_attacks[white][source_square] & (1ULL << enpassant); //L'unico enpassant possibile
                        if(enpassant_attacks){
                            int target_enpassant = get_less_significant_bit_index(enpassant_attacks);
                            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
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
                            add_move(move_list, encode_move(e1, g1, piece, 0, 0, 0, 0, 1));
                    }
                }
                
                if (castle & wq){
                    // make sure square between king and queen's rook are empty
                    if (!get_bit(occupancies[both], d1) && !get_bit(occupancies[both], c1) && !get_bit(occupancies[both], b1)){
                        // make sure king and the d1 squares are not under attacks
                        if (!is_square_attacked(e1, black) && !is_square_attacked(d1, black))
                           add_move(move_list, encode_move(e1, c1, piece, 0, 0, 0, 0, 1));
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
                            add_move(move_list, encode_move(source_square, target_square, piece, Q, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, R, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, B, 0, 0, 0, 0));
                            add_move(move_list, encode_move(source_square, target_square, piece, N, 0, 0, 0, 0));
                        }
                        else{
                            //Mossa normale
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                            //Mossa di due caselle
                            if((source_square >= a7 && source_square <= h7) && !get_bit(occupancies[both], target_square + 8)) //+ 8 perchè si muove avanti di 2 caselle
                                add_move(move_list, encode_move(source_square, target_square + 8, piece, 0, 0, 1, 0, 0));
                        }
                    }
                    attacks = pawn_attacks[black][source_square] & occupancies[white];
                    while(attacks){
                        target_square = get_less_significant_bit_index(attacks);
                            //Si procede con il discriminare se la cattura porta ad una promozione, è una catura normale o un enpassant
                            //Promozione
                        if(source_square >= a2 && source_square <= h2){
                           add_move(move_list, encode_move(source_square, target_square, piece, q, 1, 0, 0, 0));
                           add_move(move_list, encode_move(source_square, target_square, piece, r, 1, 0, 0, 0));
                           add_move(move_list, encode_move(source_square, target_square, piece, b, 1, 0, 0, 0));
                           add_move(move_list, encode_move(source_square, target_square, piece, n, 1, 0, 0, 0));
                        }
                        else{
                            //Mossa normale
                            add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                        }
                        clear_bit(attacks, target_square);
                    }
                    if(enpassant != no_sqr){
                        U64 enpassant_attacks = pawn_attacks[black][source_square] & (1ULL << enpassant); //L'unico enpassant possibile
                        if(enpassant_attacks){
                            int target_enpassant = get_less_significant_bit_index(enpassant_attacks);
                            add_move(move_list, encode_move(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
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
                            add_move(move_list, encode_move(e8, g8, piece, 0, 0, 0, 0, 1));
                    }
                }

                if (castle & bq){
                    if (!get_bit(occupancies[both], d8) && !get_bit(occupancies[both], c8) && !get_bit(occupancies[both], b8)){
                        // make sure king and the d8 squares are not under attacks
                        if (!is_square_attacked(e8, white) && !is_square_attacked(d8, white))
                            add_move(move_list, encode_move(e8, c8, piece, 0, 0, 0, 0, 1));
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
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    //Cattura
                    else add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    //Cattura
                    else add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    //Cattura
                    else add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    //Cattura
                    else add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
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
                        add_move(move_list, encode_move(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    //Cattura
                    else add_move(move_list, encode_move(source_square, target_square, piece, 0, 1, 0, 0, 0));
                    clear_bit(attacks, target_square);
                }

                clear_bit(bitboard, source_square);
            }
        }
    }
}

void print_move(int move){
    if(get_move_promoted(move))
    printf("%s%s%c", square_to_coordinates[get_move_source(move)],
                        square_to_coordinates[get_move_target(move)],
                        promoted_pieces[get_move_promoted(move)]);
    else
        printf("%s%s", square_to_coordinates[get_move_source(move)],
                        square_to_coordinates[get_move_target(move)]);                    
}

void print_move_list(moves *move_list){

    if(move_list->count == 0){
        printf("\n    No moves to print\n\n");
        return;
    }

    printf("\n    move    piece   capture   double    enpass    castling\n\n");
    
    // loop over moves within a move list
    for (int move_count = 0; move_count < move_list->count; move_count++)
    {
        // init move
        int move = move_list->moves[move_count];
        
        #ifdef WIN64
            // print move
            printf("    %s%s%c     %c       %d         %d         %d         %d\n", square_to_coordinates[get_move_source(move)],
                                                                                  square_to_coordinates[get_move_target(move)],
                                                                                  get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ',
                                                                                  ascii_pieces[get_move_piece(move)],
                                                                                  get_move_capture(move) ? 1 : 0,
                                                                                  get_move_double(move) ? 1 : 0,
                                                                                  get_move_enpassant(move) ? 1 : 0,
                                                                                  get_move_castling(move) ? 1 : 0);
        #else
            // print move
            printf("    %s%s%c     %s       %d         %d         %d         %d\n", square_to_coordinates[get_move_source(move)],
                                                                                  square_to_coordinates[get_move_target(move)],
                                                                                  get_move_promoted(move) ? promoted_pieces[get_move_promoted(move)] : ' ',
                                                                                  unicode_pieces[get_move_piece(move)],
                                                                                  get_move_capture(move) ? 1 : 0,
                                                                                  get_move_double(move) ? 1 : 0,
                                                                                  get_move_enpassant(move) ? 1 : 0,
                                                                                  get_move_castling(move) ? 1 : 0);
        #endif
        
        // print total number of moves
        
    }
    printf("\n\n    Total number of moves: %d\n\n", move_list->count);
}

void add_move(moves *move_list, int move){
    move_list->moves[move_list->count] = move;
    move_list->count++;
}

int make_move(int move, int move_flag){
    //quiet moves (mosse che non portano ad una cattura)
    if(move_flag == all_moves){
        copy_board(); //serve per poter tornare indietro nel caso in cui il re sia in scacco
        int source_square = get_move_source(move);
        int target_square = get_move_target(move);
        int piece = get_move_piece(move);
        int promoted = get_move_promoted(move);
        int capture = get_move_capture(move);
        int double_move = get_move_double(move);
        int enpas = get_move_enpassant(move);
        int castl = get_move_castling(move);

        clear_bit(bitboards[piece], source_square);
        set_bit(bitboards[piece], target_square);

        if(capture){
            int start_piece, end_piece;
            if(side == white){
                start_piece = p;
                end_piece = k;
            }
            else{
                start_piece = P;
                end_piece = K;
            }
            //loop over bitboards oppoite to the side to move
            for(int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++){
                if(get_bit(bitboards[bb_piece], target_square)){ //se la casella di destinazione è occupata da un pezzo avversario lo catturo e lo rimuovo dalla scacchiera
                    clear_bit(bitboards[bb_piece], target_square);
                    break;
                }
            }
        }
        //Handle pawn promotion
        if(promoted){
            clear_bit(bitboards[(side == white) ? P : p], target_square);
            set_bit(bitboards[(side == white) ? promoted : promoted + 6], target_square);
        }
        //Handle enpassant
        if(enpas)
            (side == white) ? clear_bit(bitboards[p], target_square + 8) : clear_bit(bitboards[P], target_square - 8);
        
        enpassant = no_sqr; //dopo qualsiasi mossa l'enpassant scompare perchè può essere effettuato solo subito dopo la mossa del pedone

        //Handle double pawn push and check if an enpassant is possible
        if(double_move){
            enpassant = (side == white) ? target_square + 8 : target_square - 8;
        }

        //Handle castling
        if(castl){
            switch(target_square){
                case g1:
                    clear_bit(bitboards[R], h1);
                    set_bit(bitboards[R], f1);
                    break;
                case c1:
                    clear_bit(bitboards[R], a1);
                    set_bit(bitboards[R], d1);
                    break;
                case g8:
                    clear_bit(bitboards[r], h8);
                    set_bit(bitboards[r], f8);
                    break;
                case c8:
                    clear_bit(bitboards[r], a8);
                    set_bit(bitboards[r], d8);
                    break;
            }
         
        }
        //Handle castling rights
        castle &= castling_rights[source_square] & castling_rights[target_square];

        //Update occupancies
        memset(occupancies, 0ULL, 24); //24 = 3 * sizeof(U64): è più veloce scrivere direttamente 24
        for(int bb_piece = P; bb_piece <= K; bb_piece++)
            occupancies[white] |= bitboards[bb_piece];
        for(int bb_piece = p; bb_piece <= k; bb_piece++)
            occupancies[black] |= bitboards[bb_piece];
        occupancies[both] = occupancies[white] | occupancies[black];
        
        //Switch side to move
        side ^= 1;

        //Check if king is in check
        if(is_square_attacked((side == white) ? get_less_significant_bit_index(bitboards[k]) : get_less_significant_bit_index(bitboards[K]), side)){
            take_back(); //Se il re si muove in una casella che è sotto attacco, la mossa è illegale, quindi torno indietro
            return 0;
        }
        else return 1;
    }
    else{
        if(get_move_capture(move)){
            //cattura
            make_move(move, all_moves);
        }
        else return 0;

    }
}

void print_move_scores(moves *move_list){
    printf(" Move scores:\n\n");
    for(int move_count = 0; move_count < move_list->count; move_count++){
        printf(" move: ");
        print_move(move_list->moves[move_count]);
        printf("  score: %d\n", score_move(move_list->moves[move_count]));
    }
}

int score_move(int move){
    if(score_pv){
        if(pv_table[0][ply] == move){
            score_pv = 0;
            //printf("PV move: ");
            //print_move(move);
            //printf(" ply: %d\n", ply);
            return 20000; //Se la mossa è la prima mossa della PV table, la sua priorità è massima
        }
    }
    if (get_move_capture(move)){
        int target_piece = P; //Lo inizializzo solo perchè nel caso di enpassant (pedone cattura pedone, non ci interessa il colore)
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
            if(get_bit(bitboards[bb_piece], get_move_target(move))){
                target_piece = bb_piece;
                break;
            }
        }
        return mvv_lva[get_move_piece(move)][target_piece] + 10000;
    }
    else {
        if(killer_move[0][ply] == move) return 9000; //Se la mossa è una killer move, la sua priorità è massima
        else if(killer_move[1][ply] == move) return 8000; //Se la mossa è una killer move secondaria, la sua priorità è massima
        else return history_moves[get_move_piece(move)][get_move_target(move)];  
    }
    return 0;
}

int sort_moves(moves *move_list){

    int move_scores[move_list->count];
    for(int move_count = 0; move_count < move_list->count; move_count++)
        move_scores[move_count] = score_move(move_list->moves[move_count]);
    //Non sarà il modo più efficiente di ordinare le mosse, ma l'array di mosse possibili è piccolo
    // quindi non ci saranno problemi di performance
    for (int current_move = 0; current_move < move_list->count; current_move++){
        for (int next_move = current_move + 1; next_move < move_list->count; next_move++){
            // compare current and next move scores
            if (move_scores[current_move] < move_scores[next_move])
            {
                // swap scores
                int temp_score = move_scores[current_move];
                move_scores[current_move] = move_scores[next_move];
                move_scores[next_move] = temp_score;
                
                // swap moves
                int temp_move = move_list->moves[current_move];
                move_list->moves[current_move] = move_list->moves[next_move];
                move_list->moves[next_move] = temp_move;
            }
        }
    }
}