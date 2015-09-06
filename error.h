#ifndef error_h_included
#define error_h_included

#include "lex.h"

#define TOKEN_ERROR    0
#define SEMANTIC_ERROR 1

char* error_strmap[2];

typedef struct {
	int num;
	int type;
	token_t* token;
} error_t;

typedef struct {
	error_t* tbl[ERRTBL_SIZE];
	size_t   count;
} errtbl_t;

error_t* alloc_error();
void     free_error(error_t* error);
error_t* alloc_errtbl();
void     free_errtbl(errtbl_t* errtbl);
void     new_error(errtbl_t* errtbl, const int type, token_t* token);
void     print_error(error_t* error);
void     print_errtbl(errtbl_t* errtbl);

#endif#ifndef error_h_included
#define error_h_included

#include "lex.h"

#define TOKEN_ERROR    0
#define SEMANTIC_ERROR 1

char* error_strmap[2];

typedef struct {
	int num;
	int type;
	token_t* token;
} error_t;

typedef struct {
	error_t* tbl[ERRTBL_SIZE];
	size_t   count;
} errtbl_t;

error_t* alloc_error();
void     free_error(error_t* error);
error_t* alloc_errtbl();
void     free_errtbl(errtbl_t* errtbl);
void     new_error(errtbl_t* errtbl, const int type, token_t* token);
void     print_error(error_t* error);
void     print_errtbl(errtbl_t* errtbl);

#endif
