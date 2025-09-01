#include "user_func.h"

extern int __din_start;
extern int __dout_start;

int main() {

    int *din = &__din_start;
    int *dout = &__dout_start;
    
    user_func(din, dout);

    // return the sum as exit code (for bare metal, this ends up in register a0)
    return 1;
}
