/**
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 */

#ifndef SIMDCompressionAndIntersection_MERSENNE_H_
#define SIMDCompressionAndIntersection_MERSENNE_H_

#include "common.h"
#include "util.h"

/**
 *  Mersenne twister - random number generator.
 *  Generate uniform distribution of 32 bit integers with the MT19937 algorithm.
 * source: http://bannister.us/weblog/?s=Mersenne
 */
class ZRandom {

public:
    enum {
        N = 624, M = 397
    };
    unsigned int MT[N + 1];
    unsigned int *map[N];
    int nValues;

    ZRandom(unsigned int iSeed = 20070102);
    void seed(unsigned iSeed);
    unsigned int getValue();
    unsigned int getValue(const uint32_t MaxValue);
    double getDouble();
    bool test(const double p);

};

ZRandom::ZRandom(unsigned iSeed) :
    nValues(0) {
    seed(iSeed);
}

void ZRandom::seed(unsigned iSeed) {
    nValues = 0;
    // Seed the array used in random number generation.
    MT[0] = iSeed;
    for (int i = 1; i < N; ++i) {
        MT[i] = 1 + (69069 * MT[i - 1]);
    }
    // Compute map once to avoid % in inner loop.
    for (int i = 0; i < N; ++i) {
        map[i] = MT + ((i + M) % N);
    }
}

inline bool ZRandom::test(const double p) {
    return getDouble() <= p;
}
inline double ZRandom::getDouble() {
    return double(getValue()) * (1.0 / 4294967296.0);
}

unsigned int ZRandom::getValue(const uint32_t MaxValue) {
    unsigned int used = MaxValue;
    used |= used >> 1;
    used |= used >> 2;
    used |= used >> 4;
    used |= used >> 8;
    used |= used >> 16;

    // Draw numbers until one is found in [0,n]
    unsigned int i;
    do
        i = getValue() & used; // toss unused bits to shorten search
    while (i > MaxValue);
    return i;
}

unsigned int ZRandom::getValue() {
    if (0 == nValues) {
        MT[N] = MT[0];
        for (int i = 0; i < N; ++i) {
            unsigned y = (0x80000000 & MT[i]) | (0x7FFFFFFF
                                  & MT[i + 1]);
            unsigned v = *(map[i]) ^ (y >> 1);
            if (1 & y)
                v ^= 2567483615;
            MT[i] = v;
        }
        nValues = N;
    }
    unsigned y = MT[N - nValues--];
    y ^= y >> 11;
    y ^= static_cast<unsigned int>((y << 7) & 2636928640);
    y ^= static_cast<unsigned int>((y << 15) & 4022730752);
    y ^= y >> 18;
    return y;
}

#endif /* SIMDCompressionAndIntersection_MERSENNE_H_ */
