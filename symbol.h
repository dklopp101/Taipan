#ifndef symbol_h_included
#define symbol_h_included

#ifndef asm_h_included
#define asm_h_included

#include <stdio.h>

#include "asm.h"
#include "lex.h"
#include "../hvm/opcodes.h"
#include "../hvm/types.h"

#define SYMTAB_SIZE 100



typedef struct {
	char id[ID_MAXLEN];
	token_t* token;
} symbol_t;

typedef struct {
	symbol_t* tbl[SYMTAB_SIZE];
	size_t    count;
} symtab_t;

int      id_inuse(symtab_t* symtab, const char* id);
void      new_symbol(symtab_t* symtab, const char* id, token_t* token);
void      new_label(symtab_t* symtab, const i64 addr, token_t* token);
int       lookup_symbol(symtab_t* symtab, const char* id, void* value_ptr);
i64       lookup_label(symtab_t* symtab, const char* id);
int       lookup_symbol(symtab_t* symtab, const char* id, void* value_ptr);
i64       lookup_label(symtab_t* symtab, const char* id);
symbol_t* alloc_symbol();
symtab_t* alloc_symtab();
void      free_symbol(symbol_t* symbol);
void      free_symtab(symtab_t* symtab);



#endif
