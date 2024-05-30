#include <stdio.h>
#include <string.h>
#include "define.h"

//---------- FUNCTIONS ----------//

void print_bitboard(U64 bitboard){
    printf("\n");

    for(int rank = 0; rank < 8; rank++){
        for(int file = 0; file < 8; file++){
            int square = rank * 8 + file;
            
            if(!file) printf(" %d  ", 8 - rank);

            printf(" %d ", get_bit(bitboard, square) ? 1 : 0);
            
        }
        printf("\n");
    }

    printf("\n     A  B  C  D  E  F  G  H\n\n");
    printf("     Bitboard: %llud\n\n", bitboard);
}

void print_board(){
    printf("\n");

    for(int rank = 0; rank < 8; rank++){
        for(int file = 0; file < 8; file++){
            int square = rank * 8 + file;
            
            if(!file) printf(" %d  ", 8 - rank);

            int piece = -1;
            for(int bb_piece = P; bb_piece <= k; bb_piece++){
                if(get_bit(bitboards[bb_piece], square)){
                    piece = bb_piece;    
                }
            
            }

            if(piece == -1) printf(" . ");
            else printf(" %s ", unicode_pieces[piece]);
        }
        printf("\n");
    }

    printf("\n     A  B  C  D  E  F  G  H\n\n");
    printf("     Bitboard: %llud\n\n", bitboards[0]);
    printf("     Side: %s\n", side == white ? "White" : "Black");
    printf("     Enpassant: %s\n", enpassant == no_sqr ? "no square" : square_to_coordinates[enpassant]);
    printf("     Castling:  %c%c%c%c\n\n", (castle & wk) ? 'K' : '-',
                                           (castle & wq) ? 'Q' : '-',
                                           (castle & bk) ? 'k' : '-',
                                           (castle & bq) ? 'q' : '-');
    
}

U64 mask_pawn_attacks(int side, int square){

    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, square);
    
    if (!side){
        if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
        if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    }
    else{
        if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
        if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);    
    }

    return attacks;
}

U64 mask_knight_attacks(int square){
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, square);

    if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);
    if ((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file) attacks |= (bitboard << 15);
    if ((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file) attacks |= (bitboard << 6);

    return attacks;
}

U64 mask_king_attacks(int square){
    U64 attacks = 0ULL;
    U64 bitboard = 0ULL;

    set_bit(bitboard, square);

    if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if ((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);
    if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if ((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);
    if ((bitboard >> 8)) attacks |= (bitboard >> 8);
    if ((bitboard << 8)) attacks |= (bitboard << 8);

    return attacks;
}

U64 mask_bishop_attacks(int square){
    U64 attacks = 0ULL;
    int init_rank, init_file;

    int target_rank = square / 8;
    int target_file = square % 8;
    
    // mask relevant bishop occupancy bits
    for (init_rank = target_rank + 1, init_file = target_file + 1; init_rank <= 6 && init_file <= 6; init_rank++, init_file++) attacks |= (1ULL << (init_rank * 8 + init_file));
    for (init_rank = target_rank - 1, init_file = target_file + 1; init_rank >= 1 && init_file <= 6; init_rank--, init_file++) attacks |= (1ULL << (init_rank * 8 + init_file));
    for (init_rank = target_rank + 1, init_file = target_file - 1; init_rank <= 6 && init_file >= 1; init_rank++, init_file--) attacks |= (1ULL << (init_rank * 8 + init_file));
    for (init_rank = target_rank - 1, init_file = target_file - 1; init_rank >= 1 && init_file >= 1; init_rank--, init_file--) attacks |= (1ULL << (init_rank * 8 + init_file));
    
    // return attack map
    return attacks;
}

U64 mask_rook_attacks(int square){
    U64 attacks = 0ULL;

    int initial_rank, initial_file;
    
    // init target rank & files
    int target_rank = square / 8;
    int target_file = square % 8;
    
    // mask relevant bishop occupancy bits
    for (initial_rank = target_rank + 1; initial_rank <= 6; initial_rank++) attacks |= (1ULL << (initial_rank * 8 + target_file));
    for (initial_rank = target_rank - 1; initial_rank >= 1; initial_rank--) attacks |= (1ULL << (initial_rank * 8 + target_file));
    for (initial_file = target_file + 1; initial_file <= 6; initial_file++) attacks |= (1ULL << (target_rank * 8 + initial_file));
    for (initial_file = target_file - 1; initial_file >= 1; initial_file--) attacks |= (1ULL << (target_rank * 8 + initial_file));
    
    // return attack map
    return attacks;
}

U64 bishop_attacks_on_the_fly(int square, U64 block){
    U64 attacks = 0ULL;
    int init_rank, init_file;

    int target_rank = square / 8;
    int target_file = square % 8;
    
    // mask relevant bishop occupancy bits
    for (init_rank = target_rank + 1, init_file = target_file + 1; init_rank <= 7 && init_file <= 7; init_rank++, init_file++) {
        attacks |= (1ULL << (init_rank * 8 + init_file));
        if ((1ULL << (init_rank * 8 + init_file)) & block ) break;
    }
    for (init_rank = target_rank - 1, init_file = target_file + 1; init_rank >= 0 && init_file <= 7; init_rank--, init_file++) {
        attacks |= (1ULL << (init_rank * 8 + init_file));
        if ((1ULL << (init_rank * 8 + init_file)) & block ) break;
    }
    for (init_rank = target_rank + 1, init_file = target_file - 1; init_rank <= 7 && init_file >= 0; init_rank++, init_file--) {
        attacks |= (1ULL << (init_rank * 8 + init_file));
        if ((1ULL << (init_rank * 8 + init_file)) & block ) break;
    }
    for (init_rank = target_rank - 1, init_file = target_file - 1; init_rank >= 0 && init_file >= 0; init_rank--, init_file--) {
        attacks |= (1ULL << (init_rank * 8 + init_file));
        if ((1ULL << (init_rank * 8 + init_file)) & block ) break;
    }
    
    // return attack map
    return attacks;
}

void init_leapers_attacks()
{
    // loop over 64 board squares
    for (int square = 0; square < 64; square++)
    {
        // init pawn attacks
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);
        
        // init knight attacks
        knight_attacks[square] = mask_knight_attacks(square);
        
        // init king attacks
        king_attacks[square] = mask_king_attacks(square);
    }
}

U64 rook_attacks_on_the_fly(int square, U64 block){
    U64 attacks = 0ULL;

    int initial_rank, initial_file;
    
    // init target rank & files
    int target_rank = square / 8;
    int target_file = square % 8;
    
    // mask relevant bishop occupancy bits
    for (initial_rank = target_rank + 1; initial_rank <= 7; initial_rank++) {
        attacks |= (1ULL << (initial_rank * 8 + target_file));
        if ((1ULL << (initial_rank * 8 + target_file)) & block ) break;
    }
    for (initial_rank = target_rank - 1; initial_rank >= 0; initial_rank--) {
        attacks |= (1ULL << (initial_rank * 8 + target_file));
        if ((1ULL << (initial_rank * 8 + target_file)) & block ) break;
    }
    for (initial_file = target_file + 1; initial_file <= 7; initial_file++) {
        attacks |= (1ULL << (target_rank * 8 + initial_file));
        if ((1ULL << (target_rank * 8 + initial_file)) & block ) break;
    }
    for (initial_file = target_file - 1; initial_file >= 0; initial_file--) {
        attacks |= (1ULL << (target_rank * 8 + initial_file));
        if ((1ULL << (target_rank * 8 + initial_file)) & block ) break;
    }
    
    return attacks;
}

 int count_bits(U64 bitboard){
    int count = 0;
    while(bitboard){
        count++;
        bitboard &= bitboard - 1;
    }
    return count;
}

int get_less_significant_bit_index(U64 bitboard){
    if(bitboard == 0) return -1;
    return count_bits((bitboard & -bitboard) - 1);
}

/*
    La funzione set_occupancy prende in input un indice, number_of_bits_in_mask che tiene conto del
    numero di bits presenti nella maschera d'attacco del pezzo corrispondente (count_bits(attack_mask)) e la maschera stessa.
    l'indice serve perchè la funzione verrà chiamata n volte, una per ogni possibile occupazione della maschera.

*/


U64 set_occupancy(int index, int number_of_bits_in_mask, U64 attack_mask){ 
    U64 occupancy = 0ULL;
    for(int count = 0; count < number_of_bits_in_mask; count++){
        int square = get_less_significant_bit_index(attack_mask);
        clear_bit(attack_mask, square);
        if(index & (1 << count)) set_bit(occupancy, square);
    }
    return occupancy;
}

unsigned int get_random_number(){
    unsigned int number = state;
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    state = number;
    return state;
}

U64 get_random_U64_number(){
    U64 n1, n2, n3, n4;

    n1 = (U64)(get_random_number()) & 0xFFFF;
    n2 = (U64)(get_random_number()) & 0xFFFF;
    n3 = (U64)(get_random_number()) & 0xFFFF;
    n4 = (U64)(get_random_number()) & 0xFFFF;

    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

U64 generate_magic_number_candidate(){
    return get_random_U64_number() & get_random_U64_number() & get_random_U64_number();
}

U64 find_magic_number(int square, int relevant_bits, int bishop){
    U64 occupancies[4096]; //Massimo numero di occupancies possibili per una maschera di 64 bit (4096 è il massimo per ora a causa del max numero di relevant bits per una maschera di attacco della torre)
    U64 attacks[4096]; 
    U64 used_attacks[4096];
    memset(used_attacks, 0ULL, sizeof(used_attacks)); //Imposta tutti gli attacchi usati a 0
    U64 attack_mask = bishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);
    int occupancy_indicies = 1 << relevant_bits; //serve per impostare index in set_occupancy = 2^relevant_bits

    for(int i = 0; i < occupancy_indicies; i++){
        //Calcola prima le occupanze (che sarebbero tutte le combinazioni di pezzi che possono bloccare l'attacco del pezzo in base alla maschera di attacco)
        occupancies[i] = set_occupancy(i, relevant_bits, attack_mask);
        //Calcola tutti i possibili attacchi per ogni occupazione della maschera in base al pezzo e alla casella  
        attacks[i] = bishop ? bishop_attacks_on_the_fly(square, occupancies[i]) : rook_attacks_on_the_fly(square, occupancies[i]);  
    }

    for(int random_count = 0; random_count < 100000000; random_count++){
        U64 magic_number = generate_magic_number_candidate();
        //l'if serve per capire se il magic_number va bene o meno 
        if(count_bits((attack_mask * magic_number) & 0xFF00000000000000ULL) < 6) continue;


        int index, fail_flag;

        for(index = 0, fail_flag = 0; !fail_flag && index < occupancy_indicies; index++){
            int magic_index = (int)((occupancies[index] * magic_number) >> (64 - relevant_bits));
            if(used_attacks[magic_index] == 0ULL) used_attacks[magic_index] = attacks[index];
            else if(used_attacks[magic_index] != attacks[index]) fail_flag = 1;
        }
        if (!fail_flag) return magic_number;  
    }
    printf("Magic number fails");
    return 0ULL;
}

void init_sliders_attacks(int bishop){
    for(int square = 0; square < 64; square++){
        bishop_masks[square] = mask_bishop_attacks(square);
        rook_masks[square] = mask_rook_attacks(square);

        U64 attack_mask = bishop ? bishop_masks[square] : rook_masks[square];

        int relevant_bits_count = count_bits(attack_mask);
        int occupancy_indicies = (1 << relevant_bits_count);

        for(int index = 0; index < occupancy_indicies; index++){
            if(bishop){
                U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);
                int magic_index = ((occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]));
                bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            }
            else{
                U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);
                int magic_index = ((occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]));
                rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
            }
        }
    }
}

U64 get_bishop_attacks(int square, U64 occupancy){
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64 - bishop_relevant_bits[square];

    return bishop_attacks[square][occupancy];
}

U64 get_rook_attacks(int square, U64 occupancy){
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_relevant_bits[square];

    return rook_attacks[square][occupancy];
}

void parse_fen(const char *fen){
    memset(bitboards, 0ULL, sizeof(bitboards));
    memset(occupancies, 0ULL, sizeof(occupancies));
    side = white;
    enpassant = no_sqr;
    castle = 0;

    for(int rank = 0; rank < 8; rank++){
        for(int file = 0; file < 8; file++){
            int square = rank * 8 + file;
            if((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z')){
                int piece = char_pieces[*fen];
                set_bit(bitboards[piece], square);
                
                fen++;
            }
            if(*fen >= '0' && *fen <= '9'){
                int offset = *fen - '0';
                int piece = -1;
                for(int bb_piece = P; bb_piece <= k; bb_piece++){
                    if(get_bit(bitboards[bb_piece], square)){
                        piece = bb_piece;
                    }
                }
                if(piece == -1) file--;

                file += offset;
                fen++;
            }
            if(*fen == '/') *fen++;
        }
    }
    *fen++;
    (*fen == 'w') ? (side = white) : (side = black);
    fen += 2;
    while(*fen != ' '){
        switch(*fen){
            case 'K': castle |= wk; break;
            case 'Q': castle |= wq; break;
            case 'k': castle |= bk; break;
            case 'q': castle |= bq; break;
            case '-': castle = 0; break;
        }
        *fen++;
    }
    *fen++;
    if (*fen != '-'){
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');
        enpassant = rank * 8 + file;
    }
    else enpassant = no_sqr;
    
    for(int piece = P; piece <= K; piece++)
        occupancies[white] |= bitboards[piece];
    
    for(int piece = p; piece <= k; piece++)
        occupancies[black] |= bitboards[piece];

    occupancies[both] = occupancies[white] | occupancies[black];
}

U64 get_queen_attacks(int square, U64 occupancy){
    return get_bishop_attacks(square, occupancy) | get_rook_attacks(square, occupancy);
}