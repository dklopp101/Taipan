#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

#include "typedefs.h"
#include "instr.h"


/* Register Macros. */
#define prc (ireg[0]) /* process return code                        */
#define ia1 (ireg[1]) /* integer arithmetic operand 1/left.         */
#define ia2 (ireg[2]) /* integer arithmetic operand 2/right.        */
#define blf (ireg[3]) /* boolean flag                               */
#define isp (ireg[4]) /* integer stack pointer/counter.             */
#define fsp (ireg[5]) /* float stack pointer/counter.               */
#define csp (ireg[6]) /* callstack pointer/counter.                 */


/* Integer Stack Macros. */
#define istk_top       (istk[isp])
#define istk_pop()     (istk[(isp--)])
#define istk_push(v)   (istk[(++isp)] = v)


/* Float Stack Macros. */
#define fstk_top       (fstk[fsp])
#define fstk_pop()     (fstk[(fsp--)])
#define fstk_push(v)   (fstk[(++fsp)] = v)


// Callstack / Subroutines / Exceptions.
// ========================================
#define cstk_top        (cstk[csp])
#define cstk_pop()      (cstk[(csp--)])
#define cstk_push(v)    (cstk[(++csp)] = v)

// These macros always contain their namesake beloning
// to the currently executing runtime frame.
#define frame_base      (frame_ptr[0]) // cstk index that is start of frame.
#define frame_outerbase (frame_ptr[1]) // cstk index of outer caller frame.
#define frame_start     (frame_ptr[2]) // instr index of the first instr of subr.
#define frame_ret       (frame_ptr[3]) // instr index of the return instr after subr.
#define frame_hcount    (frame_ptr[4]) // integer to keep count of exception handlers.


// To call a subroutine it's runtime-frame must be pushed onto
// the callstack, this frame simply provides a indexable way to
// access any runtime properties of the subroutine.
//
// First the frame's base is pushed, csp+1 is where this will get
// pushed onto the callstack, so that's what's pushed. Next we push
// the name frame's outerbase, which is simply the base of the outer
// frame(the caller), so we simply push frame_base. Next we push the
// macro param, start, which is the instruction index that is the
// first instruction of the subroutine. Last push is a 0 to initialise
// the exception handler count.
//
// Very last thing we do is set the frame_ptr. This being set makes the
// frame officially the current runtime frame
#define push_frame(start)        cstk_push(csp);           \
                                 cstk_push(frame_base);    \
                                 cstk_push(start);         \
                                 cstk_push(ip+1);          \
                                 cstk_push(0);             \
                                 frame_ptr = cstk + (csp-4)   \



// When a subroutine returns, the runtime frame for that particular call
// is no longer the current frame, so we have to pop it off the stack.
// In reality all we do is update csp and the frame pointer.
#define pop_frame() csp = frame_outerbase;              \
                    frame_ptr = cstk + frame_outerbase


#define initialize_cstk() cstk[0] = 0;      \
                          cstk[1] = 0;      \
                          cstk[2] = 0;      \
                          cstk[3] = 0;      \
                          cstk[4] = 0;      \
                          csp = 4;          \
                          frame_ptr = cstk


inline int tprocess(struct instr_t* is)
{
    // Macro call containing the optable for computed gotos.
    build_optable();

    t_int ibuf;
    t_float fbuf;

    int i, ip = 0;
    t_int ireg[IREG_SIZE];
    isp = fsp = -1;

    t_int istk[ISTK_SIZE];
    t_float fstk[FSTK_SIZE];

    int cstk[CSTK_SIZE];
    int* frame_ptr;
    initialize_cstk();

    // Execution starts here at instruction 0.
    execute_next(ip);


    // Jumps/Branching.
    jump:
        ip = iop0;
        execute_next(ip);

    jump_t:
        if (blf)
            ip = iop0;
        else
            ++ip;
        execute_next(ip);

    jump_f:
        if (!blf)
            ip = iop0;
        else
            ++ip;
        execute_next(ip);

    jump_eql:
        if (ia1 == ia2)
            ip = iop0;
        else
            ++ip;
        execute_next(ip);

    jump_neql:
        if (ia1 != ia2)
            ip = iop0;
        else
            ++ip;
        execute_next(ip);

    jump_gt_eql:
        if (ia1 >= ia2)
            ip = iop0;
        else
            ++ip;
        execute_next(ip);

    jump_lt_eql:
        if (ia1 <= ia2)
            ip = iop0;
        else
            ++ip;
        execute_next(ip);

    jump_gt:
        if (ia1 > ia2)
            ip = iop0;
        else
            ++ip;
        execute_next(ip);

    jump_lt:
        if (ia1 < ia2)
            ip = iop0;
        else
            ++ip;
        execute_next(ip);

    // Subroutines.
    call:
        push_frame(iop0);
        ip = iop0;
        execute_next(ip);

    ret:
        ip = frame_ret;
        pop_frame();
        execute_next(ip);

    // Registers.
    ireg_load_c:
        ireg[iop0] = iop1;
        execute_next(++ip);

    ia1_load_c:
        ia1 = iop0;
        execute_next(++ip);

    ia2_load_c:
        ia2 = iop0;
        execute_next(++ip);

    ireg_load_istk:
        ireg[iop0] = istk[iop1];
        execute_next(++ip);

    ia1_load_istk:
        ia1 = istk[iop1];
        execute_next(++ip);

    ia2_load_istk:
        ia2 = istk[iop1];
        execute_next(++ip);

    ireg_swap:
        ibuf = ireg[iop0];
        ireg[iop0] = ireg[iop1];
        ireg[iop1] = ibuf;
        execute_next(++ip);

    ireg_swap_istk:
        ibuf = ireg[iop0];
        ireg[iop0] = istk[i];
        istk[i] = ibuf;
        execute_next(++ip);

    ireg_eql:
        blf = ia1 == ia2;
        execute_next(++ip);

    ireg_neql:
        blf = ia1 != ia2;
        execute_next(++ip);

    ireg_gt_eql:
        blf = ia1 >= ia2;
        execute_next(++ip);

    ireg_lt_eql:
        blf = ia1 <= ia2;
        execute_next(++ip);

    ireg_gt:
        blf = ia1 > ia2;
        execute_next(++ip);

    ireg_lt:
        blf = ia1 < ia2;
        execute_next(++ip);

    ireg_and:
        blf = ia1 && ia2;
        execute_next(++ip);

    ireg_or:
        blf = ia1 || ia2;
        execute_next(++ip);

    ireg_not:
        blf = !ia1;
        execute_next(++ip);

    ireg_bw_and:
        blf &= ia2;
        execute_next(++ip);

    ireg_bw_or:
        ia1 |= ia2;
        execute_next(++ip);

    ireg_bw_xor:
        ia1 ^= ia2;
        execute_next(++ip);

    ireg_bw_not:
        ia1 = (~(ia1)) + 1;
        execute_next(++ip);

    ireg_bw_lshift:
        ia1 <<= ia2;
        execute_next(++ip);

    ireg_bw_rshift:
        ia1 >>= ia2;
        execute_next(++ip);

    ireg_inc:
        ++(ia1);
        execute_next(++ip);

    ireg_dec:
        --(ia1);
        execute_next(++ip);

    ireg_add:
        ia1 += ia2;
        execute_next(++ip);

    ireg_sub:
        ia1 -= ia2;
        execute_next(++ip);

    ireg_mul:
        ia1 *= ia2;
        execute_next(++ip);

    ireg_div:
        ia1 /= ia2;
        execute_next(++ip);

    ireg_mod:
        ia1 %= ia2;
        execute_next(++ip);



    /* STACK INSTRUCTIONS. */
    istk_push_c:
        istk_push(iop0);
        execute_next(++ip);

    istk_push_ireg:
        istk_push(ireg[iop0]);
        execute_next(++ip);

    istk_push_ia1:
        istk_push(ia1);
        execute_next(++ip);

    istk_push_ia2:
        istk_push(ia2);
        execute_next(++ip);

    istk_pop:
        --isp;
        execute_next(++ip);

    istk_pop_ireg:
        ireg[iop0] = istk_pop();
        execute_next(++ip);

    istk_pop_ia1:
        ia1 = istk_pop();
        execute_next(++ip);

    istk_pop_ia2:
        ia2 = istk_pop();
        execute_next(++ip);

    istk_swap:
        ibuf = istk[iop0];
        istk[iop0] = istk[iop1];
        istk[iop1] = ibuf;
        execute_next(++ip);

    istk_reset:
        isp = -1;
        execute_next(++ip);

    istk_load_c:
        istk[iop0] = iop1;
        execute_next(++ip);

    istk_load_ireg:
        istk[iop0] = ireg[iop1];
        execute_next(++ip);

    istk_load_ia1:
        istk[iop0] = ia1;
        execute_next(++ip);

    istk_load_ia2:
        istk[iop0] = ia2;
        execute_next(++ip);

    istk_eql:
        blf = istk_pop() == istk_pop();
        execute_next(++ip);

    istk_neql:
        blf = istk_pop() != istk_pop();
        execute_next(++ip);

    istk_gt_eql:
        blf = istk_pop() >= istk_pop();
        execute_next(++ip);

    istk_lt_eql:
        blf = istk_pop() <= istk_pop();
        execute_next(++ip);

    istk_gt:
        blf = istk_pop() > istk_pop();
        execute_next(++ip);

    istk_lt:
        blf = istk_pop() < istk_pop();
        execute_next(++ip);

    istk_and:
        blf = istk_pop() && istk_pop();
        execute_next(++ip);

    istk_or:
        blf = istk_pop() || istk_pop();
        execute_next(++ip);

    istk_not:
        blf = !istk_pop();
        execute_next(++ip);

    istk_bw_and:
        istk_top = istk_pop() & istk_top;
        execute_next(++ip);

    istk_bw_or:
        istk_top = istk_pop() | istk_top;
        execute_next(++ip);

    istk_bw_xor:
        istk_top = istk_pop() ^ istk_top;
        execute_next(++ip);

    istk_bw_not:
        istk_top = ~istk_pop();
        execute_next(++ip);

    istk_bw_lshift:
        istk_top = istk_pop() << istk_top;
        execute_next(++ip);

    istk_bw_rshift:
        istk_top = istk_pop() >> istk_top;
        execute_next(++ip);

    istk_inc:
        ++istk_top;
        execute_next(++ip);

    istk_dec:
        --istk_top;
        execute_next(++ip);

    istk_add:
        istk_top = istk_pop() + istk_top;
        execute_next(++ip);

    istk_sub:
        istk_top = istk_pop() - istk_top;
        execute_next(++ip);

    istk_mul:
        istk_top = istk_pop() * istk_top;
        execute_next(++ip);

    istk_div:
        istk_top = istk_pop() / istk_top;
        execute_next(++ip);

    istk_mod:
        istk_top = istk_pop() % istk_top;
        execute_next(++ip);

    die:
        return 0;


    /* PRINT INSTRUCTIONS. */
    print_ireg:
        printf("\ninteger registers printout\n-----------------------\n");
        for (i=0; i < IREG_SIZE; ++i)
            printf("\nireg%d: %d", i, ireg[i]);
        execute_next(++ip);

    print_iregpos:
        printf("\ireg[%d]: %d", iop0, ireg[iop0]);
        execute_next(++ip);

    print_blf:
        printf("\nblf: %d", blf);
        execute_next(++ip);

    print_ia1:
        printf("\nia1: %d", ia1);
        execute_next(++ip);

    print_ia2:
        printf("\ia2: %d", ia2);
        execute_next(++ip);

    print_istk:
        printf("\ninteger stack printout\n-----------------------\nisp: %d\n\n", isp);
        for (i=0; i < ISTK_SIZE; ++i)
            printf("ISTACK[%d]: %d\n", i, istk[i]);
        execute_next(++ip);

    print_istktop:
        printf("\nISTACK[top]: %d --- isp: %d", istk[isp], isp);
        execute_next(++ip);

    print_istkpos:
        printf("\nISTK[%d]: %d", iop0, istk[iop0]);
        execute_next(++ip);
}


#endif
