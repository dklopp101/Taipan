#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED

#include "natives.h"
#include "memory.h"

static inline
int machine(struct m_instr* instr)
{
    #define fetch_next_instr() goto *optable[instr_opcode]

    static void* optable[] = {
        &&ILOAD,
        &&ICOPY,
        &&IPRINT,
        &&JUMP,
        &&JUMPT,
        &&INC,
        &&DEC,
        &&IEQL,
        &&IADD,
        &&LEAVE
    };

    static struct m_memory mem;
    initialize_memory();


    /* Execution.. */
    START:
        fetch_next_instr();

    JUMP:
        ip = instr_op0;
        fetch_next_instr();

    JUMPT:
        if (mem.i[instr_op1])
            ip = instr_op0;
        else
            ip++;
        fetch_next_instr();

    ILOAD:
        mem.i[instr_op0] = instr_op1;
        ip++;
        fetch_next_instr();

    ICOPY:
        mem.i[instr_op0] = mem.i[instr_op1];
        ip++;
        fetch_next_instr();

    IPRINT:
        printf("%d", mem.i[instr_op0]);
        ip++;
        fetch_next_instr();


    /* New Operations added on 16/4, untested. */
    INC: /* Increments the value contains at addr specified in instr_op0 */
        (mem.i[instr_op0])++;
        ip++;
        fetch_next_instr();

    DEC:
        (mem.i[instr_op0])--;
        ip++;
        fetch_next_instr();

    IEQL: /* Tests whether specified addr are equal. op0: result dest, op2: addr1, op3: addr2. */
        mem.i[instr_op0] = mem.i[instr_op1] == mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    INEQL:
        mem.i[instr_op0] = mem.i[instr_op1] != mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    /* Bitwise Instructions. */
    IBWNEG:
        mem.i[instr_op0] = ~(mem.i[instr_op0]);
        ip++;
        fetch_next_instr();

    IBWAND:
        mem.i[instr_op0] = mem.i[instr_op1] & mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    IBWOR:
        mem.i[instr_op0] = mem.i[instr_op1] | mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    IBWXOR:
        mem.i[instr_op0] = mem.i[instr_op1] ^ mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    IAND:
        mem.i[instr_op0] = mem.i[instr_op1] && mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    IOR:
        mem.i[instr_op0] = mem.i[instr_op1] || mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    INOT:
        mem.i[instr_op0] = !(mem.i[instr_op1]);
        ip++;
        fetch_next_instr();

    ILT:
        mem.i[instr_op0] = mem.i[instr_op1] < mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    IGT:
        mem.i[instr_op0] = mem.i[instr_op1] > mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    IADD:
        mem.i[instr_op0] = mem.i[instr_op1] + mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    ISUB:
        mem.i[instr_op0] = mem.i[instr_op1] - mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    IMUL:
        mem.i[instr_op0] = mem.i[instr_op1] * mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    IDIV:
        mem.i[instr_op0] = (m_int) mem.i[instr_op1] / mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    IMOD:
        mem.i[instr_op0] = mem.i[instr_op1] % mem.i[instr_op2];
        ip++;
        fetch_next_instr();

    IRAND:
        srand(time(NULL));
        mem.i[instr_op0] = rand() % (mem.i[instr_op1] + mem.i[instr_op2]);
        ip++;
        fetch_next_instr();

    LEAVE:
        return retval;
}

#endif // MACHINE_H_INCLUDED
