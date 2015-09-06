#include "symbol.h"
#include "lex.h"

int id_inuse(symtab_t* symtab, const char* id)
{
	int i;

	for (i=0; i < symtab->count; ++i) {
		if (streql(id, ((symtab->tbl)[i])->id))
			return 1;
	}

	return -1;
}

void new_symbol(symtab_t* symtab, const char* id, token_t* token)
{
	symbol_t* symbol = alloc_symbol();
	symbol->token = token;
	strcpy(symbol->id, id);
	symtab->tbl[symtab->count] = s;
	++(symtab->count);
}


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
	symtab_add(symbol);
}


/*
	lookup_symbol(): This function serves two purposes, it determines whether a
    symbol by the specified id exists within the symbol table and secondly it
    sets a passed pointer to the value that the identified symbol evals to. So
    if the symbol isn't found at all, -1 is returned and nothing is done with 
    value_ptr. Otherwise if the symbol is found it is evaluated and value_ptr
	is set to this value and 1 is returned.
*/
int lookup_symbol(symtab_t* symtab, const char* id, void* value_ptr)
{
	symbol_t* symbol;
	int i;

	i64* i64_ptr, i64_obj;
	r64* r64_ptr, r64_obj;

	/* Determine if symbol with specified id exists within the symbol table.*/
	for (i=0; i < symtab->count; ++i) {
		if (streql(id, symtab->tbl[i])) {
			symbol = symtab->tbl[i];
			goto symbol_found;
		}
	}

	/* If control reaches here the loop completed without finding matching symbol. */
	return -1;

	/* Symbol's found so we determine what type of value we're working with. */
	symbol_found:
	switch (symbol->token->type) {
		case INT_TOKEN:
			i64_ptr  = (i64*) value_ptr;
			*i64_ptr = (i64) atoll(symbol->token->buf);
			break;
		case REAL_TOKEN:
			r64_ptr  = (r64*) value_ptr;
			*r64_ptr = (r64) strtod(symbol->token->buf, 0);
			break;
		case LABEL_DEF_TOKEN:
			i64_ptr  = (i64*) value_ptr;
			*i64_ptr = (i64) symbol->token->subtype;
			break;
	}

	return 1;
}	


/*
   lookup_label(): If the passed symtab contains a label symbol with 
   an id matching the passed id then it's assicate instruction address
   is returned after being cast to i64 from int. Otherwise i64 cast -1
   is returned, this signifies that no label by the passed id exists. */
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

