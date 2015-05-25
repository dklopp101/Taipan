#ifndef OPCODES_H_INCLUDED
#define OPCODES_H_INCLUDED

#define JUMP           0
#define JUMP_T         1
#define JUMP_F         2

#define CALL           3
#define LEAVE          4

#define I_CONST        5
#define I_POP          6

#define I_EQL          7
#define I_NEQL         8
#define I_GT_EQL       9
#define I_LT_EQL      10
#define I_GT          11
#define I_LT          12
#define I_AND         13
#define I_OR          14
#define I_NOT         15

#define I_ADD         16
#define I_SUB         17
#define I_MUL         18
#define I_DIV         19
#define I_MOD         20
#define I_NEG         21

#define DIE           22

#define DUMP_ISTK     23
#define DUMP_ISTKTOP  24
#define DUMP_BOOLFLAG 25
#define DUMP_CSTK     26

#define build_optable() static void* optable[255] = { \
                                &&jump,               \
                                &&jump_t,             \
                                &&jump_f,             \
                                &&call,               \
                                &&leave,              \
                                &&i_const,            \
                                &&i_pop,              \
                                &&i_eql,              \
                                &&i_neql,             \
                                &&i_gt_eql,           \
                                &&i_lt_eql,           \
                                &&i_gt,               \
                                &&i_lt,               \
                                &&i_and,              \
                                &&i_or,               \
                                &&i_not,              \
                                &&i_add,              \
                                &&i_sub,              \
                                &&i_mul,              \
                                &&i_div,              \
                                &&i_mod,              \
                                &&i_neg,              \
                                &&die,                \
                                &&dump_istk,          \
                                &&dump_istktop,       \
                                &&dump_boolflag,      \
                                &&dump_cstk           \
                        }
#endif
