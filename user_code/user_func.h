
void user_func(int* din, int* dout) {

    int n = *din;
    int sum = 0;
    int i;

    for (i = 1; i <= n; i++) {
        sum += i;
        sum += 1;
    }

    *dout = sum;
}