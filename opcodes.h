#ifndef OPCODES_H_INCLUDED
#define OPCODES_H_INCLUDED

#define JUMP             0
#define JUMP_EQL         1
#define JUMP_NEQL        2
#define JUMP_GT_EQL      3
#define JUMP_LT_EQL      4
#define JUMP_GT          5
#define JUMP_LT          6

#define REG_JUMP         7
#define REG_JUMP_T       8
#define REG_JUMP_F       9

#define LOOP_TIMES      10
#define LOOP_WHILE      11
#define LOOP_UNTIL      12

#define CALL            13
#define LEAVE           14

#define IREG_LOAD_C     15
#define IREG1_LOAD_C    16
#define IREG2_LOAD_C    17
#define IREG3_LOAD_C    18
#define IREG4_LOAD_C    19
#define IREG5_LOAD_C    20
#define IREG6_LOAD_C    21

#define IREG_LOAD_S     22
#define IREG1_LOAD_S    23
#define IREG2_LOAD_S    24
#define IREG3_LOAD_S    25
#define IREG4_LOAD_S    26
#define IREG5_LOAD_S    27
#define IREG6_LOAD_S    28

#define IREG_SWAP       29
#define IREG_SWAP_S     30

#define IREG_EQL        31
#define IREG_NEQL       32
#define IREG_GT_EQL     33
#define IREG_LT_EQL     34
#define IREG_GT,        35
#define IREG_LT,        36
#define IREG_AND,       37
#define IREG_OR,        38
#define IREG_NOT,       39
#define IREG_AND_BW     40
#define IREG_OR_BW      41
#define IREG_XOR_BW     42
#define IREG_NOT_BW     43
#define IREG_LSHIFT_BW  44
#define IREG_RSHIFT_BW  45

#define IREG_INC        46
#define IREG_DEC        47
#define IREG_ADD        48
#define IREG_SUB        49
#define IREG_MUL        50
#define IREG_DIV        51
#define IREG_MOD        52

#define ISTK_PUSH_C     53

#define ISTK_PUSH_R     54
#define ISTK_PUSH_R1    55
#define ISTK_PUSH_R2    56
#define ISTK_PUSH_R3    57
#define ISTK_PUSH_R4    58
#define ISTK_PUSH_R5    59
#define ISTK_PUSH_R6    60

#define ISTK_POP        61
#define ISTK_SWAP       62
#define ISTK_RESET      63

#define ISTK_LOAD_C     64
#define ISTK_LOAD_R     65
#define ISTK_LOAD_R1    66
#define ISTK_LOAD_R2    67
#define ISTK_LOAD_R3    68
#define ISTK_LOAD_R4    69
#define ISTK_LOAD_R5    70
#define ISTK_LOAD_R6    71

#define ISTK_EQL        72
#define ISTK_NEQL       73
#define ISTK_GT_EQL     74
#define ISTK_LT_EQL     75
#define ISTK_GT         76
#define ISTK_LT         77
#define ISTK_AND        78
#define ISTK_OR         79
#define ISTK_NOT        80

#define ISTK_INC        81
#define ISTK_DEC        82
#define ISTK_ADD        83
#define ISTK_SUB        84
#define ISTK_MUL        85
#define ISTK_DIV        86
#define ISTK_MOD        87
#define ISTK_INV        88

#define DIE             89

#define DUMP_ISTK       90
#define DUMP_ISTKTOP    91
#define DUMP_CSTK       92


#define build_optable() static void* optable[255] = { \
                                &&jump,               \
                                &&jump_eql,           \
                                &&jump_neql,          \
                                &&jump_gt_eql,        \
                                &&jump_lt_eql,        \
                                &&jump_gt,            \
                                &&jump_lt,            \
                                &&reg_jump,           \
                                &&reg_jump_t,         \
                                &&reg_jump_f,         \
                                &&loop_times,         \
                                &&loop_while,         \
                                &&loop_until,         \
                                &&call,               \
                                &&leave,              \
                                &&ireg_load_c,        \
                                &&ireg1_load_c,       \
                                &&ireg2_load_c,       \
                                &&ireg3_load_c,       \
                                &&ireg4_load_c,       \
                                &&ireg5_load_c,       \
                                &&ireg6_load_c,       \
                                &&ireg_load_s,        \
                                &&ireg1_load_s,       \
                                &&ireg2_load_s,       \
                                &&ireg3_load_s,       \
                                &&ireg4_load_s,       \
                                &&ireg5_load_s,       \
                                &&ireg6_load_s,       \
                                &&ireg_swap,          \
                                &&ireg_swap_s,        \
                                &&ireg_eql,           \
                                &&ireg_neql,          \
                                &&ireg_gt_eql,        \
                                &&ireg_lt_eql,        \
                                &&ireg_gt,            \
                                &&ireg_lt,            \
                                &&ireg_and,           \
                                &&ireg_or,            \
                                &&ireg_not,           \
                                &&ireg_AND_BW,        \
                                &&ireg_OR_BW,         \
                                &&ireg_XOR_BW,        \
                                &&ireg_NOT_BW,        \
                                &&ireg_LSHIFT_BW,     \
                                &&ireg_LRIGHT_BW,     \
                                &&ireg_inc,           \
                                &&ireg_dec,           \
                                &&ireg_add,           \
                                &&ireg_sub,           \
                                &&ireg_mul,           \
                                &&ireg_div,           \
                                &&ireg_mod,           \
                                &&istk_push_c,        \
                                &&istk_push_r,        \
                                &&istk_push_r1,       \
                                &&istk_push_r2,       \
                                &&istk_push_r3,       \
                                &&istk_push_r4,       \
                                &&istk_push_r5,       \
                                &&istk_push_r6,       \
                                &&istk_pop,           \
                                &&istk_pop_r,         \
                                &&istk_swap,          \
                                &&istk_reset,         \
                                &&istk_load_c,        \
                                &&istk_load_r,        \
                                &&istk_load_r1,       \
                                &&istk_load_r2,       \
                                &&istk_load_r3,       \
                                &&istk_load_r4,       \
                                &&istk_load_r5,       \
                                &&istk_load_r6,       \
                                &&istk_eql,           \
                                &&istk_neql,          \
                                &&istk_gt_eql,        \
                                &&istk_lt_eql,        \
                                &&istk_gt,            \
                                &&istk_lt,            \
                                &&istk_and,           \
                                &&istk_or,            \
                                &&istk_not,           \
                                &&istk_inc,           \
                                &&istk_dec,           \
                                &&istk_add,           \
                                &&istk_sub,           \
                                &&istk_mul,           \
                                &&istk_div,           \
                                &&istk_mod,           \
                                &&istk_inv,           \
                                &&die,                \
                                &&dump_istk,          \
                                &&dump_istktop,       \
                                &&dump_cstk           \
                        }
#endif
