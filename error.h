#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "lex.h"

char* error_strmap[2] = {"token error", 
                         "semantic error"};


error_t* alloc_error()
{
	void* malloc_check = malloc(sizeof(error_t));
	error_t* error;

	if (malloc_check) {
		error = (error_t*) malloc_check;
		error->type  = -1;
		error->token =  0;	
	} else {
		fprintf(stderr, "alloc_error() malloc call returned NULL.\n");
		exit(EXIT_FAILURE);
	}

	return error;
}


void free_error(error_t* error)
{
	free(error);
}


error_t* alloc_errtbl()
{
	void* malloc_check = malloc(sizeof(errtbl_t));
	errtbl_t* errtbl;

	if (malloc_check) {
		errtbl = (errtbl_t*) malloc_check;
		errtbl->count = 0;	
	} else {
		fprintf(stderr, "alloc_errtbl() malloc call returned NULL.\n");
		exit(EXIT_FAILURE);
	}

	return errtbl;
}



void free_errtbl(errtbl_t* errtbl)
{
	int i;

	for (i=0; i < errtbl->count; ++i)
		free(errtbl->tbl[i]);
	
	free(errtbl->tbl);
	free(errtbl);
}



void new_error(errtbl_t* errtbl, const int type, token_t* token)
{
	error_t* error = alloc_error();
	error->type  = type;
	error->token = token;
	(errtbl->tbl)[errtbl->count] = error;
	++(errtbl->count);
}



/* At this point in time print_error() simply prints the offending token, line, col
   but not much more. When i get around to it this needs to be updated to print the
   entire line and then an arrow underneith like python does to show clearly where
   the error is. */
void print_error(error_t* error)
{
	fprintd(stdout, "\n error on line %d at column %d: %s - Offending Token: %s\n",
		(int) error->token->line, 
        (int) error->token->col, 
        error_strmap[error->type], 
        error->token->buf);
}



void print_errtbl(errtbl_t* errtbl)
{
	int i;

	if (errtbl->count) {
		for (i=0; i < errtbl->count; ++i)
			print_error(errtbl->tbl[i]);
	}

	fprintf(stdout, "total errors: %d\n", (int) errtbl->count);
}
