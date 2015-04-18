#ifndef NATIVES_H_INCLUDED
#define NATIVES_H_INCLUDED

/* Native Machine Datatypes */
#define m_int  int                                  /* integer */
#define m_real double                               /* real */

#define TEXT_SIZE 250

typedef struct { char ch[TEXT_SIZE]; } m_text;  /* text */

/* Machine Instruction Stuff */
#define OPERAND_MAX 4

#define instr_opcode (instr[ip].opcode)
#define instr_op0    (instr[ip].i_op[0])
#define instr_op1    (instr[ip].i_op[1])
#define instr_op2    (instr[ip].i_op[2])
#define instr_op3    (instr[ip].i_op[3])
#define instr_op4    (instr[ip].i_op[4])

struct m_instr {
    int  opcode;
    m_int i_op[OPERAND_MAX];
    m_real r_op[OPERAND_MAX];
    m_text t_op[OPERAND_MAX];
};

#endif // NATIVES_H_INCLUDED
