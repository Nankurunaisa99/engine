#ifndef DEFINE_H
#define DEFINE_H

//---------- DEFINES ----------//

#define U64 unsigned long long
#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

//---------- MACROS ----------//

#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define clear_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

/*
          binary move bits                               hexidecimal constants
    
    0000 0000 0000 0000 0011 1111    source square       0x3f
    0000 0000 0000 1111 1100 0000    target square       0xfc0
    0000 0000 1111 0000 0000 0000    piece               0xf000
    0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
    0001 0000 0000 0000 0000 0000    capture flag        0x100000
    0010 0000 0000 0000 0000 0000    double push flag    0x200000
    0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
    1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/

#define encode_move(source, target, piece, promoted, capture, double, enpassant, castling) (source) | (target << 6) | (piece << 12) | (promoted << 16) | (capture << 20) | (double << 21) | (enpassant << 22) | (castling << 23)
#define get_move_source(move) (move & 0x3f)
#define get_move_target(move) ((move & 0xfc0) >> 6)
#define get_move_piece(move) ((move & 0xf000) >> 12)
#define get_move_promoted(move) ((move & 0xf0000) >> 16)
#define get_move_capture(move) (move & 0x100000)
#define get_move_double(move) (move & 0x200000)
#define get_move_enpassant(move) (move & 0x400000)
#define get_move_castling(move) (move & 0x800000)


//---------- ENUM ----------//

enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sqr
};

enum { white, black, both };

enum { rook, bishop };

enum { wk = 1, wq = 2, bk = 4, bq = 8 }; //l'arrocco verrà rappresentato da un numero a 4 bit dove ogni bit indica dove è possibile effettuarlo

enum { P, N, B, R, Q, K, p, n, b, r, q, k };

//---------- STRUCTS ----------//

typedef struct {
    int moves[256];
    int count;
} moves;

//---------- GLOBALS ----------//

extern const char *square_to_coordinates[];
extern const char promoted_pieces[];
extern U64 pawn_attacks[2][64];
extern U64 knight_attacks[64];
extern U64 king_attacks[64];
extern const U64 not_a_file;
extern const U64 not_h_file;
extern const U64 not_hg_file;
extern const U64 not_ab_file;
extern const int bishop_relevant_bits[64];
extern const int rook_relevant_bits[64];
extern const U64 rook_magic_numbers[64];
extern const U64 bishop_magic_numbers[64];
extern unsigned int state;
extern U64 bishop_attacks[64][512];
extern U64 rook_attacks[64][4096];
extern U64 bishop_masks[64];
extern U64 rook_masks[64];
extern U64 bitboards[12];
extern U64 occupancies[3];
extern int side;
extern int enpassant;
extern int castle;
extern char ascii_pieces[12];
extern char *unicode_pieces[12];
extern int char_pieces[];

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
extern int get_less_significant_bit_index(U64 bitboard);
extern U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask);
extern void init_leapers_attacks();
extern unsigned int get_random_number();
extern U64 get_random_U64_number();
extern U64 generate_magic_number_candidate();
extern U64 find_magic_number(int square, int relevant_bits, int bishop);
extern void init_sliders_attacks(int bishop);
extern U64 get_bishop_attacks(int square, U64 occupancy);
extern U64 get_rook_attacks(int square, U64 occupancy);
extern U64 get_queen_attacks(int square, U64 occupancy);
extern void print_board();
extern void parse_fen(const char *fen);
extern int is_square_attacked(int square, int side);
extern void print_attacked_squares(int side);
extern void generate_moves();
extern void print_move(int move);
extern void print_move_list(moves *move_list);
extern void add_move(moves *move_list, int move);

#endif