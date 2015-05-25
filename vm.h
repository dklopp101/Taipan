#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#define JUMP           0
#define JUMP_T         1
#define JUMP_F         2

#define CALL           3
#define LEAVE          4

#define I_CONST        5
#define I_POP          6

#define I_EQL          7
#define I_NEQL         8
#define I_GT_EQL       9
#define I_LT_EQL      10
#define I_GT          11
#define I_LT          12
#define I_AND         13
#define I_OR          14
#define I_NOT         15

#define I_ADD         16
#define I_SUB         17
#define I_MUL         18
#define I_DIV         19
#define I_MOD         20
#define I_NEG         21

#define DIE           22

#define DUMP_ISTK     23
#define DUMP_ISTKTOP  24
#define DUMP_BOOLFLAG 25
#define DUMP_CSTK     26

typedef unsigned char byte;

#define CALLSTACK_SIZE   20
#define INT_OPERAND_MAX   6
#define INT_STACK_SIZE   40
#define INT_REG_SIZE      8

/// SUBROUTINES.

#define frame_base     (reg.frame_ptr[0])
#define frame_calladdr (reg.frame_ptr[1])
#define frame_retaddr  (reg.frame_ptr[2])
#define frame_hcount   (reg.frame_ptr[3])

// Push the base frame onto the callstack then insert the
// opening call to the main subroutine.
#define init_callstack() cstk[reg.csp]   = 0;           \
                         cstk[reg.csp+1] = 0;           \
                         cstk[reg.csp+2] = 0;           \
                         cstk[reg.csp+3] = 0;           \
                         reg.csp = 3

/// INSTRUCTION.
#define iop(n) ((prog[ip]).iop[n])
#define execute_next(instr) goto *optable[(instr).opcode]

typedef struct {
    byte opcode;
    int iop[INT_OPERAND_MAX];
} instr_t;


/// REGISTERS.
typedef struct {
    int p_retval, bool_flag, isp, csp;
    int ireg[INT_REG_SIZE];
    int* frame_ptr;
} vmreg_t;

#define init_registers() reg.bool_flag = 1; \
                         reg.p_retval = 0;  \
                         reg.isp = -1;      \
                         reg.csp = -1

/// VM FUNCTION.
int tprocess(instr_t* prog)
{
    int i[4];
    int ip = 0;

    static void* optable[255] = {
        &&jump,
        &&jump_t,
        &&jump_f,

        &&call,
        &&leave,

        &&i_const,
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
        &&dump_boolflag,
        &&dump_cstk
    };

    vmreg_t reg;
    init_registers();

    int cstk[CALLSTACK_SIZE];
    int istk[INT_STACK_SIZE];

    init_callstack();

    execute_next(prog[ip]);

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

    call:
        // Finalise the current frame then set up the new one.
        ++(reg.csp);
        cstk[reg.csp] = frame_base; // push the current frame's finder index.

        ++(reg.csp);
        cstk[reg.csp] = reg.csp + 1; // push the new frame's base(it's index in the callstack).

        reg.frame_ptr = cstk + reg.csp; // officiate the new frame by setting the frame_ptr pointer.

        ++(reg.csp);
        cstk[reg.csp] = iop(0); // push the index of the subr being called.

        ++(reg.csp);
        cstk[reg.csp] = ip + 1; // push the index of the frame's return instruction.

        ++(reg.csp);
        cstk[reg.csp] = 0; // push 0 to reserve a spot for the frame's exception handler counter.

        ip = frame_calladdr;
        execute_next(prog[ip]);

    leave:
        // Firstly set ip to the frame's return instruction.
        ip = frame_retaddr;

        // Exit the current frame to the next outer one by
        // jumping to the outer frame's finder. Then setting
        // the frame pointer to said frame, then decrement
        // csp because the finder isnt needed anymore.
        reg.csp = frame_base;

        reg.frame_ptr = cstk + cstk[reg.csp];
        --(reg.csp);

        // Jump to the frame's return instruction.
        execute_next(prog[ip]);

    i_const:
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

    dump_cstk:
        printf("\nCALL STACK DUMP:\nsub call count: %d, csp: %d\n\n", subcalls, reg.csp);

        for (i[0]=0; i[0] < CALLSTACK_SIZE; ++i[0])
            printf("[%d] : %d\n", i[0], cstk[i[0]]);

        ++ip;
        execute_next(prog[ip]);
}

#endif
