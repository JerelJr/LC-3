/* Binary Operations */
#ifndef BINOPS_H
#define BINOPS_H

#include <stdint.h>
#include "flags.h"
#include "registers.h"

/* Register Storage */
uint16_t reg[R_COUNT];

uint16_t sign_extend(uint16_t x, int bit_count);

/* switch a 16-bit integer between big endian and little endian */
uint16_t swap16(uint16_t x);

void update_flags(uint16_t r);

#endif