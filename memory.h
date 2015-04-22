#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "natives.h"

#define prv   (i_mem.vm[0])  /* process return value  */
#define ip    (i_mem.vm[1])  /* instruction pointer   */
#define csp   (i_mem.vm[2])  /* integer stack pointer */
#define isp   (i_mem.vm[3])
#define rsp   (i_mem.vm[4])

#define fin   (s_mem.vm[0])  /* stdin string buffer   */
#define finp  (s_mem.vm[1])  /* stdin path buffer     */
#define fout  (s_mem.vm[2])  /* stdout string buffer  */
#define foutp (s_mem.vm[3])  /* stdout path buffer    */


#define INT_VM_MEM_SIZE           10
#define CALLSTACK_SIZE           200 /* defines subroutine recursion limit. */

#define INT_MAIN_MEM_SIZE        500
#define INT_STACK_MEM_SIZE       200

#define REAL_MAIN_MEM_SIZE       500
#define REAL_STACK_MEM_SIZE      200

#define STR_MAIN_MEM_SIZE        150
#define #define STR_VM_MEM_SIZE    5

/* Memory Initialization Macro. */
#define initialize_memory() prv = ip = csp = isp = rsp = 0; \
                            i_mem.memptr = i_mem.main;      \
                            r_mem.memptr = r_mem.main;      \
                            s_mem.memptr = s_mem.main;      \


struct int_memory {
    m_int *memptr;
    m_int main[INT_MAIN_MEM_SIZE];
    m_int vm[INT_VM_MEM_SIZE];
    m_int stack[INT_STACK_MEM_SIZE];
    m_int callstack[CALLSTACK_SIZE];
};

struct real_memory {
    m_real *memptr;
    m_real main[REAL_MAIN_MEM_SIZE];
    m_real stack[REAL_STACK_MEM_SIZE];
};

struct string_memory {
    m_str *memptr;
    m_str main[STR_MAIN_MEM_SIZE];
    m_str vm[STR_VM_MEM_SIZE];
};

#endif // MEMORY_H_INCLUDED
