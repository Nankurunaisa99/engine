#ifndef DEFINE_H
#define DEFINE_H

//---------- DEFINES ----------//

#define U64 unsigned long long

//---------- MACROS ----------//

#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define clear_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

//---------- ENUM ----------//

enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};

enum { white, black };


//---------- FUNCTIONS ----------//

extern void print_bitboard(U64 bitboard);
extern U64 mask_pawn_attacks(int side, int square);
extern U64 mask_knight_attacks(int square);
extern U64 mask_king_attacks(int square);
extern U64 mask_bishop_attacks(int square);
extern U64 mask_rook_attacks(int square);
extern U64 bishop_attacks_on_the_fly(int square, U64 block);
extern U64 rook_attacks_on_the_fly(int square, U64 block);
extern int count_bits(U64 bitboard);

#endif