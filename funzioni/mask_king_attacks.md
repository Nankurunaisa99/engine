# mask\_king\_attacks

* int square -> indica la posizione sulla scacchiera

{% code lineNumbers="true" %}
```c
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
```
{% endcode %}

Questa funzione genera i bit che possono essere attaccati dal re in qualsiasi momento, in base alla posizione sulla scacchiera (Il colore per il cavallo è irrilevante).

Partendo da una scacchiera vuota, viene posizionato il re sulla scacchiera e si fanno le stesse considerazioni che ho spiegato nel dettaglio nella funzione [`mask_pawn_attacks`](mask\_pawn\_attacks.md)

Unica cosa che cambia è che non teniamo traccia del colore del re poichè indifferente. (Immagini omesse per ridondanza)
