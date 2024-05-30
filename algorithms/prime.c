#ifdef __x86_64__
#include <stdio.h>
#endif

#define MAX 100  // Define a reasonable limit for the array size

int sieve_of_eratosthenes(int n) {
    // Array to store prime status
    int prime[MAX + 1];
    int count = 0;

    // Initialize all entries as true (1).
    for (int i = 0; i <= n; i++) {
        prime[i] = 1;
    }

    for (int p = 2; p * p <= n; p++) {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == 1) {
            // Update all multiples of p to not prime
            for (int i = p * p; i <= n; i += p) {
                prime[i] = 0;
            }
        }
    }

    for (int p = 2; p <= n; p++) {
        if (prime[p] == 1) {
            count = count + 1;
        }
    }
    return count;
}

int main() {
    int n = 50;
    

#ifdef __x86_64__
    int num = sieve_of_eratosthenes(n);
    printf("Amount of prime numbers below 50 is %d\n", num);
#elif defined(__riscv)
    int num = sieve_of_eratosthenes(n);
#else
#error "Unsupported architecture"
#endif

    return 0;
}
