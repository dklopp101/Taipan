
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hvm/opcodes.h"
#include "../hvm/types.h"

#include "symbol.h"
#include "token.h"
#include "asm.h"


#define emit_byte(v) \
	*write_ptr = v;  \
	++write_ptr;     \
	++(*text_size)

#define emit_opcode(opc)  \
	emit_byte(opc);       \
	++(*instr_count)

#define emit_i64(v)               \
	int_ptr = (i64*) write_ptr;   \
	*int_ptr = v;                 \
    write_ptr += sizeof(i64);     \
	(*text_size) += sizeof(i64)

#define emit_r64(v)                \
	real_ptr = (r64*) write_ptr;   \
	*real_ptr = v;                 \
    write_ptr += sizeof(r64);      \
	(*text_size) += sizeof(r64)


int main(int argc, char** argv)
{
	byte* text = malloc(50);
	printf("\n HELLO! \n");
	free(text);
	return 0;
}



byte* alloc_text(const size_t size)
{
	void* malloc_check = malloc(size);
	byte* text;

	if (malloc_check) {
		text = (byte*) malloc_check;
	} else {
		fprintf(stderr, "alloc_text() malloc call returned NULL.\n");
		exit(EXIT_FAILURE);
	}

	return text;
}


byte* assemble(FILE* hasmfile)
{
	tokstrm_t* tokstrm = alloc_tokstrm();
	token_t*   token;

	byte* text = allocate_text(TEXTMAX);
	i64*  instr_count = (i64*) text;
	i64*  text_size  = (i64*) text + sizeof(i64);
	byte* write_ptr = text + (sizeof(i64) + sizeof(i64));

	byte* byte_ptr;
	i64*  int_ptr;
	r64*  real_ptr;

	void get_emit_byte(const size_t count)
	{
		int i;
		for (i=0; i < count; ++i) {
			get_token();
			if (token->type == INT_TOKEN) {
				emit_byte(atoi(token->buf));
				continue;
			} else {
				new_error(SEMANTIC_ERROR, token);
			}
		}
	}

	void get_emit_i64(const size_t count)
	{
		int i;
		for (i=0; i < count; ++i) {
			token = next_token(hasmfile);
			if (token->type == INT_TOKEN) {
				emit_i64(atoi(token->buf));
				continue;
			} else {
				new_error(SEMANTIC_ERROR, token);
			}
		}
	}

	void get_emit_r64(const size_t count)
	{
		int i;
		for (i=0; i < count; ++i) {
			get_token();
			if (token->type == REAL_TOKEN) {
				emit_r64(atof(token->buf));
				continue;
			} else {
				new_error(SEMANTIC_ERROR, token);
			}
		}
	}

	for (;;) {
		get_token();

		switch (token->type) {
			default: /* catches int, real and symbol eval tokens as semantic errors. */
				new_error(SEMANTIC_ERROR, token);
				continue;
			case ERROR_TOKEN:
				new_error(TOKEN_ERROR, token);
				continue;
			case EOF_TOKEN:
				goto tokens_complete;
			case EOL_TOKEN:
				continue;
			case SYMBOL_DEF_TOKEN;
				byte_ptr = token->buf;
				get_token();
				switch (token->type) {
					case INT_TOKEN:
					case REAL_TOKEN:
					case SYM_EVAL_TOKEN:
						define_symbol(byte_ptr, token);
						continue;
					default:
						new_error(SEMANTIC_ERROR, token);
						continue;
				}			
			case OPCODE_TOKEN:
				emit_opcode(token->subtype);
				switch (token->subtype) {
					case DIE_OPCODE:
						continue;												
					case JMP_OPCODE:
						get_emit_i64(1);
						continue;
					case JMPEQL_OPCODE:
						get_emit_i64(2);
						continue;
					case JMPNEQL_OPCODE:
						get_emit_i64(2);
						continue;										
					case CALL_OPCODE:
						get_emit_i64(1);
						continue;	
					case RET_OPCODE:
						continue;
					case BLOD_OPCODE:
					case ILOD_OPCODE:
						get_emit_i64(2);
						continue;	
					case RLOD_OPCODE:
						get_emit_i64(1);
						get_emit_r64(1);
						continue;
					case BCPY_OPCODE:
					case ICPY_OPCODE:
					case RCPY_OPCODE:
					case BNOT_OPCODE:
					case INOT_OPCODE:
					case NOT_OPCODE:
						get_emit_i64(2);
						continue;	
					case AND_OPCODE:
					case OR_OPCODE:
					case XOR_OPCODE:
					case BEQL_OPCODE:
					case IEQL_OPCODE:
					case BNEQL_OPCODE:
					case INEQL_OPCODE:
					case BGTEQL_OPCODE:	
					case IGTEQL_OPCODE:
					case BGT_OPCODE:	
					case IGT_OPCODE:	
					case BLTEQL_OPCODE:
					case ILTEQL_OPCODE:
					case BLT_OPCODE:
					case ILT_OPCODE:
					case BAND_OPCODE:
					case IAND_OPCODE:
					case BOR_OPCODE:
					case IOR_OPCODE:
					case LSHFT_OPCODE:	
					case RSHFT_OPCODE:
					case BADD_OPCODE:
					case IADD_OPCODE:
					case RADD_OPCODE:	
					case BSUB_OPCODE:	
					case ISUB_OPCODE:
					case RSUB_OPCODE:
					case BMUL_OPCODE:
					case IMUL_OPCODE:
					case RMUL_OPCODE:	
					case BDIV_OPCODE:	
					case IDIV_OPCODE:
					case RDIV_OPCODE:	
					case BMOD_OPCODE:	
					case IMOD_OPCODE:
						get_emit_i64(3);
						continue;	
				}
		}
	} 

	tokens_complete:				
}
