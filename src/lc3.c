/* Includes */
#include "../include/binops.h"
#include "../include/flags.h"
#include "../include/inbuffer.h"
#include "../include/memio.h"
#include "../include/opcodes.h"
#include "../include/registers.h"

#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
 
void handle_interrupt(int signal)
{
	restore_input_buffering();
	printf("\n");
	exit(-2);
}

int main(int argc, const char *argv[])
{
	/* Load Arguments */
	if (argc < 2)
	{
		printf("lc3 [image-file1] ...\n");
		exit(2);
	}

	/* Setup */
	for (int j = 1; j < argc; ++j)
		if (!read_image(argv[j]))
		{
			printf("failed to load image: %s\n", argv[j]);
			exit(1);
		}

	signal(SIGINT, handle_interrupt);
	disable_input_buffering();

	/* set the Program Counter to starting position
   * 0x3000 is the default
   */
	enum
	{
		PC_START = 0x3000
	};
	reg[R_PC] = PC_START;

	int running = 1;
	while (running)
	{
		uint16_t instr = mem_read(reg[R_PC]++);
		uint16_t op = instr >> 12;

		switch (op)
		{
		case OP_ADD:
		{
			/* destination register */
			uint16_t r0 = (instr >> 9) & 0x7;
			/* source register 1 */
			uint16_t r1 = (instr >> 6) & 0x7;
			/* immediate or regisster mode */
			uint16_t imm_flag = (instr >> 5) & 0x1;

			if (imm_flag)
			{
				uint16_t imm5 = sign_extend(instr & 0x1F, 5);
				reg[r0] = reg[r1] + imm5;
			}
			else
			{
				uint16_t r2 = instr & 0x7;
				reg[r0] = reg[r1] + reg[r2];
			}

			update_flags(r0);
		}
		break;
		case OP_AND:
		{
			uint16_t r0 = (instr >> 9) & 0x7;
			uint16_t r1 = (instr >> 6) & 0x7;
			uint16_t imm_flag = (instr >> 5) & 0x1;

			if (imm_flag)
			{
				uint16_t imm5 = sign_extend(instr & 0x1F, 5);
				reg[r0] = reg[r1] & imm5;
			}
			else
			{
				uint16_t r2 = instr & 0x7;
				reg[r0] = reg[r1] & reg[r2];
			}

			update_flags(r0);
		}
		break;
		case OP_NOT:
		{
			uint16_t r0 = (instr >> 9) & 0x7;
			uint16_t r1 = (instr >> 6) & 0x7;

			reg[r0] = ~reg[r1];
			update_flags(r0);
		}
		break;
		case OP_BR:
		{
			uint16_t pc_offset = sign_extend((instr)&0x1ff, 9);
			uint16_t cond_flag = (instr >> 9) & 0x7;
			if (cond_flag & reg[R_COND])
			{
				reg[R_PC] += pc_offset;
			}
		}
		break;
		case OP_JMP:
		{
			uint16_t r1 = (instr >> 6) & 0x7;
			reg[R_PC] = reg[r1];
		}
		break;
		case OP_JSR:
		{
			uint16_t r1 = (instr >> 6) & 0x7;
			uint16_t long_pc_offset = sign_extend(instr & 0x7ff, 11);
			uint16_t long_flag = (instr >> 11) & 1;

			reg[R_R7] = reg[R_PC];
			if (long_flag)
			{
				reg[R_PC] += long_pc_offset; // JSR
			}
			else
			{
				reg[R_PC] = reg[r1]; // JSRR
			}
			break;
		}
		break;
		case OP_LD:
		{
			uint16_t r0 = (instr >> 9) & 0x7;
			uint16_t pc_offset = sign_extend(instr & 0x1ff, 9);
			reg[r0] = mem_read(reg[R_PC] + pc_offset);
			update_flags(r0);
		}
		break;
		case OP_LDI:
		{
			uint16_t r0 = (instr >> 9) & 0x7;
			uint16_t pc_offset = sign_extend(instr & 0x1ff, 9);
			reg[r0] = mem_read(mem_read(reg[R_PC] + pc_offset));
			update_flags(r0);
		}
		break;
		case OP_LDR:
		{
			uint16_t r0 = (instr >> 9) & 0x7;
			uint16_t r1 = (instr >> 6) & 0x7;
			uint16_t offset = sign_extend(instr & 0x3F, 6);
			reg[r0] = mem_read(reg[r1] + offset);
			update_flags(r0);
		}
		break;
		case OP_LEA:
		{
			uint16_t r0 = (instr >> 9) & 0x7;
			uint16_t pc_offset = sign_extend(instr & 0x1ff, 9);
			reg[r0] = reg[R_PC] + pc_offset;
			update_flags(r0);
		}
		break;
		case OP_ST:
		{
			uint16_t r0 = (instr >> 9) & 0x7;
			uint16_t pc_offset = sign_extend(instr & 0x1ff, 9);
			mem_write(reg[R_PC] + pc_offset, reg[r0]);
		}
		break;
		case OP_STI:
		{
			uint16_t r0 = (instr >> 9) & 0x7;
			uint16_t pc_offset = sign_extend(instr & 0x1ff, 9);
			mem_write(mem_read(reg[R_PC] + pc_offset), reg[r0]);
		}
		break;
		case OP_STR:
		{
			uint16_t r0 = (instr >> 9) & 0x7;
			uint16_t r1 = (instr >> 6) & 0x7;
			uint16_t offset = sign_extend(instr & 0x3F, 6);
			mem_write(reg[r1] + offset, reg[r0]);
		}
		break;
		case OP_TRAP:
			switch (instr & 0xFF)
			{
			case TRAP_GETC:
			{
				reg[R_R0] = (uint16_t)getchar();
			}
			break;
			case TRAP_OUT:
			{
				putc((char)reg[R_R0], stdout);
				fflush(stdout);
			}
			break;
			case TRAP_PUTS:
			{
				uint16_t *c = memory + reg[R_R0];
				while (*c)
				{
					putc((char)*c, stdout);
					++c;
				}
				fflush(stdout);
			}
			break;
			case TRAP_IN:
			{
				printf("Enter a character: ");
				reg[R_R0] = (uint16_t)getchar();
			}
			break;
			case TRAP_PUTSP:
			{
				uint16_t *c = memory + reg[R_R0];
				while (*c)
				{
					char char1 = (*c) & 0xFF;
					putc(char1, stdout);
					char char2 = (*c) >> 8;
					if (char2)
						putc(char2, stdout);
					++c;
				}
				fflush(stdout);
			}
			break;
			case TRAP_HALT:
			{
				puts("HALT");
				fflush(stdout);
				running = 0;
				break;
			}
			break;
			case OP_RES:
			case OP_RTI:
			default:
				/* Bad Opcode */
				abort();
				break;
			}
		}
	}
	/* Shutdown */
	restore_input_buffering();

	return 0;
}
