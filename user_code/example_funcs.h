
int fibonacci(int n);
void first_n_fibonacci(int n, int* output);
int sum_numbers(int n);

int fibonacci(int n) {

    if (n == 0 || n == 1) return n;

    int a = 0, b = 1, tmp;

    for (int i = 1; i < n; i++) {
        tmp = b;
        b = a + b;
        a = tmp;
    }

    return b;
}

void first_n_fibonacci(int n, int* output) {

    output[0] = 0;
    output[1] = 1;

    for (int i = 2; i <= n; i++) {
        output[i] = output[i-1] + output[i-2];
    }
}

int sum_numbers(int n) {

    int sum = 0;
    int i;

    for (i = 1; i <= n; i++) {
        sum += i;
    }

    return sum;
}