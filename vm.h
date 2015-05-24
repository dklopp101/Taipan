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
#define I_AND         11
#define I_OR          12
#define I_NOT         13

#define I_ADD         14
#define I_SUB         15
#define I_MUL         16
#define I_DIV         17
#define I_MOD         18
#define I_NEG         19

#define DIE           20

#define DUMP_ISTK     21
#define DUMP_ISTKTOP  22
#define DUMP_BOOLFLAG 23

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
{ int i[4];
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
        &&i_and,
        &&i_or,
        &&i_not,

        &&i_add,
        &&i_sub,
        &&i_mul,
        &&i_div,
        &&i_mod,
        &&i_neg,

        &&die,

        &&dump_istk,
        &&dump_istktop,
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
        ++(reg.isp);
        istk[reg.isp] = iop(0);
        ++ip;
        execute_next(prog[ip]);

    i_pop:
        reg.isp--;
        ++ip;
        execute_next(prog[ip]);

    // Integer Logic.
    i_eql:
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        reg.bool_flag = i[0] == i[1];

        ++ip;
        execute_next(prog[ip]);

    i_neql:
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        reg.bool_flag = i[0] != i[1];

        ++ip;
        execute_next(prog[ip]);

    i_gt_eql:
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        reg.bool_flag = i[0] >= i[1];

        ++ip;
        execute_next(prog[ip]);


    i_lt_eql:
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        reg.bool_flag = i[0] <= i[1];

        ++ip;
        execute_next(prog[ip]);


    i_gt:
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        reg.bool_flag = i[0] > i[1];

        ++ip;
        execute_next(prog[ip]);

    i_lt:
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        reg.bool_flag = i[0] < i[1];

        ++ip;
        execute_next(prog[ip]);

    i_and:
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        reg.bool_flag = i[0] && i[1];

        ++ip;
        execute_next(prog[ip]);

    i_or:
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        reg.bool_flag = i[0] || i[1];

        ++ip;
        execute_next(prog[ip]);

    i_not:
        i[0] = istk[reg.isp];
        --(reg.isp);

        reg.bool_flag = !(i[0]);

        ++ip;
        execute_next(prog[ip]);

    i_add:
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        ++(reg.isp);
        istk[reg.isp] = i[0] + i[1];

        ++ip;
        execute_next(prog[ip]);

    i_sub:
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        ++(reg.isp);
        istk[reg.isp] = i[0] - i[1];

        ++ip;
        execute_next(prog[ip]);

    i_mul:
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        ++(reg.isp);
        istk[reg.isp] = i[0] * i[1];

        ++ip;
        execute_next(prog[ip]);

    i_div:
        /// ONE DAY IMPLEMENT ERROR THROWING IF DIVIDING BY 0!
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        ++(reg.isp);
        istk[reg.isp] = i[0] / i[1];

        ++ip;
        execute_next(prog[ip]);

    i_mod:
        i[0] = istk[reg.isp];
        --(reg.isp);

        i[1] = istk[reg.isp];
        --(reg.isp);

        ++(reg.isp);
        istk[reg.isp] = i[0] % i[1];

        ++ip;
        execute_next(prog[ip]);


    i_neg:
        i[0] = istk[reg.isp];
        --(reg.isp);

        ++(reg.isp);
        istk[reg.isp] = ~(i[0]);

        ++ip;
        execute_next(prog[ip]);

    die:
        return reg.p_retval;


    // VM Debugging.
    dump_istk:
        printf("\nINTEGER STACK DUMP:\nisp: %d\n\n", reg.isp);
        for (i[0]=0; i[0] < INT_STACK_SIZE; ++i[0])
            printf("[%d] : %d\n", i[0], istk[i[0]]);
        ++ip;
        execute_next(prog[ip]);

    dump_istktop:
        printf("\ninteger stack top: %d\n", istk[reg.isp]);
        ++ip;
        execute_next(prog[ip]);

    dump_boolflag:
        printf("\nBOOL FLAG: %d\n", reg.bool_flag);
        ++ip;
        execute_next(prog[ip]);
}

#endif
