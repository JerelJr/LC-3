#include "../include/binops.h"

/* fill in 0s for positive numbers and 1s for negative numbers */
uint16_t sign_extend(uint16_t x, int bit_count)
{
  if ((x >> (bit_count - 1)) & 1)
    {
      x |= (0xFFFF << bit_count);
    }

  return x;
}

/* switch a 16-bit integer between big endian and little endian */
uint16_t swap16(uint16_t x) { return (x << 8) | (x >> 8); }

/* update sign flags when a value is written to a register */ 
void update_flags(uint16_t r)
{
  if (reg[r] == 0)
    reg[R_COND] = FL_ZRO;
  else if (reg[r] >> 15)  //1 in the leftmost bit indicates negative
    reg[R_COND] = FL_NEG;
  else
    reg[R_COND] = FL_POS;
}
