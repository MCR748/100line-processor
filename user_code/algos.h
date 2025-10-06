#ifndef ALGOS_H
#define ALGOS_H

int algorithm(int* input, int* output);

#if defined(ALGO_FIBONACCI)

int algorithm(int* input, int* output) {
    // Return first n fibonacci numbers
    int n = *input;

    output[0] = 0;
    output[1] = 1;

    for (int i = 2; i <= n; i++) {
        output[i] = output[i-1] + output[i-2];
    }

    return n + 1;   // No. of outputs
}

#elif defined(ALGO_PASCAL)

int algorithm(int* input, int* output) {
    // Returns nth row of the triangle
    int n = *input;

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

int algorithm(int* input, int* output) {
    // Returns prime numbers below n using the sieve of erastothenes
    int n = *input;

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

#elif defined(ALGO_QUICKSORT)

void quicksort(int* arr, int low, int high) {

    int i, j, tmp;

    if (low < high) {
        // Partition array
        int pivot = arr[high];
        i = low - 1;

        for (j = low; j <= high; j++) {
            if (arr[j] <= pivot) {   // Swap
                i++;
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }

        // Recursive calls for each partition
        quicksort(arr, low, i-1);
        quicksort(arr, i+1, high);
    }
}

int algorithm(int* input, int* output) {
    // Quicksort on the given input

    // Take first input as size of array
    int n = input[0];
    int i, j;

    // Copy inputs to output array
    for (i = 1; i <= n; i++) {
        output[i-1] = input[i];
    }
    
    // Run quicksort
    quicksort(output, 0, n-1);

    return n;
}

#endif

#endif