#include "algos.h"

int user_func(int* din, int* dout) {

    int n = *din;
    
    // --- Write your algorithm here ---

    // Functions with a single returned output
    // int out = fibonacci(n);
    // *dout = out;

    // Functions with a returned array
    int num_outputs = algorithm(n, dout);

    // Return the number of outputs of the function
    return num_outputs;
}
