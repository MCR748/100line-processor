#include <stdio.h>
#include <stdint.h>

// Function prototype declaration
void fibonacci(int n, int* fibArray);

int main() {
#ifdef __x86_64__
    int fibNumbers[10]; // Array to hold the first 10 Fibonacci numbers
    fibonacci(10, fibNumbers); // Call the modified fibonacci function
    
    printf("The first 10 Fibonacci numbers are:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", fibNumbers[i]);
    }
    printf("\n");
#elif defined(__riscv)
    // Similar implementation for RISC-V if needed
    int fibNumbers[10]; // Array to hold the first 10 Fibonacci numbers
    fibonacci(10, fibNumbers); // Call the modified fibonacci function
    
    // Output array size and pointer to new variables
    uintptr_t arraySize = sizeof(fibNumbers);
    uintptr_t arrayPointer = (uintptr_t)fibNumbers;   
#else
#error "Unsupported architecture"
#endif
    return 0;
}

// Implementation of the Fibonacci function
void fibonacci(int n, int* fibArray) {
    fibArray[0] = 0; // Base case
    fibArray[1] = 1; // Base case
    for (int i = 2; i < n; i++) {
        fibArray[i] = fibArray[i - 1] + fibArray[i - 2];
    }
}
