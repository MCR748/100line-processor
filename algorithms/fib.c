#ifdef __x86_64__
#include <stdio.h>
#endif

// Function prototype declaration
int fibonacci(int n);

int main() {
    // Conditional compilation based on the target architecture
#ifdef __x86_64__
    int tenth_fibonacci = fibonacci(10);
    printf("The 10th Fibonacci number is: %d\n", tenth_fibonacci);
#elif defined(__riscv)
    // No action needed for RISC-V
    int tenth_fibonacci = fibonacci(10);
#else
#error "Unsupported architecture"
#endif

    return 0;
}

// Implementation of the Fibonacci function using a for loop
int fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        int a = 0;
        int b = 1;
        int temp;
        for (int i = 2; i <= n; i++) {
            temp = a;
            a = b;
            b = temp + b;
        }
        return b;
    }
}
