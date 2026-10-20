#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define P 30803

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

int gcd(int a, int b) {
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }

    return a;
}

int extended_gcd(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }

    int x1, y1;
    int d = extended_gcd(b, a % b, &x1, &y1);

    *x = y1;
    *y = x1 - (a / b) * y1;

    return d;
}

int mod_inverse(int a, int mod) {
    int x, y;

    if (extended_gcd(a, mod, &x, &y) != 1) {
        return -1;
    }

    x %= mod;

    if (x < 0) {
        x += mod;
    }

    return x;
}

int generate_key(void) {
    int c;

    do {
        c = 2 + rand() % (P - 3);
    } while (gcd(c, P - 1) != 1);

    return c;
}

int main(void) {
    srand((unsigned)time(NULL));

    int m;

    printf("Enter message (1..%d): ", P - 1);

    if (scanf("%d", &m) != 1 || m < 1 || m >= P) {
        printf("Invalid message!\n");
        return 1;
    }

    int c_A = generate_key();
    int c_B = generate_key();

    int d_A = mod_inverse(c_A, P - 1);
    int d_B = mod_inverse(c_B, P - 1);

    if (d_A == -1 || d_B == -1) {
        printf("Key generation error!\n");
        return 1;
    }

    printf("\np = %d\n", P);
    printf("A: c_A = %d, d_A = %d\n", c_A, d_A);
    printf("B: c_B = %d, d_B = %d\n\n", c_B, d_B);

    int x1 = mod_pow(m, c_A, P);
    printf("A -> B: x1 = %d\n", x1);

    int x2 = mod_pow(x1, c_B, P);
    printf("B -> A: x2 = %d\n", x2);

    int x3 = mod_pow(x2, d_A, P);
    printf("A -> B: x3 = %d\n", x3);

    int decrypted = mod_pow(x3, d_B, P);

    printf("\nOriginal message: %d\n", m);
    printf("Decrypted message: %d\n", decrypted);

    if (m == decrypted) {
        printf("Success: message restored!\n");
    } else {
        printf("Error: messages do not match!\n");
        return 1;
    }

    return 0;
}
