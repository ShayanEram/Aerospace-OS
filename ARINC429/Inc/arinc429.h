#ifndef LIB_ARINC429_INC_ARINC429_H
#define LIB_ARINC429_INC_ARINC429_H

#include <stdint.h>
#include <stdbool.h>

// ARINC 429 word representation
typedef struct {
    uint8_t label;       // 8 bits
    uint8_t sdi;         // 2 bits
    uint32_t data;       // 19 bits (we'll store in 32 for convenience)
    uint8_t ssm;         // 2 bits
    bool parity;         // 1 bit
} ARINC429_Word;

/**
 * Encode to arinc-429
 */
uint32_t arinc429_encode(ARINC429_Word w);

/**
 * Decode from arinc-429
 */
ARINC429_Word arinc429_decode(uint32_t raw);

#endif /* LIB_ARINC429_INC_ARINC429_H */