
#include <stdio.h>
#include <stdint.h>

// Структура ключей RSA
typedef struct {
    int p;
    int q;
    int n;
    int phi;
    int e;
    int d;
} rsa_key_t;

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

// Вычисление обратного элемента
int mod_inverse(int a, int mod) {
    int x, y;

    if (extended_gcd(a, mod, &x, &y) != 1) {
        return -1;
    }

    return (x % mod + mod) % mod;
}

// Генерация ключей RSA
int generate_keys(rsa_key_t *key, int p, int q, int d) {
    key->p = p;
    key->q = q;

    key->n = p * q;
    key->phi = (p - 1) * (q - 1);

    key->d = d;
    key->e = mod_inverse(d, key->phi);

    return key->e != -1;
}

// Шифрование открытым ключом
int rsa_encrypt(int message, const rsa_key_t *key) {
    return mod_pow(message, key->e, key->n);
}

// Расшифрование секретным ключом
int rsa_decrypt(int encrypted, const rsa_key_t *key) {
    return mod_pow(encrypted, key->d, key->n);
}

// Вывод параметров ключа
void print_keys(const char *name, const rsa_key_t *key) {
    printf("\nSubscriber %s:\n", name);
    printf("p = %d, q = %d\n", key->p, key->q);
    printf("n = %d\n", key->n);
    printf("phi(n) = %d\n", key->phi);
    printf("Public key:  (e=%d, n=%d)\n", key->e, key->n);
    printf("Private key: (d=%d, n=%d)\n", key->d, key->n);
}

int main(void) {
    rsa_key_t A, B;

    // Генерация ключей двух абонентов
    if (!generate_keys(&A, 131, 227, 3) ||
        !generate_keys(&B, 113, 281, 3)) {
        printf("Key generation error!\n");
        return 1;
    }

    print_keys("A", &A);
    print_keys("B", &B);

    // Выбор получателя сообщения
    char recipient;

    printf("\nChoose recipient (A/B): ");

    if (scanf(" %c", &recipient) != 1) {
        return 1;
    }

    const rsa_key_t *key;

    if (recipient == 'A' || recipient == 'a') {
        key = &A;
    } else if (recipient == 'B' || recipient == 'b') {
        key = &B;
    } else {
        printf("Invalid recipient!\n");
        return 1;
    }

    // Ввод сообщения
    int message;

    printf("Enter message (0..%d): ", key->n - 1);

    if (scanf("%d", &message) != 1 ||
        message < 0 || message >= key->n) {
        printf("Invalid message!\n");
        return 1;
    }

    // Шифрование сообщения
    int encrypted = rsa_encrypt(message, key);

    // Расшифрование сообщения получателем
    int decrypted = rsa_decrypt(encrypted, key);

    printf("\n--- RSA ---\n");
    printf("Original message: %d\n", message);
    printf("Encrypted message: %d\n", encrypted);
    printf("Decrypted message: %d\n", decrypted);

    if (message == decrypted) {
        printf("Success: message restored!\n");
    } else {
        printf("Error: decryption failed!\n");
        return 1;
    }

    return 0;
}
