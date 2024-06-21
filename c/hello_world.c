
#ifdef __x86_64__
#include <stdio.h>
#endif

int main() {

#ifdef __x86_64__
    printf("Hello, World!\n");
#endif

    return 0;
}