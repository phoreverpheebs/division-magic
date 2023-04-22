#include <stdio.h>
#include <stdlib.h>

#include "division-magic.h"

int main(int argc, char **argv) {
    int base;
    uint64_t divisor;
    char *input;

    if (argc < 2) {
        printf("Provide a constant divisor to calculate a magic reciprocal for\n");
        exit(1);
    }

    base = 10;
    input = argv[1];
    if (input[0] == '0' || input[1] == 'x') {
        input += 2;
        base = 16;
    } else if (input[0] == '0' || input[1] == 'b') {
        input += 2;
        base = 2;
    }

    divisor = strtoul(input, NULL, base);

    if (divisor == 0) divisor++;

    struct magic_t ms = get_magic(divisor, 0);
    printf("%lx: Magic: %lx ", divisor, ms.M);
    if (ms.preshift)
        printf("| Preshift: %d ", ms.preshift);
    printf("| Postshift: %d\n", ms.postshift);
    
    return 1;
}
