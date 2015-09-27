#ifndef opcodes_h
#define opcodes_h

#define OPC_COUNT        50

#define DIE_OPCODE        0

/* Branching / Jumping. */
#define JMP_OPCODE        1
#define JMPEQL_OPCODE     2
#define JMPNEQL_OPCODE    3
#define CALL_OPCODE       4
#define RET_OPCODE        5

/* Data Managment. */
#define BLOD_OPCODE       6
#define ILOD_OPCODE       7
#define RLOD_OPCODE       8
#define BCPY_OPCODE       9
#define ICPY_OPCODE      10
#define RCPY_OPCODE      11

/* Logic. */
#define BEQL_OPCODE      12
#define IEQL_OPCODE      13
#define BNEQL_OPCODE     14
#define INEQL_OPCODE     15
#define BGTEQL_OPCODE    16
#define IGTEQL_OPCODE    17
#define BGT_OPCODE       18
#define IGT_OPCODE       19
#define BLTEQL_OPCODE    20
#define ILTEQL_OPCODE    21
#define BLT_OPCODE       22
#define ILT_OPCODE       23
#define BAND_OPCODE      24
#define IAND_OPCODE      25
#define BOR_OPCODE       26
#define IOR_OPCODE       27
#define BNOT_OPCODE      28
#define INOT_OPCODE      29

/* Bitwise Logic. */
#define AND_OPCODE       30
#define OR_OPCODE        31
#define XOR_OPCODE	     32
#define NOT_OPCODE       33
#define LSHFT_OPCODE     34
#define RSHFT_OPCODE     35

/* Arithmetic. */
#define BADD_OPCODE      36
#define IADD_OPCODE      37
#define RADD_OPCODE      38
#define BSUB_OPCODE      39
#define ISUB_OPCODE      40
#define RSUB_OPCODE      41
#define BMUL_OPCODE      42
#define IMUL_OPCODE      43
#define RMUL_OPCODE      44
#define BDIV_OPCODE      45
#define IDIV_OPCODE      46
#define RDIV_OPCODE      47
#define BMOD_OPCODE      48
#define IMOD_OPCODE      49

char* opcode_strmap[OPC_COUNT];

int is_opcode(const char* str);
int get_opcode(const char* str);
int in_opcode_range(const int opcode);

#endif
