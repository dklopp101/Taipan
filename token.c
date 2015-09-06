/* Daniel Klopp 2015
   Hydrogen Assembler's Lexical Analyser
   -----------------------------------------

   BUGLIST:
	* Token col calculations are off by one but cannot work out why/where.
      Given line: load_byte 1 2, the first two token's cols are calculated
      correctly but curiously the third token is off by one and needs to be
      incremented once to be the correct col for the token, 2.

	
   TODO:
	* create a much better error system for the entire app.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hvm/opcodes.h"
#include "token.h"

char* toktype_strmap[TOKEN_TYPE_COUNT] = {"error_token", 
                                          "eof_token", 
                                          "eol_token",
                                          "opcode_token",
                                          "label_def_token",
		                                  "label_call_token",
                                          "int_token",
						                  "real_token"};


void add_token(tokstrm_t* tokstrm, token_t* token)
{
	tokstrm->strm[tokstrm->count] = token;
	++(tokstrm->count);
}

tokstrm_t* alloc_tokstrm()
{
	void* malloc_check = malloc(sizeof(tokstrm_t));
	tokstrm_t* tokstrm;	

	if (malloc_check) {
		tokstrm = (tokstrm_t*) malloc_check;
		tokstrm->count = 0;
	} else {
		fprintf(stderr, "alloc_tokstrm() malloc call returned NULL.\n");
		exit(EXIT_FAILURE);
	}

	return tokstrm;
}

void free_tokstrm(tokstrm_t* tokstrm)
{
	int i;

	for (i=0; i < tokstrm->count; ++i)
		free_token(tokstrm->strm[i]);

	free(tokstrm->strm);
	free(tokstrm);
}

void print_tokstrm(tokstrm_t* tokstrm)
{
	int i;

	for (i=0; i < tokstrm->count; ++i)
		print_token(tokstrm->strm[i]);
}

token_t* next_token(FILE* hasmfile)
{
	int ch;
	static int token_count = 0; /* starts at 0 because is used after incrementation. */
	static int line_count  = 1; /* starts at 1 because is used before incrementation. */
	static int col_count   = 0; /* starts at 0 because is used after incrementation. */   
	token_t* token = alloc_token();


	/* Only use the two functions below for getting/ungetting characters from the 
       hasmfile stream, they maintain col_count properly! */
	inline int get_char() {
		++col_count;
		return getc(hasmfile);
	}

	inline void unget_char(const int ch) {
		--col_count;
		ungetc(ch, hasmfile);
	}

	/* Main Loop. */
	for (;;)  {
		ch = get_char();
		switch_start: 
		switch (ch) {
			default:
				token->buf[token->buflen] = (char) ch;
				++token->buflen;
				continue;
			case ' ':
				if (token->buflen > 0) {
					classify_token(token);
					goto return_token;
				} else {
					continue;
				}
			case ';':
				if (token->buflen > 0) {
					classify_token(token);
					unget_char(ch);
					goto return_token;
				} else {
					for (;;) {
						ch = get_char();
						if ((ch == '\n') || (ch == '\0'))
							goto switch_start;
					}
				}
			case ':':
				if (token->buflen > 0) {
					classify_token(token);
					if (token->type == LABEL_CALL_TOKEN) {
						token->type = LABEL_DEF_TOKEN;
						goto return_token;
					}
				} 
				token->type = ERROR_TOKEN;
				goto return_token;
			case '\n':
				if (token->buflen > 0) {
					classify_token(token);
					unget_char(ch);
				} else {
					++line_count;
					col_count = 0;
					token->type = EOL_TOKEN;	
				}
				goto return_token;
			case EOF:
				if (token->buflen > 0) {
					classify_token(token);
					unget_char(ch);
				} else {
					token->type = EOF_TOKEN;	
				}
				goto return_token;
		}

	}

	return_token:
		++token_count;
		token->num  = token_count;
		token->line = line_count;
		token->col = col_count - token->buflen;
		return token;
}


token_t* alloc_token()
{
	void* malloc_check;
	token_t* token;	

	malloc_check = malloc(sizeof(token_t));
	if (malloc_check) {
		token = (token_t*) malloc_check;
	} else {
		fprintf(stderr, "alloc_token() malloc call returned NULL.\n");
		exit(EXIT_FAILURE);
	}

	/* All members are set to zero for safety and to signal this token is a virgin. */
	token->buf[0]  = 0;
	token->buflen  = 0;
	token->num     = 0;
	token->type    = 0;
	token->subtype = 0;

	return token;
}



/* Simply pretty prints the data members of token. */
void print_token(token_t* token)
{
	fprintf(stdout, "\n\nnumber: %d | type: %s | buflen: %d | line: %d | col: %d", 
		token->num, toktype_strmap[token->type], token->buflen, token->line, token->col);
	
	if (token->buflen)
		fprintf(stdout, " | str: %s", token->buf);

	fprintf(stdout, "\n");
}



/* Analyses token and sets it's type member and if required,
   it's subtype member. This function returns nothing.

   If token doesn't work as any of the valid token types then
   it is an invalid token which means it gets classified as an
   error token. */
void classify_token(token_t* token)
{ 
	token->subtype = is_opcode(token->buf);

	if (token->subtype > -1) /* if token has evaluated to an opcode. */
		token->type = OPCODE_TOKEN;

	else if (is_id(token->buf) > -1)
		token->type = LABEL_CALL_TOKEN;

	else if (is_int(token->buf) > -1)
		token->type = INT_TOKEN;

	else if (is_real(token->buf) > -1)
		token->type = REAL_TOKEN;

	else
		token->type = ERROR_TOKEN;
}


/* Returns 1 if str is a valid c identifier, -1 if not. */
int is_id(const char* str)
{
	int i;
	int str_len = strlen(str);

	if (str_len < 1)
		return -1;
	if (isdigit(str[0]))
		return -1;
	
	for (i=0; i < str_len; ++i) {
		if (ispunct(str[i])) {
			if (str[i] != '_')
				return -1;
		}
	}

	return 1;
}



/* Returns 1 if str is a valid integer, -1 if not.
   Note: 0456 would return -1 as would any floats. */
int is_int(const char* str)
{
	int i;
	int str_len = strlen(str);

	if (str_len < 1)
		return -1;	
	if (str[0] == 0)
		return -1;

	for (i=0; i < str_len; ++i) {
		if (!isdigit(str[i]))
			return -1;
	}

	return 1; 
}



/* Returns 1 if str is a valid floating point number, -1 if not.
   Integers will return -1. Note: .05, 0.05, 00.05, 000.05 will
   all return 1. Anything with more than one decimal point returns -1. */
int is_real(const char* str)
{
	int decp_found = 0;
	int str_len  = strlen(str);
	int i;

	if (str_len < 1)
		return -1;

	for (i=0; i < str_len; ++i) {
		if (!isdigit(str[i])) {
			if (str[i] == '.') {
				if (decp_found)
					return -1;
				else
					decp_found = 1;
			} else {
				return -1;
			}
		}
	}

	if (decp_found)
		return 1;
	else
		return -1;
}
