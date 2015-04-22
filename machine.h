#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED

#include "natives.h"
#include "memory.h"

#define I_FOUT_FMODE "wb"
#define R_FOUT_FMODE "wb"
#define S_FOUT_FMODE "wb"

#define I_FIN_FMODE  "rb"
#define R_FIN_FMODE  "rb"
#define S_FIN_FMODE  "rb"

#define I_PRINTF_FMT "%d"
#define R_PRINTF_FMT "%f"
#define S_PRINTF_FMT "%s"

#define I_SCANF_FMT  "%d"
#define R_SCANF_FMT  "%f"
#define S_SCANF_FMT  "%s"

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
int machine(struct m_program* prog);
{
    FILE* c_fptr;

    static struct int_memory i_mem;
    static struct real_memory r_mem;
    static struct str_memory s_mem;
    initialize_memory();


    /* Value Loading Instructions. */
    I_LOAD:
        i_mem.memptr[instr_iop0] = instr_iop1;
        ip++;
        fetch_next_instr();

    R_LOAD:
        r_mem.memptr[instr_iop0] = instr_rop1;
        ip++;
        fetch_next_instr();

    S_LOAD:
        strcpy(s_mem.memptr[instr_iop0], instr_sop1);
        ip++;
        fetch_next_instr();


    /* Address-Value Copying Instructions. */
    I_COPY:
        i_mem.memptr[instr_iop0] = i_mem.memptr[instr_iop1];
        ip++;
        fetch_next_instr();

    R_COPY:
        r_mem.memptr[instr_iop0] = r_mem.memptr[instr_iop1];
        ip++;
        fetch_next_instr();

    S_COPY:
        strcpy(s_mem.memptr[instr_iop0], s_mem[instr_sop1]);
        ip++;
        fetch_next_instr();


    /* Branching/Subroutine Instructions. */
    JUMP:
        ip = instr_iop0;
        fetch_next_instr();

    JUMP_T:
        ip = i_mem.memptr[instr_iop1] ? instr_iop0 : ip + 1;
        fetch_next_instr();

    JUMP_F:
        ip = (!(i_mem.memptr[instr_iop1])) ? instr_iop0 : ip + 1;
        fetch_next_instr();

    CALL:
        i_mem.callstack[csp++] = ip + 1; /* push return address. */
        ip = instr_iop0;
        fetch_next_instr();

    RET:
        ip = i_mem.callstack[--csp]; /* set instruction pointer so we return from func. */
        fetch_next_instr();


    /* Stack Instructions. */
    I_PUSH_CNST:
        i_s_memtack[isp++] = instr_iop0;
        ip++;
        fetch_next_instr;

    I_PUSH:
        i_s_memtack[isp++] = i_mem.memptr[instr_iop0];
        ip++;
        fetch_next_instr;

    I_POP:
        i_mem.memptr[instr_iop0] = i_s_memtack[--isp];
        ip++;
        fetch_next_instr();

    R_PUSH_CNST:
        r_s_memtack[rsp++] = instr_rop0;
        ip++;
        fetch_next_instr;

    R_PUSH:
        r_s_memtack[rsp++] = r_mem.memptr[instr_iop0];
        ip++;
        fetch_next_instr;

    R_POP:
        r_mem.memptr[instr_iop0] = r_s_memtack[--isp];
        ip++;
        fetch_next_instr();


    /* Arithmetic Instructions. */
        /* integer arithmetic */
    I_INC:
        (i_mem[instr_iop0])++;
        ip++;
        fetch_next_instr();

    I_DEC:
        (i_mem[instr_iop0])--;
        ip++;
        fetch_next_instr();

    I_ADD:
        i_mem[instr_iop0] = i_mem[instr_iop1] + i_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_SUB:
        i_mem[instr_iop0] = i_mem[instr_iop1] - i_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_MUL:
        i_mem[instr_iop0] = i_mem[instr_iop1] * i_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_DIV:
        i_mem[instr_iop0] = (m_int) i_mem[instr_iop1] / i_mem[instr_iop2];
        ip++;
        fetch_next_instr();
    I_MOD:
        i_mem[instr_iop0] = i_mem[instr_iop1] % i_mem[instr_iop2];
        ip++;
        fetch_next_instr();


        /* real arithmetic */
    R_ADD:
        r_mem[instr_iop0] = r_mem[instr_iop1] + r_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    R_SUB:
        r_mem[instr_iop0] = r_mem[instr_iop1] - r_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    R_MUL:
        r_mem[instr_iop0] = r_mem[instr_iop1] * r_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    R_DIV:
        r_mem[instr_iop0] = r_mem[instr_iop1] / r_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    R_MOD:
        r_mem[instr_iop0] = r_mem[instr_iop1] % r_mem[instr_iop2];
        ip++;
        fetch_next_instr();



    /* Logical Instructions.*/
        /* integer logic */
    I_EQL:
        i_mem[instr_iop0] = i_mem[instr_iop1] == i_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_NEQL:
        i_mem[instr_iop1] = i_mem[instr_iop1] != i_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_LT:
        i_mem[instr_iop0] = i_mem[instr_iop1] < i_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_EQL_LT:
        i_mem[instr_iop0] = i_mem[instr_iop1] <= i_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_GT:
        i_mem[instr_iop0] = i_mem[instr_iop1] > i_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_EQL_GT:
        i_mem[instr_iop0] = i_mem[instr_iop1] => i_mem[instr_iop2];
        ip++;

    IAND:
        i_mem[instr_iop0] = i_mem[instr_iop1] && i_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    IOR:
        i_mem[instr_iop0] = i_mem[instr_iop1] || i_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    INOT:
        i_mem[instr_iop0] = !(i_mem[instr_iop1]);
        ip++;
        fetch_next_instr();

        /* real logic */
    R_EQL:
        sprintf(gps1, "%f", r_mem[instr_iop1]);
        sprintf(gps2, "%f", r_mem[instr_iop1]);
        i_mem[instr_iop0] = strcmp(gps1, gps2) == 0;
        ip++;
        fetch_next_instr();

    R_NEQL:
        sprintf(gps1, "%f", r_mem[instr_iop1]);
        sprintf(gps2, "%f", r_mem[instr_iop1]);
        i_mem[instr_iop0] = strcmp(gps1, gps2) != 0;
        ip++;
        fetch_next_instr();

    R_LT:
        r_mem[instr_iop0] = r_mem[instr_iop1] < r_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_EQL_LT:
        r_mem[instr_iop0] = r_mem[instr_iop1] <= r_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_GT:
        r_mem[instr_iop0] = r_mem[instr_iop1] > r_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_EQL_GT:
        r_mem[instr_iop0] = r_mem[instr_iop1] => r_mem[instr_iop2];
        ip++;



    /* All String Instructions. */
    S_CH_EQL:
        i_mem[instr_iop0] = s_mem[instr_iop1][instr_iop2] == (s_mem[instr_iop3][instr_iop4];
        ip++;
        fetch_next_instr();

    S_CH_NEQL:
        i_mem[instr_iop0] = s_mem[instr_iop1][instr_iop2] != (s_mem[instr_iop3][instr_iop4];
        ip++;
        fetch_next_instr();

    S_CH_COPY:
        s_mem[instr_iop0][instr_iop1] = s_mem[instr_iop2][s_mem[instr_iop3];
        ip++;
        fetch_next_instr();

    S_LEN:
        i_mem[instr_iop0] = strlen(s_mem[instr_sop1]);
        ip++;
        fetch_next_instr();

    S_JOIN:
        strcat(s_mem[instr_iop0], s_mem[instr_iop1]);
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
        printf( "%d", i_mem[instr_iop0] );
        ip++;
        fetch_next_instr();

    R_COUT
        printf( "%d", r_mem[instr_iop0] );
        ip++;
        fetch_next_instr();

    S_COUT:
        printf( "%s", s_mem[instr_iop0] );
        ip++;
        fetch_next_instr();

    I_CIN
        scanf( "%d", i_mem[instr_iop0] );
        ip++;
        fetch_next_instr();

    R_CIN
        scanf( "%f", r_mem[instr_iop0] );
        ip++;
        fetch_next_instr();

    S_CIN:
        scanf( "%s", s_mem[instr_iop0] );
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



    /* Bitwise Instructions. */
    I_INV_BW:
        i_mem[instr_iop0] = ~(i_mem[instr_iop0]);
        ip++;
        fetch_next_instr();

    I_AND_BW:
        i_mem[instr_iop0] = i_mem[instr_iop1] & i_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_OR_BW:
        i_mem[instr_iop0] = i_mem[instr_iop1] | i_mem[instr_iop2];
        ip++;
        fetch_next_instr();

    I_XOR_BW:
        i_mem[instr_iop0] = i_mem[instr_iop1] ^ i_mem[instr_iop2];
        ip++;
        fetch_next_instr();


    LEAVE:
        return retval;

}

#endif // MACHINE_H_INCLUDED




#endif // MACHINE_H_INCLUDED
