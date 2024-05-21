#ifndef DEFINE_H
#define DEFINE_H

//---------- DEFINES ----------//

#define U64 unsigned long long
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))

//---------- MACROS ----------//

#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))

//---------- FUNCTIONS ----------//

extern void print_bitboard(U64 bitboard);

#endif