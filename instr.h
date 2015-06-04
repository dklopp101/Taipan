#ifndef INSTR_H_INCLUDED
#define INSTR_H_INCLUDED

#include "typedefs.h"

#define JUMP             0 /* implemented */
#define JUMP_T           1 /* implemented */
#define JUMP_F           2 /* implemented */
#define JUMP_EQL         3 /* implemented */
#define JUMP_NEQL        4 /* implemented */
#define JUMP_GT_EQL      5 /* implemented */
#define JUMP_LT_EQL      6 /* implemented */
#define JUMP_GT          7 /* implemented */
#define JUMP_LT          8 /* implemented */
#define CALL             9
#define RET             10
#define IREG_LOAD_C     11 /* implemented */
#define IA1_LOAD_C      12 /* implemented */
#define IA2_LOAD_C      13 /* implemented */
#define IREG_LOAD_ISTK  14 /* implemented */
#define IA1_LOAD_ISTK   15 /* implemented */
#define IA2_LOAD_ISTK   16 /* implemented */
#define IREG_SWAP       17 /* implemented */
#define IREG_SWAP_ISTK  18 /* implemented */
#define IREG_EQL        19 /* implemented */
#define IREG_NEQL       20 /* implemented */
#define IREG_GT_EQL     21 /* implemented */
#define IREG_LT_EQL     22 /* implemented */
#define IREG_GT         23 /* implemented */
#define IREG_LT         24 /* implemented */
#define IREG_AND        25 /* implemented */
#define IREG_OR         26 /* implemented */
#define IREG_NOT        27 /* implemented */
#define IREG_BW_AND     28 /* implemented */
#define IREG_BW_OR      29 /* implemented */
#define IREG_BW_XOR     30 /* implemented */
#define IREG_BW_NOT     31 /* implemented */
#define IREG_BW_LSHIFT  32 /* implemented */
#define IREG_BW_RSHIFT  33 /* implemented */
#define IREG_INC        34 /* implemented */
#define IREG_DEC        35 /* implemented */
#define IREG_ADD        36 /* implemented */
#define IREG_SUB        37 /* implemented */
#define IREG_MUL        38 /* implemented */
#define IREG_DIV        39 /* implemented */
#define IREG_MOD        40 /* implemented */
#define ISTK_PUSH_C     41 /* implemented */
#define ISTK_PUSH_IREG  42 /* implemented */
#define ISTK_PUSH_IA1   43 /* implemented */
#define ISTK_PUSH_IA2   44 /* implemented */
#define ISTK_POP        45 /* implemented */
#define ISTK_POP_IREG   46 /* implemented */
#define ISTK_POP_IA1    47 /* implemented */
#define ISTK_POP_IA2    48 /* implemented */
#define ISTK_SWAP       49 /* implemented */
#define ISTK_RESET      50 /* implemented */
#define ISTK_LOAD_C     51 /* implemented TEST THIS!! */
#define ISTK_LOAD_IREG  52 /* implemented TEST THIS!! */
#define ISTK_LOAD_IA1   53 /* implemented TEST THIS!! */
#define ISTK_LOAD_IA2   54 /* implemented TEST THIS!! */
#define ISTK_EQL        55
#define ISTK_NEQL       56
#define ISTK_GT_EQL     57
#define ISTK_LT_EQL     58
#define ISTK_GT         59
#define ISTK_LT         60
#define ISTK_AND        61
#define ISTK_OR         62
#define ISTK_NOT        63
#define ISTK_BW_AND     64
#define ISTK_BW_OR      65
#define ISTK_BW_XOR     66
#define ISTK_BW_NOT     67
#define ISTK_BW_LSHIFT  68
#define ISTK_BW_RSHIFT  69
#define ISTK_INC        70
#define ISTK_DEC        71
#define ISTK_ADD        72
#define ISTK_SUB        73
#define ISTK_MUL        74
#define ISTK_DIV        75
#define ISTK_MOD        76

#define DIE             77
#define PRINT_IREG      78
#define PRINT_IREGPOS   79
#define PRINT_BLF       80
#define PRINT_IA1       81
#define PRINT_IA2       82
#define PRINT_ISTK      83
#define PRINT_ISTKTOP   84
#define PRINT_ISTKPOS   85


#define build_optable() static void* optable[255] = { \
                                &&jump,               \
                                &&jump_t,             \
                                &&jump_f,             \
                                &&jump_eql,           \
                                &&jump_neql,          \
                                &&jump_gt_eql,        \
                                &&jump_lt_eql,        \
                                &&jump_gt,            \
                                &&jump_lt,            \
                                &&call,               \
                                &&ret,                \
                                &&ireg_load_c,        \
                                &&ia1_load_c,         \
                                &&ia2_load_c,         \
                                &&ireg_load_istk,     \
                                &&ia1_load_istk,      \
                                &&ia2_load_istk,      \
                                &&ireg_swap,          \
                                &&ireg_swap_istk,     \
                                &&ireg_eql,           \
                                &&ireg_neql,          \
                                &&ireg_gt_eql,        \
                                &&ireg_lt_eql,        \
                                &&ireg_gt,            \
                                &&ireg_lt,            \
                                &&ireg_and,           \
                                &&ireg_or,            \
                                &&ireg_not,           \
                                &&ireg_bw_and,        \
                                &&ireg_bw_or,         \
                                &&ireg_bw_xor,        \
                                &&ireg_bw_not,        \
                                &&ireg_bw_lshift,     \
                                &&ireg_bw_rshift,     \
                                &&ireg_inc,           \
                                &&ireg_dec,           \
                                &&ireg_add,           \
                                &&ireg_sub,           \
                                &&ireg_mul,           \
                                &&ireg_div,           \
                                &&ireg_mod,           \
                                &&istk_push_c,        \
                                &&istk_push_ireg,     \
                                &&istk_push_ia1,      \
                                &&istk_push_ia2,      \
                                &&istk_pop,           \
                                &&istk_pop_ireg,      \
                                &&istk_pop_ia1,       \
                                &&istk_pop_ia2,       \
                                &&istk_swap,          \
                                &&istk_reset,         \
                                &&istk_load_c,        \
                                &&istk_load_ireg,     \
                                &&istk_load_ia1,      \
                                &&istk_load_ia2,      \
                                &&istk_eql,           \
                                &&istk_neql,          \
                                &&istk_gt_eql,        \
                                &&istk_lt_eql,        \
                                &&istk_gt,            \
                                &&istk_lt,            \
                                &&istk_and,           \
                                &&istk_or,            \
                                &&istk_not,           \
                                &&istk_bw_and,        \
                                &&istk_bw_or,         \
                                &&istk_bw_xor,        \
                                &&istk_bw_not,        \
                                &&istk_bw_lshift,     \
                                &&istk_bw_rshift,     \
                                &&istk_inc,           \
                                &&istk_dec,           \
                                &&istk_add,           \
                                &&istk_sub,           \
                                &&istk_mul,           \
                                &&istk_div,           \
                                &&istk_mod,           \
                                &&die,                \
                                &&print_ireg,         \
                                &&print_iregpos,      \
                                &&print_blf,          \
                                &&print_ia1,          \
                                &&print_ia2,          \
                                &&print_istk,         \
                                &&print_istktop,      \
                                &&print_istkpos       \
                                }

struct instr_t {
    byte opcode;
    t_int iop[4];
    t_float fop[4];
};


#define opc()  (is[ip].opcode)

#define iop0   (is[ip].iop[0])
#define iop1   (is[ip].iop[1])
#define iop2   (is[ip].iop[2])

#define fop0   (is[ip].fop[0])
#define fop1   (is[ip].fop[1])
#define fop2   (is[ip].fop[2])

#define execute_next(iptr) goto *optable[is[iptr].opcode]


#endif
