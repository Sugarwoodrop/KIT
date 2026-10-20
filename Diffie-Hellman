
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define P 30803
#define G 2

int mod_pow(int base, int exp, int mod) {
    int64_t result = 1;
    int64_t b = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * b) % mod;
        }

        b = (b * b) % mod;
        exp /= 2;
    }

    return (int)result;
}

int main(void) {
    srand((unsigned)time(NULL));

    int X_A = 2 + rand() % (P - 3);
    int X_B = 2 + rand() % (P - 3);

    int Y_A = mod_pow(G, X_A, P);
    int Y_B = mod_pow(G, X_B, P);

    int Z_A = mod_pow(Y_B, X_A, P);
    int Z_B = mod_pow(Y_A, X_B, P);

    printf("p = %d, g = %d\n\n", P, G);

    printf("Participant A:\n");
    printf("Secret key X_A = %d\n", X_A);
    printf("Public key Y_A = %d\n\n", Y_A);

    printf("Participant B:\n");
    printf("Secret key X_B = %d\n", X_B);
    printf("Public key Y_B = %d\n\n", Y_B);

    printf("Shared key A = %d\n", Z_A);
    printf("Shared key B = %d\n", Z_B);

    if (Z_A == Z_B) {
        printf("\nSuccess: shared keys match!\n");
    } else {
        printf("\nError: shared keys do not match!\n");
        return 1;
    }

    return 0;
}
