
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define P 30803

// Быстрое возведение в степень по модулю
int mod_pow(int a, int n, int mod) {
    int64_t result = 1;
    int64_t base = a % mod;

    while (n > 0) {
        if (n % 2 == 1) {
            result = result * base % mod;
        }

        base = base * base % mod;
        n /= 2;
    }

    return (int)result;
}

// НОД
int gcd(int a, int b) {
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

// Расширенный алгоритм Евклида
int extended_gcd(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }

    int x1, y1;
    int g = extended_gcd(b, a % b, &x1, &y1);

    *x = y1;
    *y = x1 - (a / b) * y1;

    return g;
}

// Обратный элемент по модулю
int mod_inverse(int a, int mod) {
    int x, y;

    if (extended_gcd(a, mod, &x, &y) != 1) {
        return -1;
    }

    return (x % mod + mod) % mod;
}

// Генерация показателя шифрования
int generate_key(void) {
    int c;

    do {
        c = 2 + rand() % (P - 3);
    } while (gcd(c, P - 1) != 1);

    return c;
}

int main(void) {
    srand((unsigned)time(NULL));

    // Три карты
    int cards[3] = {101, 202, 303};
    const char *names[3] = {"alpha", "beta", "gamma"};

    // Генерация ключей
    int cA = generate_key();
    int cB = generate_key();

    int dA = mod_inverse(cA, P - 1);
    int dB = mod_inverse(cB, P - 1);

    printf("p = %d\n", P);
    printf("Alice: cA = %d, dA = %d\n", cA, dA);
    printf("Bob:   cB = %d, dB = %d\n\n", cB, dB);

    // ШАГ 1
    // Алиса шифрует карты и перемешивает их
    int order[3] = {0, 1, 2};

    for (int i = 2; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }

    int u[3];

    for (int i = 0; i < 3; i++) {
        u[i] = mod_pow(cards[order[i]], cA, P);
    }

    printf("Step 1: Alice sends three cards:\n");
    for (int i = 0; i < 3; i++) {
        printf("u%d = %d\n", i + 1, u[i]);
    }

    // ШАГ 2
    // Боб выбирает одну карту для Алисы
    int alice_index = rand() % 3;
    int selected = u[alice_index];

    printf("\nStep 2: Bob selects u%d = %d\n",
           alice_index + 1, selected);

    // Алиса расшифровывает свою карту
    int alice_card = mod_pow(selected, dA, P);

    printf("Alice receives card = %d\n", alice_card);

    // ШАГ 3
    // Боб шифрует оставшиеся две карты
    int v[2];
    int j = 0;

    for (int i = 0; i < 3; i++) {
        if (i != alice_index) {
            v[j++] = mod_pow(u[i], cB, P);
        }
    }

    // Случайная перестановка двух карт
    if (rand() % 2) {
        int temp = v[0];
        v[0] = v[1];
        v[1] = temp;
    }

    printf("\nStep 3: Bob sends:\n");
    printf("v1 = %d\n", v[0]);
    printf("v2 = %d\n", v[1]);

    // ШАГ 4
    // Алиса случайно выбирает карту для Боба
    int bob_index = rand() % 2;

    int w = mod_pow(v[bob_index], dA, P);

    printf("\nStep 4: Alice sends w = %d\n", w);

    // Боб окончательно расшифровывает карту
    int bob_card = mod_pow(w, dB, P);

    printf("Bob receives card = %d\n", bob_card);

    // Определение полученных карт
    int alice_id = -1;
    int bob_id = -1;
    int remaining_id = -1;

    for (int i = 0; i < 3; i++) {
        if (cards[i] == alice_card) {
            alice_id = i;
        } else if (cards[i] == bob_card) {
            bob_id = i;
        } else {
            remaining_id = i;
        }
    }

    if (alice_id == -1 || bob_id == -1 ||
        alice_id == bob_id || remaining_id == -1) {
        printf("\nError: invalid card distribution!\n");
        return 1;
    }

    printf("\n--- RESULT ---\n");
    printf("Alice: %s (%d)\n",
           names[alice_id], alice_card);
    printf("Bob: %s (%d)\n",
           names[bob_id], bob_card);
    printf("Remaining: %s (%d)\n",
           names[remaining_id], cards[remaining_id]);

    return 0;
}
