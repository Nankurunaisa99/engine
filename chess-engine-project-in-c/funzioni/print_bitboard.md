# print\_bitboard

* U64 bitboard -> Rappresentazione in numero a 64bit della scacchiera

{% code lineNumbers="true" fullWidth="true" %}
```c
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
```
{% endcode %}

La funzione <mark style="color:purple;">`print_bitboard`</mark> prende come argomento un [U64](../definizioni/u64.md) bitboard e, in modo molto semplice, stampa a video la scacchiera insieme agli indici di riga e di colonna.\
La scacchiera mostrerà degli 0 nelle caselle dove non sono presenti dei pezzi, e degli 1 viceversa.



<div align="center" data-full-width="false">

<figure><img src="../../.gitbook/assets/bitboard 0.PNG" alt="" width="452"><figcaption><p>Rapprezentazione della scacchiera vuota</p></figcaption></figure>

</div>

<div align="center">

<figure><img src="../../.gitbook/assets/bitboard generica.PNG" alt="" width="436"><figcaption><p>Rappresentazione della scacchiera con dei pezzi casuali</p></figcaption></figure>

</div>

{% hint style="info" %}
Nell'ultimo caso, 45222190 sarà la rappresentazione decimale del numero a 64bit identificato dalla nostra tastiera (la tastiera vuota sarà semplicemente 0ULL)
{% endhint %}
