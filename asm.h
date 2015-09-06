#ifndef asm_h_included
#define asm_h_included

#include <stdio.h>

#include "asm.h"
#include "token.h"
#include "../hvm/opcodes.h"
#include "../hvm/types.h"

#define SYMTAB_SIZE 100
#define TEXTMAX 1000

typedef struct {
	char id[ID_MAXLEN];
	token_t* token;
} symbol_t;

typedef struct {
	symbol_t* tbl[SYMTAB_SIZE];
	size_t    count;
} symtab_t;


/* Note: When a label def token is found the buf of that token is 
   the already validated id of the label. Now the subtype member
   of the token object isnt being used so we can utilise it to hold
   the addr of our label. This allows us to represent the label within
   the token just like normal symbols are represented which allows
   everything to be stored and thought of as a symbol. */
void new_label(symtab_t* symtab, const i64 addr, token_t* token)
{
	symbol_t* symbol = alloc_symbol();
	symbol->token = token;
	symbol->token->subtype = (int) addr;
	symtab->tbl[symtab->count] = symbol;
	++(symtab->count);	
}

i64 lookup_label(symtab_t* symtab, const char* id)
{
	int i;

	for (i=0; i < symtab->count; ++i) {
		if (streql(id, symtab->tbl[i]) 
			&& ((symtab->tbl)[i])->type == LABEL_DEF_TOKEN)
			return (i64) ((symtab->tbl)[i])->subtype;
	}

	return (i64) -1;
}

int lookup_symbol(symtab_t* symtab, const char* id);
symbol_t* alloc_symbol();
symtab_t* alloc_symtab();
void free_symtab(symtab_t* symtab);
i64 lookup_label(label_tbl_t* label_tbl, const char* id);
label_t* alloc_label();
label_tbl_t* alloc_label_tbl();
void free_label(label_t* label);
void free_label_tbl(label_tbl_t* label_tbl);
int main(int argc, char** argv);
byte* assemble(FILE* hasmfile);

#endif
