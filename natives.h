#ifndef NATIVES_H_INCLUDED
#define NATIVES_H_INCLUDED

#define STR_MEMSIZE        256
#define RECORD_MEMBER_MAX   20

/* Native Machine Datatypes */
#define m_int  int
#define m_real double

typedef struct {
    char mem[STR_MEMSIZE];
} m_str;

/* Machine Instruction Stuff */
#define instr_opcode (instr[ip].opcode)

#define instr_iop0   (program->code[ip].i_arg[0])
#define instr_iop1   (program->code[ip].i_arg[1])
#define instr_iop2   (program->code[ip].i_arg[2])
#define instr_iop3   (program->code[ip].i_arg[3])
#define instr_iop4   (program->code[ip].i_arg[4])

#define instr_rop0   (program->code[ip].r_arg[0])
#define instr_rop1   (program->code[ip].r_arg[1])
#define instr_rop2   (program->code[ip].r_arg[2])
#define instr_rop3   (program->code[ip].r_arg[3])
#define instr_rop4   (program->code[ip].r_arg[4])

#define instr_sop0   (program->code[ip].s_arg[0])
#define instr_sop1   (program->code[ip].s_arg[1])
#define instr_sop2   (program->code[ip].s_arg[2])
#define instr_sop3   (program->code[ip].s_arg[3])
#define instr_sop4   (program->code[ip].s_arg[4])

#define INSTR_ARG_MAX 5

struct m_instr {
    int  opcode;
    m_int i_arg[INSTR_ARG_MAX];
    m_real r_arg[INSTR_ARG_MAX];
    m_str s_arg[INSTR_ARG_MAX];
};

struct m_code {
    struct m_instr* code;
    unsigned instr_count;
};

#endif // NATIVES_H_INCLUDED
