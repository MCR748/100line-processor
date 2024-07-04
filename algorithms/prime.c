#include <stdio.h>
#include <stdlib.h>

#define MAX 100  // Upper limit of the numbers checkecd

// Function prototype declaration
void sieve_of_eratosthenes(int n, int num_primes, int *primes);

int main() {
    int n = MAX;
    int num_primes = 10; // Number of primes to find
    int primes[10]; // Integer array to store the primes

#ifdef __x86_64__
    sieve_of_eratosthenes(n, num_primes, primes);
    printf("The first %d prime numbers below %d are:\n", num_primes, n);
    for (int i = 0; i < num_primes; i++) {
        printf("%d ", primes[i]);
    }
    printf("\n");
#elif defined(__riscv)
    sieve_of_eratosthenes(n, num_primes, primes);
    uintptr_t arraySize = sizeof(primes);
    uintptr_t arrayPointer = (uintptr_t)primes;
#else
#error "Unsupported architecture"
#endif
    return 0;
}

// Implementation of sieve_of_eratosthenes
void sieve_of_eratosthenes(int n, int num_primes, int *primes) {
    int prime[MAX + 1];
    int count = 0;

    // Initialize all entries as true (1).
    for (int i = 0; i <= n; i++) {
        prime[i] = 1;
    }

    for (int p = 2; p * p <= n; p++) {
        if (prime[p] == 1) {
            for (int i = p * p; i <= n; i += p) {
                prime[i] = 0;
            }
        }
    }

    for (int p = 2, j = 0; p <= n && j < num_primes; p++) {
        if (prime[p] == 1) {
            primes[j++] = p; // Store the prime number in the passed array
        }
    }
}
