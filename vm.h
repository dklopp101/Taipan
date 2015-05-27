#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#include "opcodes.h"

#define PROCESS_RETVAL_INDEX 0

#define IARITH_OP_A (ireg[5])
#define IARITH_OP_B (ireg[6])

#define istk_top       (istk[isp])
#define istk_pop()     (istk[isp--])
#define istk_push(v)   (istk[++isp] = v)

#define fstk_top       (fstk[fsp)
#define fstk_pop()     (fstk[fsp--])
#define fstk_push(v)   (fstk[++fsp] = v)

typedef unsigned char byte;
typedef int t_int;
typedef double t_float;

#define INT_OPERAND_MAX     6
#define ISTK_SIZE          40
#define IREG_SIZE           8

#define FLOAT_OPERAND_MAX   6
#define FSTK_SIZE          40
#define FREG_SIZE           8

/// SUBROUTINES.
#define CALLSTACK_SIZE     20

typedef struct {
    int stk[CALLSTACK_SIZE];
    int* frame;
} callstack_t;

// Initalises callstack pointer, pushes main frame,
// initalises frame pointer.
#define init_callstack() cstk.stk[0] = 0;   \
                         cstk.stk[1] = 0;   \
                         cstk.stk[2] = 0;   \
                         cstk.stk[3] = 0;   \
                         cstk.frame = cstk.stk

// Subroutine Callstack Frames.
// frame: 0 - frame base (it's index in the callstack).
//        1 - start instruction (first instruction of subroutine).
//        2 - return instruction (instruction directly after subroutine call).
//        3 - exception handler count (counts how many exception handlers in frame).
//        n - frame base finder.
//
// When a subroutine is called it first pushes 0,1,2,3 of above onto the stack,
// this is the head of the frame, when another subroutine is called from this one,
// the frame base finder is pushed onto the callstack, then the frame pointer is
// reset. This allows the previous frame base to be found no matter how many
// exception handlers or whatever happends inside the stack.

#define frame_base       (cstk.frame[0])
#define frame_calladdr   (cstk.frame[1])
#define frame_retaddr    (cstk.frame[2])
#define frame_hcount     (cstk.frame[3])

/// INSTRUCTIONS.
#define iop(n) ((prog[ip]).iop[n])
#define fop(n) ((prog[ip]).fop[n])

#define execute_next(instr) goto *optable[(instr).opcode]

typedef struct {
    byte opcode;
    t_int iop[INT_OPERAND_MAX];
    t_float fop[FLOAT_OPERAND_MAX];
} instr_t;


/// VM FUNCTION.
int tprocess(instr_t* __restrict__ prog)
{
    build_optable();

    callstack_t cstk;
    init_callstack();

    register t_int ireg[8];

    register int isp = -1;
    register int fsp = -1;
    register int csp = 0;
    register int ip  = 0;

    t_int ireg[IREG_SIZE];
    t_float freg[FREG_SIZE];

    t_int istk[ISTK_SIZE];
    t_float fstk[FSTK_SIZE];




    // Program execution begins here.
    execute_next(prog[ip]);


/// CONTOL TRANSFER INSTRUCTIONS.
    // Conditional / Unconditional Branching.
    jump:
        ip = iop(0);
        execute_next(prog[ip]);

    jump_eql:
        if (IARITH_OP_A == IARITH_OP_B)
            ip = iop(0);
        else
            ++ip;
        execute_next(prog[ip]);

    jump_neql:
        if (IARITH_OP_A != IARITH_OP_B)
            ip = iop(0);
        else
            ++ip;
        execute_next(prog[ip]);

    jump_gt_eql:
        if (IARITH_OP_A >= IARITH_OP_B)
            ip = iop(0);
        else
            ++ip;
        execute_next(prog[ip]);

    jump_lt_eql:
        if (IARITH_OP_A <= IARITH_OP_B)
            ip = iop(0);
        else
            ++ip;
        execute_next(prog[ip]);

    jump_gt:
        if (IARITH_OP_A > IARITH_OP_B)
            ip = iop(0);
        else
            ++ip;
        execute_next(prog[ip]);

    jump_lt:
        if (IARITH_OP_A < IARITH_OP_B)
            ip = iop(0);
        else
            ++ip;
        execute_next(prog[ip]);


    // Recursive Subroutine Instructions.
    call:
        // Finalise the current frame then set up the new one.
        ++(csp);
        cstk.stk[csp] = frame_base; // push the current frame's finder index.

        ++(csp);
        cstk.stk[csp] = csp + 1; // push the new frame's base(it's index in the callstack).

        cstk.frame = cstk.stk + csp; // officiate the new frame by setting current frame.

        ++(csp);
        cstk.stk[csp] = iop(0); // push the index of the subr being called.

        ++(csp);
        cstk.stk[csp] = ip + 1; // push the index of the frame's return instruction.

        ++(csp);
        cstk.stk[csp] = 0; // push 0 to reserve a spot for the frame's exception handler counter.

        ip = frame_calladdr;
        execute_next(prog[ip]);

    leave:
        // Firstly set ip to the frame's return instruction.
        ip = frame_retaddr;

        // Exit the current frame to the next outer one by
        // jumping to the outer frame's finder. Then setting
        // the frame pointer to said frame, then decrement
        // csp because the finder isnt needed anymore.
        csp = frame_base;

        cstk.frame = cstk.stk + cstk.stk[csp];
        --(csp);

        // Jump to the frame's return instruction.
        execute_next(prog[ip]);


/// INTEGER REGISTERS INSTRUCTIONS.
    // Data Copying.
    ireg_load_c:
        ireg[iop(0)] = iop(1);
        execute_next(prog[++ip]);

    ireg_load_s:
        ireg[iop(0)] = istk[isp - iop(1));
        execute_next(prog[++ip]);

(IARITH_OP_A >= IARITH_OP_B)

    iregR_load_c:
        ireg[1] = iop(0);
        execute_next(prog[++ip]);

    iregS_load_s:
        ireg[1] = istk[isp - iop(1));
        execute_next(prog[++ip]);

    iregL_load_c:
        ireg[2] = iop(0);
        execute_next(prog[++ip]);

    iregL_load_s:
        ireg[2] = istk[isp - iop(1));
        execute_next(prog[++ip]);

    iregR_load_c:
        ireg[3] = iop(0);
        execute_next(prog[++ip]);

    iregR_load_s:
        ireg[3] = istk[isp - iop(1));
        execute_next(prog[++ip]);

    ireg_swap:
        i = ireg[iop(0)];
        ireg[iop(0)] = ireg[iop(1)];
        ireg[iop(1)] = i;
        execute_next(prog[++ip]);

    ireg_swap_s:
        i = ireg[iop(0)];
        ireg[iop(0)] = istk[iop(1)];
        ireg[iop(1)] = i;
        execute_next(prog[++ip]);


    // Binary/Unary Logic.
    ireg_eql:
        IARITH_OP_A = IARITH_OP_A == IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_neql:
        IARITH_OP_A = IARITH_OP_A != IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_gt_eql:
        IARITH_OP_A = IARITH_OP_A >= IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_lt_eql:
        IARITH_OP_A = IARITH_OP_A <= IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_gt:
        IARITH_OP_A = IARITH_OP_A > IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_lt:
        IARITH_OP_A = IARITH_OP_A < IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_and:
        IARITH_OP_A = IARITH_OP_A && IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_or:
        IARITH_OP_A = IARITH_OP_A || IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_not:
        IARITH_OP_A = !IARITH_OP_A;
        execute_next(prog[++ip]);

    ireg_AND:
        IARITH_OP_A = IARITH_OP_A & IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_OR:
        IARITH_OP_A = IARITH_OP_A | IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_XOR:
        IARITH_OP_A = IARITH_OP_A ^ IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_NOT:
        IARITH_OP_A = ~IARITH_OP_A;
        execute_next(prog[++ip]);

    ireg_LSHIFT:
        IARITH_OP_A = IARITH_OP_A << IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_LRIGHT:
        IARITH_OP_A = IARITH_OP_A >> IARITH_OP_B;
        execute_next(prog[++ip]);

    // Binary/Unary Arithmetic.
    ireg_inc:
        ++IARITH_OP_A;
        execute_next(prog[++ip]);

    ireg_dec:
        --IARITH_OP_A;
        execute_next(prog[++ip]);

    ireg_add:
        IARITH_OP_A += IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_sub:
        IARITH_OP_A -= IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_mul:
        IARITH_OP_A *= IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_div:
        IARITH_OP_A /= IARITH_OP_B;
        execute_next(prog[++ip]);

    ireg_mod:
        IARITH_OP_A %= IARITH_OP_B;
        execute_next(prog[++ip]);


/// INTEGER STACK INSTRUCTIONS.
    // Data Copying.
    istk_push_c:
        istk_push(iop(0));
        execute_next(prog[++ip]);

    istk_push_r:
        istk_push(ireg[iop(0)]);
        execute_next(prog[++ip]);

    istk_pop:
        --isp;
        execute_next(prog[++ip]);

    istk_pop_r:
        ireg[isop(0)] = istk_pop();
        execute_next(prog[++ip]);

    istk_swap:
        i = istk[iop(0)];
        istk[iop(0)] = istk[iop(1)];
        istk[iop(1)] = i;
        execute_next(prog[++ip]);

    istk_reset:
        isp = 0;
        execute_next(prog[++ip]);


    // Binary/Unary Logic.
    istk_eql:
        istk_top = istk_pop() == istk_top;
        execute_next(prog[++ip]);

    istk_neql:
        istk_top = istk_pop() != istk_top;
        execute_next(prog[++ip]);

    istk_gt_eql:
        istk_top = istk_pop() >= istk_top;
        execute_next(prog[++ip]);

    istk_lt_eql:
        istk_top = istk_pop() <= istk_top;
        execute_next(prog[++ip]);

    istk_gt:
        istk_top = istk_pop() > istk_top;
        execute_next(prog[++ip]);

    istk_lt:
        istk_top = istk_pop() > istk_top;
        execute_next(prog[++ip]);

    istk_and:
        istk_top = istk_pop() && istk_top;
        execute_next(prog[++ip]);

    istk_or:
        istk_top = istk_pop() || istk_top;
        execute_next(prog[++ip]);

    istk_not:
        istk_top = !istk_pop();
        execute_next(prog[++ip]);


    // Binary/Unary Arithmetic.
    istk_inc:
        ++istk_top;
        execute_next(prog[++ip]);

    istk_dec:
        --istk_top;
        execute_next(prog[++ip]);

    istk_add:
        istk_top = istk_pop() + istk_top;
        execute_next(prog[++ip]);

    istk_sub:
        istk_top = istk_pop() - istk_top;
        execute_next(prog[++ip]);

    istk_mul:
        istk_top = istk_pop() * istk_top;
        execute_next(prog[++ip]);

    istk_div:
        istk_top = istk_pop() / istk_top;
        execute_next(prog[++ip]);

    istk_mod:
        istk_top = istk_pop() % istk_top;
        execute_next(prog[++ip]);

    istk_inv:
        istk_top = ~istk_top;
        execute_next(prog[++ip]);






    die:
        return ireg[PROCESS_RETVAL_INDEX];


    // VM Debugging.
    dump_istk:
        printf("\nINTEGER STACK DUMP:\nisp: %d\n\n", isp);
        for (iop_left=0; iop_left < ISTK_SIZE; ++iop_left)
            printf("[%d] : %d\n", iop_left, istk[iop_left]);

        ++ip;
        execute_next(prog[ip]);

    dump_istktop:
        printf("\ninteger stack top: %d\n", istk[isp]);
        ++ip;
        execute_next(prog[ip]);

    dump_boolflag:
        printf("\nBOOL FLAG: %d\n", boolflag);

        ++ip;
        execute_next(prog[ip]);

    dump_cstk:
        printf("\nCALL STACK DUMP:\ncsp: %d\n\n", csp);

        for (iop_left=0; iop_left < CALLSTACK_SIZE; ++iop_left)
            printf("[%d] : %d\n", iop_left, cstk.stk[iop_left]);

        ++ip;
        execute_next(prog[ip]);
}

#endif
