#include <string.h>

#include "opcodes.h"

char* opcode_strmap[OPC_COUNT] = { "die",
                                   "jmp",
                                   "jmpeql",
						           "jmpneql",
                                   "call",
                                   "ret",
                                   "blod",
                                   "ilod",
                                   "rlod",
                                   "bcpy",
                                   "icpy",
                                   "rcpy",
                                   "beql",
                                   "ieql",
                                   "bneql",
                                   "ineql",
                                   "bgteql",
                                   "igteql",
                                   "bgt",
                                   "igt",
                                   "blteql",
                                   "ilteql",
                                   "blt",
                                   "ilt",
                                   "band",
                                   "iand",
                                   "bor",
                                   "ior",
                                   "bnot",
                                   "inot",
                                   "and",
                                   "or",
                                   "xor",
                                   "not",
                                   "lshft",
                                   "rshft",
                                   "badd",
                                   "iadd",
                                   "radd",
                                   "bsub",
                                   "isub",
                                   "rsub",
                                   "bmul",
                                   "imul",
                                   "rmul",
                                   "bdiv",
                                   "idiv",
                                   "rdiv",
                                   "bmod",
                                   "imod"};


/* Returns opcode if str if valid opcode string.
   Otherwise returns -1 if string not valid opcode. */
int is_opcode(const char* str)
{
	int i;

	for (i=0; i < OPC_COUNT; ++i) {
		if (strcmp(str, opcode_strmap[i]) == 0)
            return 1;
	}

	return 0;
}



int get_opcode(const char* str)
{
	int opcode = 0;

	for (; opcode < OPC_COUNT; ++opcode) {
		if (strcmp(str, opcode_strmap[opcode]) == 0)
			break;
	}

	return opcode;
}



/* Predicate function testing whether opcode is within valid range. */
int in_opcode_range(const int opcode)
{
	return ((opcode > -1) && (opcode < OPC_COUNT));
}
