
#include "example_funcs.h"

int user_func(int* din, int* dout) {

    int n = *din;
    
    // Functions with a single returned output
    // int out = fibonacci(n);
    // *dout = out;

    // Functions with a returned array
    first_n_fibonacci(n, dout);

    // Return the number of outputs of the function
    int num_outputs = n + 1;

    return num_outputs;
}
