#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "natives.h"

#define prv        (i_mem.vm[0])     /* process return value          */
#define ip         (i_mem.vm[1])     /* instruction pointer           */
#define csp        (i_mem.vm[2])     /* subroutine call stack pointer */
#define isp        (i_mem.vm[3])     /* integer stack pointer         */
#define rsp        (i_mem.vm[4])     /* real stack pointer            */
#define imem       (i_mem.data)      /* integer memory access pointer */
#define rmem       (r_mem.data)      /* real memory access pointer    */
#define smem       (s_mem.data)      /* string memory access pointer  */
#define src_stack  (i_mem.callstack) /* subroutine callstack          */
#define istack     (i_mem.stack)     /* integer stack access pointer  */
#define rstack     (r_mem.stack)     /* real stack access pointer     */
#define c_in       (s_mem.vm[0])     /* console-in buffer             */
#define c_out      (s_mem.vm[2])     /* console-out-buffer            */
#define c_in_p     (s_mem.vm[3])     /* console-in path buffer        */
#define c_out_p    (s_mem.vm[4])     /* console-out path buffer       */
#define f_in       (s_mem.vm[5])     /* file-in buffer                */
#define f_out      (s_mem.vm[6])     /* file-out buffer               */
#define gpub1      (s_mem.vm[9])     /*  */
#define gpub2      (s_mem.vm[10])

#define CALLSTACK_SIZE           200 /* defines subroutine recursion limit. */

#define INT_VM_MEM_SIZE           10
#define INT_MAIN_MEM_SIZE        500
#define INT_STACK_MEM_SIZE       200

#define REAL_MAIN_MEM_SIZE       500
#define REAL_STACK_MEM_SIZE      200

#define STR_MAIN_MEM_SIZE        150
#define STR_VM_MEM_SIZE            5

#define FIN_BUF_SIZE             256
#define FOUT_BUF_SIZE            256

#define initialize_memory()     if (m_recursion_level == 1) prv = ip = csp = isp = rsp = 0


struct int_memory {
    int data[INT_MAIN_MEM_SIZE];
    int vm[INT_VM_MEM_SIZE];
    int stack[INT_STACK_MEM_SIZE];
    int callstack[CALLSTACK_SIZE];
};

struct real_memory {
    double data[REAL_MAIN_MEM_SIZE];
    double stack[REAL_STACK_MEM_SIZE];
};

struct string_memory {
    char data[STR_MAIN_MEM_SIZE][STR_MEMSIZE];
    char vm[STR_VM_MEM_SIZE][STR_MEMSIZE];
};

#endif // MEMORY_H_INCLUDED
