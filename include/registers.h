/* Definition of Registers including Memory Mapped Registers */
#ifndef REGISTERS_H
#define REGISTERS_H

/* 10 registers
*** 8 general purpose
*** 1 program counter
*** 1 condition flag
*/
enum
  {
    R_R0 = 0, //general purpose
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC,     //program counter
    R_COND,   //condition flag
    R_COUNT   //number of registers
  };

/* Memory Mapped Registers */
enum
  {
    MR_KBSR = 0xFE00, //keyboard status
    MR_KBDR = 0xFE02  //keyboard data
  };

#endif