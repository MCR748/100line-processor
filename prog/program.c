#include "user_func.h"

extern int __din_start;
extern int __dout_start;

int main() {

    int *din = &__din_start;
    int *dout = &__dout_start;
    
    int num_outputs = user_func(din, dout);

    // return the number of outputs as exit code (for bare metal, this ends up in register a0)
    return num_outputs;
}
