/* Op Codes and Trap Codes definitions */
#ifndef OPCODES_H
#define OPCODES_H

/* Op Codes */ 
enum
  {
    OP_BR = 0, // conditional branch
    OP_ADD,    // addition
    OP_LD,     // load
    OP_ST,     // store
    OP_JSR,    // jump to subroutine
    OP_AND,    // bit-wise logical and
    OP_LDR,    // load base + offset
    OP_STR,    // store base + offset
    OP_RTI,    // return from interrupt
    OP_NOT,    // bit-wise complement
    OP_LDI,    // load indirect
    OP_STI,    // store indirect
    OP_JMP,    // jump
    OP_RES,    // reserved
    OP_LEA,    // load effective address
    OP_TRAP    // system call
  };

/* Trap Codes */
enum
  {
    TRAP_GETC = 0x20,  //get character from keyboard
    TRAP_OUT = 0x21,   //output a character
    TRAP_PUTS = 0x22,  //output a character string
    TRAP_IN = 0x23,    //get character string
    TRAP_PUTSP = 0x24, //output a byte string
    TRAP_HALT = 0x25   //halt program ececution
  };

#endif