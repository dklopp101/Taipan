#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED

#include "memory.h"
#include "natives.h"

#define optable_jump()     goto *optable[instr_opcode]
#define fetch_next_instr() ip++; optable_jump();

inline
int machine(struct instr_t *prog)
{

        /* if this table bugs out then the instructions
           likely need && before them(no spaces). */
        static void *optable[] = {
        /* This operation table is the ABSOLUTE last call on proper
           orderings of the operation's labels. If you do change an
           operation's source code position, make sure to come to this
           table and update it and test that it's correct! */



            /* Branching Instructions / Subroutine Call System */
            &&JUMP,
            &&JUMP_T,
            &&JUMP_F,
            &&CALL,
            &&RET,

            /* Constant Loaders */
            &&I_LOAD,
            &&R_LOAD,
            &&S_LOAD,

            /* Memory Copiers */
            &&I_COPY,
            &&R_COPY,
            &&S_COPY,

            /* Stack Manipulators */
            &&I_PUSH_CNST,
            &&I_PUSH,
            &&I_POP,
            &&R_PUSH_CNST,
            &&R_PUSH,
            &&R_POP,

            /* Integer Logic */
            &&I_EQL,
            &&I_NEQL,
            &&I_LT,
            &&I_EQL_LT,
            &&I_GT,
            &&I_EQL_GT,
            &&I_AND,
            &&I_OR,
            &&I_NOT,

            /* Integer Arithmetic */
            &&I_INC,
            &&I_DEC,
            &&I_ADD,
            &&I_SUB,
            &&I_MUL,
            &&I_DIV,
            &&I_MOD,

            /* Real Arithmetic */
            &&R_ADD,
            &&R_SUB,
            &&R_MUL,
            &&R_DIV,

            /* Type Casts */
            &&I_TO_R,
            &&I_TO_S,
            &&R_TO_I,
            &&R_TO_S,
            &&S_TO_I,
            &&S_TO_R,

            /* Real Logic */
            &&R_EQL,
            &&R_NEQL,
            &&R_LT,
            &&R_GT,

            /* String Manipulators. */
            &&S_CH_EQL,
            &&S_CH_NEQL,
            &&S_CH_COPY,
            &&S_LEN,
            &&S_JOIN,

            /* Binary File IO */
            &&I_FOUT,
            &&R_FOUT,
            &&S_FOUT,
            &&I_FIN,
            &&R_FIN,
            &&S_FIN,

            /* Console IO */
            &&COUT,
            &&CIN,

            /* Text-Mode File IO */
            &&TXTOUT,
            &&TXTIN,

            /* Integer Bitwise Operations. */
            &&I_INV_BW,
            &&I_AND_BW,
            &&I_OR_BW,
            &&I_XOR_BW,

            &&LOAD_INSTR,     /* Loads new Instruction. */

            &&LEAVE           /* Stops the vm. */
        };


        FILE *fstream;

        static struct int_memory i_mem;
        static struct real_memory r_mem;
        static struct string_memory s_mem;

        static int m_recursion_level = 0;
        m_recursion_level++;

        initialize_memory();



        /* Branching/Subroutine Instructions. */
        JUMP:
            ip = instr_iop0;
            optable_jump();

        JUMP_T:
            ip = imem[instr_iop1] ? instr_iop0 : ip + 1;
            optable_jump();

        JUMP_F:
            ip = (!(imem[instr_iop1])) ? instr_iop0 : ip + 1;
            optable_jump();

        CALL:
            src_stack[csp++] = ip + 1; /* push return address. */
            ip = instr_iop0;
            optable_jump();

        RET:
            ip = src_stack[--csp]; /* set instruction pointer so we return from func. */
            optable_jump();



        /* Value Loading Instructions. */
        I_LOAD:
            imem[instr_iop0] = instr_iop1;
            fetch_next_instr();

        R_LOAD:
            rmem[instr_iop0] = instr_rop1;
            fetch_next_instr();

        S_LOAD:
            strcpy(smem[instr_iop0], instr_sop1);
            fetch_next_instr();



        /* Address-Value Copying Instructions. */
        I_COPY:
            imem[instr_iop0] = imem[instr_iop1];
            fetch_next_instr();

        R_COPY:
            rmem[instr_iop0] = rmem[instr_iop1];
            fetch_next_instr();

        S_COPY:
            strcpy(smem[instr_iop0], smem[instr_iop1]);
            fetch_next_instr();



        /* Stack Instructions. */
        I_PUSH_CNST:
            istack[isp++] = instr_iop0;
            fetch_next_instr();

        I_PUSH:
            istack[isp++] = imem[instr_iop0];
            fetch_next_instr();

        I_POP:
            imem[instr_iop0] = istack[--isp];
            fetch_next_instr();

        R_PUSH_CNST:
            rstack[rsp++] = instr_rop0;
            fetch_next_instr();

        R_PUSH:
            rstack[rsp++] = rmem[instr_iop0];
            fetch_next_instr();

        R_POP:
            rmem[instr_iop0] = rstack[--isp];
            fetch_next_instr();


        /* Logical Instructions.*/
            /* integer logic */
        I_EQL:
            imem[instr_iop0] = imem[instr_iop1] == imem[instr_iop2];
            fetch_next_instr();

        I_NEQL:
            imem[instr_iop1] = imem[instr_iop1] != imem[instr_iop2];
            fetch_next_instr();

        I_LT:
            imem[instr_iop0] = imem[instr_iop1] < imem[instr_iop2];
            fetch_next_instr();

        I_EQL_LT:
            imem[instr_iop0] = imem[instr_iop1] <= imem[instr_iop2];
            fetch_next_instr();

        I_GT:
            imem[instr_iop0] = imem[instr_iop1] > imem[instr_iop2];
            fetch_next_instr();

        I_EQL_GT:
            imem[instr_iop0] = (imem[instr_iop1]) >= (imem[instr_iop2]);
            fetch_next_instr();

        I_AND:
            imem[instr_iop0] = imem[instr_iop1] && imem[instr_iop2];
            fetch_next_instr();

        I_OR:
            imem[instr_iop0] = imem[instr_iop1] || imem[instr_iop2];
            fetch_next_instr();

        I_NOT:
            imem[instr_iop0] = !(imem[instr_iop1]);
            fetch_next_instr();


        /* Arithmetic Instructions. */
            /* integer arithmetic */
        I_INC:
            (imem[instr_iop0])++;
            fetch_next_instr();

        I_DEC:
            (imem[instr_iop0])--;
            fetch_next_instr();

        I_ADD:
            imem[instr_iop0] = imem[instr_iop1] + imem[instr_iop2];
            fetch_next_instr();

        I_SUB:
            imem[instr_iop0] = imem[instr_iop1] - imem[instr_iop2];
            fetch_next_instr();

        I_MUL:
            imem[instr_iop0] = imem[instr_iop1] * imem[instr_iop2];
            fetch_next_instr();

        I_DIV:
            imem[instr_iop0] = (int) imem[instr_iop1] / imem[instr_iop2];
            fetch_next_instr();
        I_MOD:
            imem[instr_iop0] = imem[instr_iop1] % imem[instr_iop2];
            fetch_next_instr();



           /* real arithmetic */
        R_ADD:
            rmem[instr_iop0] = rmem[instr_iop1] + rmem[instr_iop2];
            fetch_next_instr();

        R_SUB:
            rmem[instr_iop0] = rmem[instr_iop1] - rmem[instr_iop2];
            fetch_next_instr();

        R_MUL:
            rmem[instr_iop0] = rmem[instr_iop1] * rmem[instr_iop2];
            fetch_next_instr();

        R_DIV:
            rmem[instr_iop0] = rmem[instr_iop1] / rmem[instr_iop2];
            fetch_next_instr();


        /* Type Casting Instructions. */
        I_TO_R:
            rmem[instr_iop0] = (double) imem[instr_iop1];
            fetch_next_instr();

        I_TO_S:
            sprintf(smem[instr_iop0], "%d", imem[instr_iop1]);
            fetch_next_instr();

        R_TO_I:
            imem[instr_iop0] = (int) rmem[instr_iop1];
            fetch_next_instr();

        R_TO_S:
            sprintf(smem[instr_iop0], "%f", rmem[instr_iop1]);
            fetch_next_instr();

        S_TO_I:
            imem[instr_iop0] = atoi(smem[instr_iop1]);
            fetch_next_instr();

        S_TO_R:
            rmem[instr_iop0] = atof(smem[instr_iop1]);
            fetch_next_instr();



            /* real logic */
        R_EQL:
            sprintf(gpub1, "%f", rmem[instr_iop1]);
            sprintf(gpub2, "%f", rmem[instr_iop1]);
            imem[instr_iop0] = strcmp(gpub1, gpub2) == 0;
            fetch_next_instr();

        R_NEQL:
            sprintf(gpub1, "%f", rmem[instr_iop1]);
            sprintf(gpub2, "%f", rmem[instr_iop1]);
            imem[instr_iop0] = strcmp(gpub1, gpub2) != 0;
            fetch_next_instr();

        R_LT:
            rmem[instr_iop0] = rmem[instr_iop1] < rmem[instr_iop2];
            fetch_next_instr();


        R_GT:
            rmem[instr_iop0] = rmem[instr_iop1] > rmem[instr_iop2];
            fetch_next_instr();



        /* All String Instructions. */
        S_CH_EQL:
            imem[instr_iop0] = smem[instr_iop1][instr_iop2] == smem[instr_iop3][instr_iop4];
            fetch_next_instr();

        S_CH_NEQL:
            imem[instr_iop0] = smem[instr_iop1][instr_iop2] != smem[instr_iop3][instr_iop4];
            fetch_next_instr();

        S_CH_COPY:
            smem[instr_iop0][instr_iop1] = smem[instr_iop2][instr_iop3];
            fetch_next_instr();

        S_LEN:
            imem[instr_iop0] = strlen(smem[instr_iop1]);
            fetch_next_instr();

        S_JOIN:
            strcat(smem[instr_iop0], smem[instr_iop1]);
            fetch_next_instr();


        /* Binary File In/Out Instructions..

           operand 0: addr of smem where file path is,
           operand 1: addr of imem where writing will start.
           operand 2: the number of int objects to write.

           So it writes from imem[instr_iop1] to imem[instr_iop2].

        */
        I_FOUT:
            fstream = fopen(smem[instr_iop0], I_FOUT_FMODE);
            fwrite(&imem[instr_iop1], INT_SIZE, instr_iop2, fstream);
            fclose(fstream);
            fetch_next_instr();

        R_FOUT:
            fstream = fopen(smem[instr_iop0], R_FOUT_FMODE);
            fwrite(&rmem[instr_iop1], REAL_SIZE, instr_iop2, fstream);
            fclose(fstream);
            fetch_next_instr();

        S_FOUT:
            fstream = fopen(smem[instr_iop0], S_FOUT_FMODE);
            fwrite(&smem[instr_iop1], STR_SIZE, instr_iop2, fstream);
            fclose(fstream);
            fetch_next_instr();

        /* Okay the file out instructions write 1-n contiguous cells of memory to the
           specified file. Now these file in instructions below read these files back
           into memory.

           So basically:
           operand0: path of file we're reading.
           operand1: mem addr to read the files data into.
           operand2: specifies how many objects to read in.
           */

        I_FIN:
            fstream = fopen(smem[instr_iop0], I_FIN_FMODE);
            fread(&imem[instr_iop1], INT_SIZE, instr_iop2, fstream);
            fclose(fstream);
            fetch_next_instr();

        R_FIN:
            fstream = fopen(smem[instr_iop0], I_FIN_FMODE);
            fread(&rmem[instr_iop1], REAL_SIZE, instr_iop2, fstream);
            fclose(fstream);
            fetch_next_instr();

        S_FIN:
            fstream = fopen(smem[instr_iop0], I_FIN_FMODE);
            fread(&smem[instr_iop1], STR_SIZE, instr_iop2, fstream);
            fclose(fstream);
            fetch_next_instr();




        /* Console IO Instructions.
           ----------------------------

           Only strings can be send to std in/out. So cout takes the address
           of smem that instruction specifies to print. cin takes in a string
           from the console, this string is stored in smem cell specified by
           instr_iop0.*/
        COUT:
            printf(S_PRINTF_FMT, smem[instr_iop0]);
            fetch_next_instr();

        CIN:
            scanf(S_SCANF_FMT, smem[instr_iop0]);
            fetch_next_instr();




        /* Text-Mode File IO Instructions.
           -------------------------------
           These instructions perform file io, but only in text mode.
           TXTOUT writes 1-n cells of smem to the file at path specified
           by smem[instr_iop0]

           For TXTOUT:
           operand0: addr of smem, file path.
           operand1: addr of smem where writing starts.
           operand2: number of smem cells to write to file. (must be 1 or more).
        */
        TXTOUT:
            fstream = fopen(smem[instr_iop0], TEXT_OUT_FMODE);

            int_buf1 = 0;
            while (int_buf1 < instr_iop1) {
                fputs(&smem[instr_iop1 + 1], fstream);
                int_buf1++;
            }

            fclose(fstream);
            fetch_next_instr();


        /*  TXTIN:
            Basically, we're just reading in 256 chars at a time
            from specified file, we specify how many strings worth
            of text to read in.

            operand0: smem to addr file path.
            operand1: smem cell to start reading data to.
            operand2: number of strings worth of data to read in.
        */
        TXTIN:
            fstream = fopen(&smem[instr_iop0], TEXT_IN_FMODE);
            fgets(&smem[instr_iop1], STRLEN + instr_iop2, fstream);
            fclose(fstream);
            fetch_next_instr();



        /* Bitwise Instructions. */
        I_INV_BW:
            imem[instr_iop0] = ~(imem[instr_iop0]);
            fetch_next_instr();

        I_AND_BW:
            imem[instr_iop0] = imem[instr_iop1] & imem[instr_iop2];
            fetch_next_instr();

        I_OR_BW:
            imem[instr_iop0] = imem[instr_iop1] | imem[instr_iop2];
            fetch_next_instr();

        I_XOR_BW:
            imem[instr_iop0] = imem[instr_iop1] ^ imem[instr_iop2];
            fetch_next_instr();




        /* Instruction Loading Instruction. */
        LOAD_INSTR:
            /* This is by far the most complicated of all the instructions...
               Okay to load an instruction, first you create a representation
               of it in memory, so we choose a frame address, then we make sure this
               plus 5 is free memory for all 3 data types, then we set imem[frame_num-1]
               to the opcode, note we're subtracting. Next we the set our arguments in
               positions relevent to the frame number for all datatypes. */

            /* Set the new instructions opcode. */
           (prog + (imem[instr_iop0]))->opcode = imem[imem[instr_iop1] - 1];

            /* Set any integer operands that may be.. */
            (prog + (imem[instr_iop0]))->i_arg[0] = imem[imem[instr_iop1] + 1];
            (prog + (imem[instr_iop0]))->i_arg[1] = imem[imem[instr_iop1] + 2];
            (prog + (imem[instr_iop0]))->i_arg[2] = imem[imem[instr_iop1] + 3];
            (prog + (imem[instr_iop0]))->i_arg[3] = imem[imem[instr_iop1] + 4];
            (prog + (imem[instr_iop0]))->i_arg[4] = imem[imem[instr_iop1] + 5];

            /* Set any real operands that may be.. */
            (prog + (imem[instr_iop0]))->r_arg[0] = rmem[imem[instr_iop1] + 1];
            (prog + (imem[instr_iop0]))->r_arg[1] = rmem[imem[instr_iop1] + 2];
            (prog + (imem[instr_iop0]))->r_arg[2] = rmem[imem[instr_iop1] + 3];
            (prog + (imem[instr_iop0]))->r_arg[3] = rmem[imem[instr_iop1] + 4];
            (prog + (imem[instr_iop0]))->r_arg[4] = rmem[imem[instr_iop1] + 5];

            /* Set any string operands that maybe be.. */
            strcpy((prog + (imem[instr_iop0]))->s_arg[0], smem[instr_iop1 + 1]);
            strcpy((prog + (imem[instr_iop0]))->s_arg[1], smem[instr_iop1 + 2]);
            strcpy((prog + (imem[instr_iop0]))->s_arg[2], smem[instr_iop1 + 3]);
            strcpy((prog + (imem[instr_iop0]))->s_arg[3], smem[instr_iop1 + 4]);
            strcpy((prog + (imem[instr_iop0]))->s_arg[0], smem[instr_iop1 + 5]);

            /* Instruction loading complete.. */
            fetch_next_instr();


        LEAVE:
            /* Check if any allocated memory needs freeing. */
                /* code goes here! */

            return prv;

}


#endif // MACHINE_H_INCLUDED
