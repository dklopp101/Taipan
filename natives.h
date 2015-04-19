#ifndef NATIVES_H_INCLUDED
#define NATIVES_H_INCLUDED

/* Native Machine Datatypes */
#define m_int  int                                  /* integer */
#define m_real double                               /* real */

#define STR_MEMSIZE 256

typedef struct m_str_t { char mem[STR_MEMSIZE]; } m_str;  /* text */

/* Machine Instruction Stuff */
#define instr_opcode (instr[ip].opcode)

#define instr_iop0   (instr[ip].i_op[0])
#define instr_iop1   (instr[ip].i_op[1])
#define instr_iop2   (instr[ip].i_op[2])
#define instr_iop3   (instr[ip].i_op[3])
#define instr_iop4   (instr[ip].i_op[4])

#define instr_rop0   (instr[ip].r_op[0])
#define instr_rop1   (instr[ip].r_op[1])
#define instr_rop2   (instr[ip].r_op[2])
#define instr_rop3   (instr[ip].r_op[3])
#define instr_rop4   (instr[ip].r_op[4])

#define instr_sop0   (instr[ip].s_op[0])
#define instr_sop1   (instr[ip].s_op[1])
#define instr_sop2   (instr[ip].s_op[2])
#define instr_sop3   (instr[ip].s_op[3])
#define instr_sop4   (instr[ip].s_op[4])

#define INSTR_ARG_MAX 5

struct m_instr {
    int  opcode;
    m_int i_args[INSTR_ARG_MAX];
    m_real r_args[INSTR_ARG_MAX];
    m_str s_args[INSTR_ARG_MAX];
};

#endif // NATIVES_H_INCLUDED
