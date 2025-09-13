
int algorithm(int n, int* output);

#if defined(ALGO_FIBONACCI)

int algorithm(int n, int* output) {
    // Return first n fibonacci numbers

    output[0] = 0;
    output[1] = 1;

    for (int i = 2; i <= n; i++) {
        output[i] = output[i-1] + output[i-2];
    }

    return n + 1;   // No. of outputs
}

#elif defined(ALGO_PASCAL)

int algorithm(int n, int* output) {
    // Returns nth row of the triangle

    output[0] = 1;  // 0th row
    
    int i, j;
    for (i = 1; i <= n; i++) {
        output[i] = 1;

        // To use the same array, calculate outputs from end to start
        for (j = i-1; j > 0; j--) {
            output[j] = output[j] + output[j-1];
        }
    }

    return n + 1;
}

#elif defined(ALGO_PRIMES)

int algorithm(int n, int* output) {
    // Returns prime numbers below n using the sieve of erastothenes

    int primesieve[n+1];
    int count = 0;
    int i, j;

    for (i = 2; i <= n; i++) primesieve[i] = 1;

    for (i = 2; i <= n; i++) {
        if (primesieve[i] == 1) {
            output[count++] = i;
            for (j = i+i; j <= n; j+=i) primesieve[j] = 0;
        }
    }

    return count;
}

#endif