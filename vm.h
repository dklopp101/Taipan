#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#include "opcodes.h"

typedef unsigned char byte;

#define CALLSTACK_SIZE   20

#define INT_OPERAND_MAX   6
#define INT_STACK_SIZE   40
#define INT_REG_SIZE      8

#define FLOAT_OPERAND_MAX   6
#define FLOAT_STACK_SIZE   40
#define FLOAT_REG_SIZE      8

/// SUBROUTINES.
#define frame_base     (reg.frame_ptr[0])
#define frame_calladdr (reg.frame_ptr[1])
#define frame_retaddr  (reg.frame_ptr[2])
#define frame_hcount   (reg.frame_ptr[3])

typedef int t_int;
typedef double t_float;

// Push the base frame onto the callstack then insert the
// opening call to the main subroutine.
#define init_callstack() cstk[reg.csp]   = 0;           \
                         cstk[reg.csp+1] = 0;           \
                         cstk[reg.csp+2] = 0;           \
                         cstk[reg.csp+3] = 0;           \
                         reg.csp = 2;                   \
                         reg.frame_ptr = cstk

#define push_frame(a, b, c, d) ++(reg.csp); cstk

/// INSTRUCTIONS.
#define iop(n) ((prog[ip]).iop[n])
#define fop(n) ((prog[ip]).fop[n])
#define execute_next(instr) goto *optable[(instr).opcode]

typedef struct {
    byte opcode;
    t_int iop[INT_OPERAND_MAX];
    t_float fop[FLOAT_OPERAND_MAX];
} instr_t;


/// REGISTERS.
typedef struct {
    int p_retval, bool_flag, csp, isp, fsp;
    t_int ireg[INT_REG_SIZE];
    t_float freg[FLOAT_REG_SIZE];
    int* frame_ptr;
} vmreg_t;

#define init_registers() reg.bool_flag = 0; \
                         reg.p_retval = 0;  \
                         reg.isp = -1;      \
                         reg.fsp = -1;      \
                         reg.csp = 0

/// VM FUNCTION.
int tprocess(instr_t* prog)
{
    build_optable();

    t_int   i[3];
    t_float f[3];

    vmreg_t reg;
    init_registers();

    int cstk[CALLSTACK_SIZE];
    init_callstack();

    t_int   istk[INT_STACK_SIZE];
    t_float fstk[FLOAT_STACK_SIZE];

    int ip = 0;

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

    // Push integer constant onto integer stack.
    i_const:
        ++(reg.isp);
        istk[reg.isp] = iop(0);
        ++ip;
        execute_next(prog[ip]);

    // Pop value off top of integer stack.
    // Right now it simply decrements isp because
    // there's nowhere to pop to until the RAM/heap get's written.
    i_pop:
        reg.isp--;
        ++ip;
        execute_next(prog[ip]);

    f_const:
        ++(reg.fsp);
        fstk[reg.fsp] = fop(0);
        ++ip;
        execute_next(prog[ip]);

    f_pop:
        reg.fsp--;
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

    // Integer Arithmetic.
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


    i_inv:
        i[0] = istk[reg.isp];
        --(reg.isp);

        ++(reg.isp);
        istk[reg.isp] = ~(i[0]);

        ++ip;
        execute_next(prog[ip]);


    // Floating Point Logic.
    f_eql:
        ++ip;
        execute_next(prog[ip]);

    f_neql:
        ++ip;
        execute_next(prog[ip]);

    f_gt_eql:
        ++ip;
        execute_next(prog[ip]);


    f_lt_eql:
        ++ip;
        execute_next(prog[ip]);

    f_gt:
        ++ip;
        execute_next(prog[ip]);

    f_lt:
        ++ip;
        execute_next(prog[ip]);


    // Floating Point Arithmetic
    f_add:
        f[0] = fstk[reg.fsp];
        --(reg.fsp);

        f[1] = fstk[reg.fsp];
        --(reg.fsp);

        ++(reg.fsp);
        fstk[reg.fsp] = f[0] + f[1];

        ++ip;
        execute_next(prog[ip]);

        ++ip;
        execute_next(prog[ip]);

    f_sub:
        f[0] = fstk[reg.fsp];
        --(reg.fsp);

        f[1] = fstk[reg.fsp];
        --(reg.fsp);

        ++(reg.fsp);
        fstk[reg.fsp] = f[0] - f[1];

        ++ip;
        execute_next(prog[ip]);

    f_mul:
        f[0] = fstk[reg.fsp];
        --(reg.fsp);

        f[1] = fstk[reg.fsp];
        --(reg.fsp);

        ++(reg.fsp);
        fstk[reg.fsp] = f[0] * f[1];

        ++ip;
        execute_next(prog[ip]);

    f_div:
        f[0] = fstk[reg.fsp];
        --(reg.fsp);

        f[1] = fstk[reg.fsp];
        --(reg.fsp);

        // Check for zero division.
        /*if (f[1] == 0)
            throw_exception(ZERO_DIVIDE_EXCEPTION);*/

        ++(reg.fsp);
        fstk[reg.fsp] = f[0] / f[1];

        ++ip;
        execute_next(prog[ip]);

    f_mod:

        ++ip;
        execute_next(prog[ip]);


    f_inv:
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

    dump_fstk:
        printf("\nFLOAT STACK DUMP:\nfsp: %d\n\n", reg.fsp);
        for (i[0]=0; i[0] < FLOAT_STACK_SIZE; ++i[0])
            printf("[%d] : %f\n", i[0], fstk[i[0]]);

        ++ip;
        execute_next(prog[ip]);

    dump_fstktop:
        printf("\float stack top: %f\n", fstk[reg.fsp]);
        ++ip;
        execute_next(prog[ip]);

    dump_boolflag:
        printf("\nBOOL FLAG: %d\n", reg.bool_flag);

        ++ip;
        execute_next(prog[ip]);

    dump_cstk:
        printf("\nCALL STACK DUMP:\n, csp: %d\n\n", reg.csp);

        for (i[0]=0; i[0] < CALLSTACK_SIZE; ++i[0])
            printf("[%d] : %d\n", i[0], cstk[i[0]]);

        ++ip;
        execute_next(prog[ip]);

}

#endif
