#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

static unsigned int trailing_zeros(uint64_t n) {
    unsigned int i;
    for (i = 0; !((n >> i) & 1); ++i);
    return i;
}

struct magic_t {
    uint64_t M;
    int preshift;
    int postshift;
    int add;
};

struct magic_t get_magic(uint64_t d, unsigned int leading) {
    uint64_t signedmin, signedmax, delta, ones,
             nc, p, q1, q2, r1, r2, w = sizeof(uint64_t) * 8;

    struct magic_t ms;
    ms.add = 0;

    ones = (uint64_t) (~0) >> leading;
    signedmin = INT64_MIN;
    signedmax = INT64_MAX;

    nc = ones - ((ones + 1 - d) % d);
    assert(nc % d == d - 1);

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
            if (q2 >= signedmax)
                ms.add = 1;
            q2 = (q2 << 1) | 1;
            r2 = ((r2 << 1) | 1) - d;
        } else {
            if (q2 >= signedmin)
                ms.add = 1;
            q2 <<= 1;
            r2 = (r2 << 1) | 1;
        }

        ++p;
        delta = d - 1 - r2;
    } while (p < (w * 2) && (q1 < delta || (q1 == delta && r1 == 0)));

    if (ms.add && !(d & 1)) {
        unsigned int preshift = trailing_zeros(d);
        ms = get_magic(d << preshift, leading + preshift);
        assert(ms.add == 0 && ms.preshift == 0);
        ms.preshift = preshift;
        return ms;
    }

    ms.M = q2 + 1;
    ms.postshift = p - w;
    ms.preshift = 0;

    if (ms.add)
        --ms.postshift;

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

    struct magic_t ms = get_magic(divisor, 0);
    printf("%lx: Magic: %lx | Preshift: %d | Postshift: %d\n", divisor, ms.M, ms.preshift, ms.postshift);
    
    return 1;
}
