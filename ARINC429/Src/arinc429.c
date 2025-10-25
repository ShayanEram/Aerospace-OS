#include "arinc429.h"

uint32_t arinc429_encode(ARINC429_Word w) {
    uint32_t word = 0;

    word |= (w.label & 0xFF);

    word |= ((w.sdi & 0x3) << 8);

    word |= ((w.data & 0x7FFFF) << 10);

    word |= ((w.ssm & 0x3) << 29);

    uint32_t temp = word;
    int ones = 0;
    for (int i = 0; i < 31; i++) {
        if (temp & (1u << i)) ones++;
    }
    bool parity = (ones % 2 == 0);
    if (parity) {
        word |= (1u << 31);
    }

    return word;
}

ARINC429_Word arinc429_decode(uint32_t raw) {
    ARINC429_Word w;

    w.label = raw & 0xFF;
    w.sdi   = (raw >> 8) & 0x3;
    w.data  = (raw >> 10) & 0x7FFFF;
    w.ssm   = (raw >> 29) & 0x3;

    bool parity_bit = (raw >> 31) & 0x1;

    int ones = 0;
    for (int i = 0; i < 31; i++) {
        if (raw & (1u << i)) 
            ones++;
    }
    bool expected_parity = (ones % 2 == 0);
    w.parity = (expected_parity == parity_bit);

    return w;
}