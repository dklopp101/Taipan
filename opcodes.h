#ifndef OPCODES_H_INCLUDED
#define OPCODES_H_INCLUDED

#define JUMP           0
#define JUMP_T         1
#define JUMP_F         2

#define CALL           3
#define LEAVE          4

#define I_CONST        5
#define I_POP          6

#define F_CONST        7
#define F_POP          8

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
#define I_INV         21

#define F_EQL          7
#define F_NEQL         8
#define F_GT_EQL       9
#define F_LT_EQL      10
#define F_GT          11
#define F_LT          12

#define F_ADD         16
#define F_SUB         17
#define F_MUL         18
#define F_DIV         19
#define F_MOD         20
#define F_NEG         21

#define DIE           22

#define DUMP_ISTK     23
#define DUMP_ISTKTOP  24
#define DUMP_FSTK     25
#define DUMP_FSTKTOP  26
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
                                &&f_const,            \
                                &&f_pop,              \
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
                                &&i_inv,              \
                                &&f_eql,              \
                                &&f_neql,             \
                                &&f_gt_eql,           \
                                &&f_lt_eql,           \
                                &&f_gt,               \
                                &&f_lt,               \
                                &&f_add,              \
                                &&f_sub,              \
                                &&f_mul,              \
                                &&f_div,              \
                                &&f_mod,              \
                                &&f_inv,              \
                                &&die,                \
                                &&dump_istk,          \
                                &&dump_istktop,       \
                                &&dump_fstk,          \
                                &&dump_fstktop,       \
                                &&dump_boolflag,      \
                                &&dump_cstk           \
                        }
#endif
