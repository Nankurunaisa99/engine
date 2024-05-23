#include <stdio.h>
#include "define.h"

//---------- MAIN ----------//

int main(){
    init_leapers_attacks();

    printf("\nrandom number = %ud\n", get_random_U64_numbers());
    printf("random number = %ud\n", get_random_U64_numbers());
    printf("random number = %ud\n", get_random_U64_numbers());
    printf("random number = %ud\n", get_random_U64_numbers());
    printf("random number = %ud\n", get_random_U64_numbers());

    return 0;

}