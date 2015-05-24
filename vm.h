#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

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
#define iop(n) ((prog[ip]).iop[n])
#define execute_next(instr) goto *optable[(instr).opcode]

typedef struct {
    byte opcode;
    int iop[INT_OPERAND_MAX];
} instr_t;


/// REGISTERS.
typedef struct {
    int p_retval, bool_flag, isp;
    int ireg[INT_REG_SIZE];
} vmreg_t;

#define init_registers() reg.bool_flag = 1; \
                         reg.p_retval = 0;  \
                         reg.isp = -1

#define istk_push(v) ++reg.isp; istk[reg.isp] = v

#define istk_pop()   istk[--(reg.isp)]

/// VM FUNCTION.
int tprocess(instr_t* prog)
{ int i;
    int ip;

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
        &&dump_boolflag
    };

    vmreg_t reg;
    init_registers();

    int istk[INT_STACK_SIZE];

    // Jump to program start instruction.
    ip = prog->iop[0];
    execute_next(prog[ip]);


    /// Instruction Code
    jump:
        ip = iop(0);
        execute_next(prog[ip]);

    jump_t:
        if (reg.bool_flag) {
            ip = iop(0);
            execute_next(prog[ip]);
        } else {
            ++ip;
            execute_next(prog[ip]);
        }

    jump_f:
        if (!(reg.bool_flag)) {
            ip = iop(0);
            execute_next(prog[ip]);
        } else {
            ++ip;
            execute_next(prog[ip]);
        }

    i_push:
        istk_push(iop(0));
        ++ip;
        execute_next(prog[ip]);

    i_pop:
        reg.isp--;
        ++ip;
        execute_next(prog[ip]);

    // Integer Logic.
    i_eql:
        reg.bool_flag = istk_pop() == istk_pop();
        ++ip;
        execute_next(prog[ip]);

    i_neql:
        reg.bool_flag = istk_pop() != istk_pop();
        ++ip;
        execute_next(prog[ip]);

    i_gt_eql:
        reg.bool_flag = istk_pop() >= istk_pop();
        ++ip;
        execute_next(prog[ip]);


    i_lt_eql:
        reg.bool_flag = istk_pop() <= istk_pop();
        ++ip;
        execute_next(prog[ip]);


    i_gt:
        reg.bool_flag = istk_pop() > istk_pop();
        ++ip;
        execute_next(prog[ip]);

    i_lt:
        reg.bool_flag = istk_pop() < istk_pop();
        ++ip;
        execute_next(prog[ip]);

    die:
        return reg.p_retval;


    // VM Debugging.
    dump_istk:
        printf("\nINTEGER STACK DUMP:\nisp: %d\n\n", reg.isp);
        for (i=0; i < INT_STACK_SIZE; ++i)
            printf("[%d] : %d\n", i, istk[i]);
        ++ip;
        execute_next(prog[ip]);

    dump_boolflag:
        printf("\nBOOL FLAG: %d\n", reg.bool_flag);
        ++ip;
        execute_next(prog[ip]);
}

#endif
