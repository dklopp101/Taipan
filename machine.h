#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED

#include "natives.h"
#include "memory.h"

#define I_FOUT_FMODE "wb"
#define R_FOUT_FMODE "wb"
#define S_FOUT_FMODE "wb"

#define I_FIN_FMODE "rb"
#define R_FIN_FMODE "rb"
#define S_FIN_FMODE "rb"

#define I_PRINTF_FMT "%d"
#define R_PRINTF_FMT "%f"
#define S_PRINTF_FMT "%s"

#define I_SCANF_FMT "%d"
#define R_SCANF_FMT "%f"
#define S_SCANF_FMT "%s"

#define fetch_next_instr() goto *optable[instr_opcode]

static void* optable[] = {
    I_LOAD,
    R_LOAD,
    S_LOAD,
    I_COPY,
    R_COPY,
    S_COPY,
    JUMP,
    JUMP_T,
    JUMP_F,
    CALL,
    RET,
    I_INC,
    I_DEC,
    I_ADD,
    I_SUB,
    I_MUL,
    I_DIV,
    I_MOD,
    R_ADD,
    R_SUB,
    R_MUL,
    R_DIV,
    R_MOD,
    I_EQL,
    I_NEQL,
    R_EQL,
    R_NEQL,
    R_LT,
    R_EQL_LT,
    R_GT,
    R_EQL_GT,
    I_LT,
    I_EQL_LT,
    I_GT,
    I_EQL_GT,
    IAND,
    IOR,
    INOT,
    I_INV_BW,
    I_AND_BW,
    I_OR_BW,
    I_XOR_BW,
    S_LEN,
    I_FOUT,
    R_FOUT,
    S_FOUT,
    I_FIN,
    R_FIN,
    S_FIN,
    I_COUT,
    R_COUT,
    S_COUT,
    I_CIN,
    R_CIN,
    S_CIN,
    TEXT_OUT,
    TEXT_IN,
    LEAVE
}



static inline
int machine(struct m_instr* instr)
{
    FILE* c_fptr;
    static struct m_memory mem;
    initialize_memory();


    /* execution cycle begins here */
    START:
        fetch_next_instr();



    /* Value Loading Instructions. */
    I_LOAD:
        mem.i[instr_iop0] = instr_iop1;
        ip++;
        fetch_next_instr();

    R_LOAD:
        mem.r[instr_iop0] = instr_rop1;
        ip++;
        fetch_next_instr();

    S_LOAD:
        strcpy( mem.s[instr_iop0], instr_sop1 );
        ip++;
        fetch_next_instr();



    /* Address-Value Copying Instructions. */
    I_COPY:
        mem.i[instr_iop0] = mem.i[instr_iop1];
        ip++;
        fetch_next_instr();

    R_COPY:
        mem.r[instr_iop0] = mem.r[instr_iop1];
        ip++;
        fetch_next_instr();

    S_COPY:
        strcpy( mem.s[instr_iop0], mem.s[instr_sop1] );
        ip++;
        fetch_next_instr();



    /* Branching/Subroutine Instructions. */
    JUMP:
        ip = instr_iop0;
        fetch_next_instr();

    JUMP_T:
        ip = ( mem.i[instr_iop1] ? instr_iop0 : ip + 1 );
        fetch_next_instr();

    JUMP_F:
        ip = ( !(mem.i[instr_iop1]) ? instr_iop0 : ip + 1 );
        fetch_next_instr();

    CALL:
        callstack[csp++] = ip; /* push return address. */
        ip = mem.i[instr_iop0];
        fetch_next_instr();

    RET:
        ip = callstack[--csp]; /* set instruction pointer so we return from func. */
        fetch_next_instr();



    /* Arithmetic Instructions. */
        /* integer arithmetic */
    I_INC:
        (mem.i[instr_iop0])++;
        ip++;
        fetch_next_instr();

    I_DEC:
        (mem.i[instr_iop0])--;
        ip++;
        fetch_next_instr();

    I_ADD:
        mem.i[instr_iop0] = mem.i[instr_iop1] + mem.i[instr_iop2];
        ip++;
        fetch_next_instr();

    I_SUB:
        mem.i[instr_iop0] = mem.i[instr_iop1] - mem.i[instr_iop2];
        ip++;
        fetch_next_instr();

    I_MUL:
        mem.i[instr_iop0] = mem.i[instr_iop1] * mem.i[instr_iop2];
        ip++;
        fetch_next_instr();

    I_DIV:
        mem.i[instr_iop0] = (m_int) mem.i[instr_iop1] / mem.i[instr_iop2];
        ip++;
        fetch_next_instr();
    I_MOD:
        mem.i[instr_iop0] = mem.i[instr_iop1] % mem.i[instr_iop2];
        ip++;
        fetch_next_instr();


        /* real arithmetic */
    R_ADD:
        mem.r[instr_iop0] = mem.r[instr_iop1] + mem.r[instr_iop2];
        ip++;
        fetch_next_instr();

    R_SUB:
        mem.r[instr_iop0] = mem.r[instr_iop1] - mem.r[instr_iop2];
        ip++;
        fetch_next_instr();

    R_MUL:
        mem.r[instr_iop0] = mem.r[instr_iop1] * mem.r[instr_iop2];
        ip++;
        fetch_next_instr();

    R_DIV:
        mem.r[instr_iop0] = mem.r[instr_iop1] / mem.r[instr_iop2];
        ip++;
        fetch_next_instr();

    R_MOD:
        mem.r[instr_iop0] = mem.r[instr_iop1] % mem.r[instr_iop2];
        ip++;
        fetch_next_instr();



    /* Logical Instructions.*/
        /* integer logic */
    I_EQL:
        mem.i[instr_iop0] = mem.i[instr_iop1] == mem.i[instr_iop2];
        ip++;
        fetch_next_instr();

    I_NEQL:
        mem.i[instr_iop1] = mem.i[instr_iop1] != mem.i[instr_iop2];
        ip++;
        fetch_next_instr();

    I_LT:
        mem.i[instr_iop0] = mem.i[instr_iop1] < mem.i[instr_iop2];
        ip++;
        fetch_next_instr();

    I_EQL_LT:
        mem.i[instr_iop0] = mem.i[instr_iop1] <= mem.i[instr_iop2];
        ip++;
        fetch_next_instr();

    I_GT:
        mem.i[instr_iop0] = mem.i[instr_iop1] > mem.i[instr_iop2];
        ip++;
        fetch_next_instr();

    I_EQL_GT:
        mem.i[instr_iop0] = mem.i[instr_iop1] => mem.i[instr_iop2];
        ip++;

    IAND:
        mem.i[instr_iop0] = mem.i[instr_iop1] && mem.i[instr_iop2];
        ip++;
        fetch_next_instr();

    IOR:
        mem.i[instr_iop0] = mem.i[instr_iop1] || mem.i[instr_iop2];
        ip++;
        fetch_next_instr();

    INOT:
        mem.i[instr_iop0] = !(mem.i[instr_iop1]);
        ip++;
        fetch_next_instr();

        /* real logic */
    R_EQL:
        sprintf( gps1, "%f", mem.r[instr_iop1] );
        sprintf( gps2, "%f", mem.r[instr_iop1] );
        mem.i[instr_iop0] = strcmp(gps1, gps2) == 0;
        ip++;
        fetch_next_instr();

    R_NEQL:
        sprintf( gps1, "%f", mem.r[instr_iop1] );
        sprintf( gps2, "%f", mem.r[instr_iop1] );
        mem.i[instr_iop0] = strcmp(gps1, gps2) != 0;
        ip++;
        fetch_next_instr();

    R_LT:

    R_EQL_LT:

    R_GT:

    R_EQL_GT:

    /* Bitwise Instructions. */
    I_INV_BW:
        mem.i[instr_iop0] = ~(mem.i[instr_iop0]);
        ip++;
        fetch_next_instr();

    I_AND_BW:
        mem.i[instr_iop0] = mem.i[instr_iop1] & mem.i[instr_iop2];
        ip++;
        fetch_next_instr();

    I_OR_BW:
        mem.i[instr_iop0] = mem.i[instr_iop1] | mem.i[instr_iop2];
        ip++;
        fetch_next_instr();

    I_XOR_BW:
        mem.i[instr_iop0] = mem.i[instr_iop1] ^ mem.i[instr_iop2];
        ip++;
        fetch_next_instr();



    /* All String Instructions. */
    S_CH_EQL:
        mem.i[instr_iop0] = mem.s[instr_iop1][instr_iop2] == (mem.s[instr_iop3][instr_iop4];
        ip++;
        fetch_next_instr();

    S_CH_NEQL:
        mem.i[instr_iop0] = mem.s[instr_iop1][instr_iop2] != (mem.s[instr_iop3][instr_iop4];
        ip++;
        fetch_next_instr();

    S_CH_COPY:
        mem.s[instr_iop0][instr_iop1] = mem.s[instr_iop2][mem.s[instr_iop3];
        ip++;
        fetch_next_instr();

    S_LEN:
        mem.i[instr_iop0] = strlen(mem.s[instr_sop1]);
        ip++;
        fetch_next_instr();

    S_JOIN:
        strcat(mem.s[instr_iop0], mem.s[instr_iop1]);
        ip++;
        fetch_next_instr();



    /* All stdin/out related instructions. */
    I_FOUT
        c_fptr = fopen( foutp, I_FOUT_FMODE );
        fwrite( &imem[instr_iop0], sizeof(m_int), 1, c_fptr );
        fclose(c_fptr);
        ip++;
        fetch_next_instr();

    R_FOUT
        c_fptr = fopen( foutp, R_FOUT_FMODE );
        fwrite( &rmem[instr_iop0], sizeof(m_real), 1, c_fptr );
        fclose(c_fptr);
        ip++;
        fetch_next_instr();

    S_FOUT:
        c_fptr = fopen( foutp, S_FOUT_FMODE );
        fwrite( &smem[instr_iop0], sizeof(m_str_struct), 1, c_fptr );
        fclose(c_fptr);
        ip++;
        fetch_next_instr();

    I_FIN
        c_fptr = fopen( foutp, I_FIN_FMODE );
        fread( &imem[instr_iop0], sizeof(m_int), 1, c_fptr );
        fclose(c_fptr);
        ip++;
        fetch_next_instr();

    R_FIN
        c_fptr = fopen( foutp, R_FIN_FMODE );
        fread( &rmem[instr_iop0], sizeof(m_int), 1, c_fptr );
        fclose(c_fptr);
        ip++;
        fetch_next_instr();

    S_FIN:
        c_fptr = fopen( foutp, S_FIN_FMODE );
        fwrite( &smem[instr_iop0], sizeof(m_str_struct), 1, c_fptr );
        fclose(c_fptr);
        ip++;
        fetch_next_instr();

    I_COUT
        printf( "%d", mem.i[instr_iop0] );
        ip++;
        fetch_next_instr();

    R_COUT
        printf( "%d", mem.r[instr_iop0] );
        ip++;
        fetch_next_instr();

    S_COUT:
        printf( "%s", mem.s[instr_iop0] );
        ip++;
        fetch_next_instr();

    I_CIN
        scanf( "%d", mem.i[instr_iop0] );
        ip++;
        fetch_next_instr();

    R_CIN
        scanf( "%f", mem.r[instr_iop0] );
        ip++;
        fetch_next_instr();

    S_CIN:
        scanf( "%s", mem.s[instr_iop0] );
        ip++;
        fetch_next_instr();

    TEXT_OUT:
        c_fptr = fopen( foutp, TEXT_OUT_FMODE );
        fputs( &smem[instr_iop0], c_fptr );
        fclose(c_fptr);
        ip++;
        fetch_next_instr();

    TEXT_IN:
        c_fptr = fopen( foutp, TEXT_IN_FMODE );
        fgets( &smem[instr_iop0], c_fptr );
        fclose(c_fptr);
        ip++;
        fetch_next_instr();


    LEAVE:
        return retval;

}

#endif // MACHINE_H_INCLUDED

