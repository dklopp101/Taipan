#ifndef NATIVES_H_INCLUDED
#define NATIVES_H_INCLUDED

#define I_FOUT_FMODE   "wb"
#define R_FOUT_FMODE   "wb"
#define S_FOUT_FMODE   "wb"

#define I_FIN_FMODE    "rb"
#define R_FIN_FMODE    "rb"
#define S_FIN_FMODE    "rb"

#define I_PRINTF_FMT   "%d"
#define R_PRINTF_FMT   "%f"
#define S_PRINTF_FMT   "%s"

#define I_SCANF_FMT    "%d"
#define R_SCANF_FMT    "%f"
#define S_SCANF_FMT    "%s"

#define TEXT_OUT_FMODE  "w"
#define TEXT_IN_FMODE   "r"

#define STR_CELL_SIZE   256
#define STR_MEMSIZE     (sizeof(char) * STR_CHARLEN)


/* Native Machine Datatypes */
#define m_int  int
#define m_real double /* looking to delete these two macros, only left now for legacy reasons. */

#define STRLEN    256
#define INT_SIZE  (sizeof(m_int))
#define REAL_SIZE (sizeof(m_real))
#define STR_SIZE  (sizeof(char) * STRLEN)


/* convinience macros used by machine.h */

#define instr_opcode ((prog + ip)->opcode)

#define instr_iop0   ((prog + ip)->i_arg[0])
#define instr_iop1   ((prog + ip)->i_arg[1])
#define instr_iop2   ((prog + ip)->i_arg[2])
#define instr_iop3   ((prog + ip)->i_arg[3])
#define instr_iop4   ((prog + ip)->i_arg[4])

#define instr_rop0   ((prog + ip)->r_arg[0])
#define instr_rop1   ((prog + ip)->r_arg[1])
#define instr_rop2   ((prog + ip)->r_arg[2])
#define instr_rop3   ((prog + ip)->r_arg[3])
#define instr_rop4   ((prog + ip)->r_arg[4])

#define instr_sop0   ((prog + ip)->s_arg[0])
#define instr_sop1   ((prog + ip)->s_arg[1])
#define instr_sop2   ((prog + ip)->s_arg[2])
#define instr_sop3   ((prog + ip)->s_arg[3])
#define instr_sop4   ((prog + ip)->s_arg[4])


/* Machine Instruction. */
struct instr_t {
    int opcode;
    int i_arg[5];
    double r_arg[5];
    char s_arg[5][256];
};

#endif // NATIVES_H_INCLUDED
