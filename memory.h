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
#define CALL_STACK_DEPTH 100  /* defines recursion limit. */

/* Integer Memory. */
#define TOTAL_IMEM_SIZE  300  /* main memory. */
#define INT_ARRAY_MAX    100  /* defines maximum number of arrays that can be allocated. */

/* Real Memory. */
#define TOTAL_RMEM_SIZE  300
#define REAL_ARRAY_MAX   100

/* String Memory. */
#define TOTAL_SMEM_SIZE  300
#define STR_ARRAY_MAX    100

/* Machine Used Registers */
#define prv   (mem.i[0])  /* process return value  */
#define ip    (mem.i[1])  /* instruction pointer   */
#define csp   (mem.i[2])  /* integer stack pointer */
#define fin   (mem.s[0])  /* stdin string buffer   */
#define finp  (mem.s[1])  /* stdin path buffer     */
#define fout  (mem.s[2])  /* stdout string buffer  */
#define foutp (mem.s[3])  /* stdout path buffer    */

/* Memory Initialization Macro. */
#define initialize_memory() prv = ip = csp = 0;          \
                            istk = i + INT_STACK_START;  \
                            rstk = r + REAL_STACK_START; \

struct m_memory {

    /* Call Stack. */
    m_int* callstack[CALL_STACK_DEPTH];

    /* Main Memory. */
    m_int i[IMEM_SIZE];
    m_real r[RMEM_SIZE];
    m_str s[SMEM_SIZE]; /* first 4 of these cannot(shouldn't) be used by progrms. */

    /* Dynamically Allocated Array Pointer Arrays. */
    m_int* i_arr[INT_ARRAY_MAX];
    m_real* r_arr[REAL_ARRAY_MAX];
    m_str* s_arr[STR_ARRAY_MAX];

};

#endif // MEMORY_H_INCLUDED
