#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "natives.h"

/* Process Return Codes. */
#define EXIT_SUCCESS_RC 0
#define RETVAL_INIT_VALUE  EXIT_SUCCESS_RC

/* VM Memory-Related Size Specifications. */
#define INT_REG_SIZE   15 /* sets size of integer register memory, minimum is 4. */
#define INT_RAM_SIZE  200 /* sets size of integer ram memory, no minimum. */
#define IMEM_SIZE     INT_REG_SIZE + INT_RAM_SIZE
#define INT_ARRAY_MAX 100 /* sets maximum number of integer arrays that can be created at runtime. */

#define REAL_REG_SIZE   15 /* sets size of real register memory, minimum is 4. */
#define REAL_RAM_SIZE  200 /* sets size of real ram memory, no minimum. */
#define RMEM_SIZE     REAL_REG_SIZE + REAL_RAM_SIZE
#define REAL_ARRAY_MAX 100 /* sets maximum number of real arrays that can be created at runtime. */

#define TEXT_REG_SIZE   15 /* sets size of real register memory, minimum is 4. */
#define TEXT_RAM_SIZE  200 /* sets size of real ram memory, no minimum. */
#define TMEM_SIZE     TEXT_REG_SIZE + TEXT_RAM_SIZE
#define TEXT_ARRAY_MAX 100 /* sets maximum number of real arrays that can be created at runtime. */

/* Machine-Allocated registers. */
#define retval  (mem.i[0])  /* stores return code for machine process. */
#define ip      (mem.i[1]) /* instruction pointer register */

struct m_memory {
    m_int i[IMEM_SIZE];
    m_real r[RMEM_SIZE];
    m_text t[TMEM_SIZE];

    m_int* i_arr[INT_ARRAY_MAX];
    m_real* r_arr[REAL_ARRAY_MAX];
    m_text* t_arr[TEXT_ARRAY_MAX];
};

#define initialize_memory() ip = 0; retval = RETVAL_INIT_VALUE

#endif // MEMORY_H_INCLUDED
