#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "natives.h"


/*  Memory Sizes & Specifications.
    -------------------------------

    Memory in tvm comes in two distinct flavors. The main memory and
    dynamically allocated memory. Main memory is "built-in" memory,
    this is simply an array that is put on the stack, it is split up
    into sectors, registers, ram, stack, in that order. Dynamically
    allocated memory is an array of pointers, the size of the array
    being the absolute limit for how many arrays of memory can be
    allocated by the machine code, although the actual size of the
    allocated array has no bounds(other than those of the heap itself).

    Integers and reals share the exact same memory specifications, but
    of course they are separate, string memory is the same except there's
    no stack for strings, so registers then ram and thats it.

*/

/* Integer Memory. */
#define TOTAL_IMEM_SIZE 300          /* sets the total size of integer main memory.                           */
#define INT_REG_SIZE     25          /* (addrs 0-24) sets size of integer register memory, minimum is 5.      */
#define INT_RAM_BASE    INT_REG_SIZE /* sets the addr that ram starts at.                                     */
#define INT_STACK_BASE  200          /* sets the addr that the stack starts at.                               */
#define INT_ARRAY_MAX   100          /* sets maximum number of integer arrays that can be created at runtime. */

/* Real Memory. */
#define TOTAL_RMEM_SIZE 300
#define REAL_REG_SIZE     25
#define REAL_RAM_BASE    REAL_REG_SIZE
#define REAL_STACK_BASE  200
#define REAL_ARRAY_MAX   100

/* String Memory. */
#define TOTAL_SMEM_SIZE 300
#define STR_REG_SIZE     25
#define STR_RAM_SIZE    INT_REG_SIZE
#define STR_ARRAY_MAX   100

/* Machine Used Registers */
#define retval (mem.i[0])  /* stores return code for machine process. */
#define ip     (mem.i[1])  /* instruction pointer                     */
#define isp    (mem.i[2])  /* integer stack pointer                   */
#define rsp    (mem.i[3])  /* real stack pointer                      */
#define fin    (mem.s[0])  /* stdin string buffer                     */
#define finp   (mem.s[1])  /* stdin path buffer                       */
#define fout   (mem.s[2])  /* stdout string buffer                    */
#define foutp  (mem.s[3])  /* stdout path buffer                      */

struct m_memory {

    /* 0 retval
       1 instruction pointer
       2 int stack pointer
       3 real stack pointer */
    m_int i[IMEM_SIZE];

    /* */
    m_real r[RMEM_SIZE];

    /* 0 stdout buffer
       1 stdin buffer */
    m_str s[SMEM_SIZE];


    m_int* i_arr[INT_ARRAY_MAX];
    m_real* r_arr[REAL_ARRAY_MAX];
    m_str* s_arr[STR_ARRAY_MAX];


    /* Stack pointers.*/
    m_int* istk;
    m_real* rstk;
};

#define initialize_memory() ip = isp = rsp = ssp = retval = 0; \
                            istk = i + INT_STACK_START;        \
                            rstk = r + REAL_STACK_START;       \

#endif // MEMORY_H_INCLUDED
