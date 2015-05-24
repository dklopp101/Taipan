#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#define execute_next(instr) goto *optable[(instr)->opcode]

#define JUMP           0
#define JUMP_T         1
#define JUMP_F         2

#define I_PUSH         3
#define I_POP          4
#define I_EQL          5

#define I_NEQL         6
#define I_GT_EQL       7
#define I_LT_EQL       8
#define I_GT           9
#define I_LT          10

#define DIE           11

#define DUMP_ISTK     12
#define DUMP_BOOLFLAG 13

typedef unsigned char byte;

#define INT_OPERAND_MAX   6
#define INT_STACK_SIZE   40
#define INT_REG_SIZE      8

/// INSTRUCTION.
#define opc()  (ip->opcode)
#define iop(n) (ip->iop[n])
#define rop(n) (ip->rop[n])

typedef struct {
    byte opcode;
    int iop[INT_OPERAND_MAX];
} instr_t;


/// REGISTERS.
typedef struct {
    instr_t* ip;
    int p_retval, bool_flag, isp;
    int ireg[INT_REG_SIZE];
} vmreg_t;

#define init_registers() reg.ip = is;       \
                         reg.bool_flag = 0; \
                         reg.p_retval = 0;  \
                         reg.isp = -1


/// VM FUNCTION.
int tprocess(instr_t* is)
{ int i;

    static void* optable[255] = {
        &&jump,
        &&jump_t,
        &&jump_f,

        &&i_push,
        &&i_pop,

        &&i_eql,
        &&i_neql,
        &&i_gt_eql,
        &&i_lt_eql,
        &&i_gt,
        &&i_lt,

        &&die,

        &&dump_istk,
        &&dump_boolflag,
    };

    vmreg_t reg;
    init_registers();

    int istk[INT_STACK_SIZE];

    // Jump to program start instruction.
    reg.ip = is + is[0].iop[0];
    execute_next(reg.ip);


    /// Instruction Code
    jump:
        reg.ip = is + reg.ip->iop[0];
        execute_next(reg.ip);

    jump_t:
        if (reg.bool_flag) {
            reg.ip = is + reg.ip->iop[0];
            execute_next(reg.ip);
        } else {
            execute_next(reg.ip++);
        }

    jump_f:
        if (reg.bool_flag) {
            reg.ip = is + reg.ip->iop[0];
            execute_next(reg.ip);
        } else {
            execute_next(reg.ip++);
        }

    i_push:
        (reg.isp)++;
        istk[reg.isp] = reg.ip->iop[0];
        execute_next(reg.ip++);

    i_pop:
        reg.isp--;
        execute_next(reg.ip++);

    i_eql:
        reg.bool_flag = istk[--(reg.isp)] == istk[--(reg.isp)];
        execute_next(reg.ip++);

    i_neql:
        reg.bool_flag = istk[--(reg.isp)] != istk[--(reg.isp)];
        execute_next(reg.ip++);

    i_gt_eql:
        reg.bool_flag = istk[--(reg.isp)] >= istk[--(reg.isp)];
        execute_next(reg.ip++);


    i_lt_eql:
        reg.bool_flag = istk[--(reg.isp)] <= istk[--(reg.isp)];
        execute_next(reg.ip++);


    i_gt:
        reg.bool_flag = istk[--(reg.isp)] > istk[--(reg.isp)];
        execute_next(reg.ip++);

    i_lt:
        reg.bool_flag = istk[--(reg.isp)] < istk[--(reg.isp)];
        execute_next(reg.ip++);

    die:
        return reg.p_retval;


    // VM Debugging.
    dump_istk:
        printf("\nINTEGER STACK DUMP:\nisp: %d\n\n", reg.isp);
        for (i=0; i < INT_STACK_SIZE; ++i)
            printf("[%d] : %d\n", i, istk[i]);
        execute_next(reg.ip++);

    dump_boolflag:
        printf("\nBOOL FLAG: %d\n", reg.bool_flag);
        execute_next(reg.ip++);
}

#endif
