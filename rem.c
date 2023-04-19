#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct magic_t {
    uint64_t M;
    int s;
};

struct magic_t get_magic(uint64_t d) {
    uint64_t signedmin, signedmax, delta,
             nc, p, q1, q2, r1, r2, w = sizeof(uint64_t) * 8;

    struct magic_t ms;

    signedmin = INT64_MIN;
    signedmax = INT64_MAX;
    nc = ~0 - (~d + 1) % d;
    p = w - 1;
    q1 = signedmin / nc;
    r1 = signedmin % nc;
    q2 = signedmax / d;
    r2 = signedmax % d;

    do {
        if (r1 >= nc - r1) {
            q1 = (q1 << 1) | 1;
            r1 = (r1 << 1) - nc;
        } else {
            q1 <<= 1;
            r1 <<= 1;
        }

        if (r2 + 1 >= d - r2) {
            q2 = (q2 << 1) | 1;
            r2 = ((r2 << 1) | 1) - d;
        } else {
            q2 <<= 1;
            r2 = (r2 << 1) | 1;
        }

        ++p;
        delta = d - 1 - r2;
    } while (p < (w * 2) && (q1 < delta || (q1 == delta && r1 == 0)));

    ms.M = q2 + 1;
    ms.s = p - w;

    return ms;
}

int main(int argc, char **argv) {
    uint64_t divisor;

    if (argc < 2) {
        printf("Provide a constant divisor to calculate a magic reciprocal for\n");
        exit(1);
    }

    int base = 10;
    char *input = argv[1];
    if (input[0] == '0' || input[1] == 'x') {
        input += 2;
        base = 16;
    } else if (input[0] == '0' || input[1] == 'b') {
        input += 2;
        base = 2;
    }

    divisor = strtoul(input, NULL, base);

    if (divisor == 0) divisor++;

    struct magic_t ms = get_magic(divisor);
    printf("%lx: %lx | %d\n", divisor, ms.M, ms.s);
    
    return 1;
}
