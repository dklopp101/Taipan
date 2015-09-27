#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../vm/opcodes.h"
#include "parser.h"

#define eof EOF
#define eol '\n'

#define get_char()        \
	ch = getc(hasmfile);  \
	++col_count

#define unget_char()      \
	ungetc(ch, hasmfile); \
    --col_count

char* token_strmap[9] = {"error_token",
                         "eof_token",
                         "eol_token",
                         "opcode_token",
                         "label_def_token",
                         "sym_def_token",
                         "sym_eval_token",
				   		 "int_token",
                         "real_token"};

FILE* open_hasm(const char* path)
{
    return fopen(path, "r");
}

tstream_t* parse_hasm(FILE* hasmfile)
{
    tstream_t* tstream = alloc_tstream();
    token_t*   token;
    int i=0;

    for (;i < TOKEN_STREAM_SIZE; ++i) {
        token = next_token(hasmfile);
        (tstream->strm)[(tstream->len)] = token;
        ++(tstream->len);

        if (token->type == EOF_TOKEN)
            break;
    }

    return tstream;
}

tstream_t* alloc_tstream()
{
	void* malloc_check;
	tstream_t* tstream;

	malloc_check = malloc(sizeof(token_t));
	if (malloc_check) {
		tstream = (tstream_t*) malloc_check;
        tstream->len = 0;
	} else {
		fprintf(stderr, "alloc_tstream() malloc call returned NULL.\n");
		exit(EXIT_FAILURE);
	}

	return tstream;
}

void print_tstream(tstream_t* tstream)
{
    int i;

    for (i=0; i < tstream->len; ++i)
        print_token(tstream->strm[i]);
    printf("\n\t\t %d tokens.", tstream->len);
}

void free_tstream(tstream_t* tstream)
{
    int i=0;
    for (; i < tstream->len; ++i)
        free_token(tstream->strm[i]);

    free(tstream);
}


token_t* next_token(FILE* hasmfile)
{
	static int token_count = 0;
	static int line_count  = 1;
	static int col_count   = 1;

	int ch;
    int type;
    int subtype;

	token_t* token = alloc_token();

	for (;;) {
		get_char();
        switch_start:
		switch (ch) {
			default:
				token->buf[(token->buflen)] = (char) ch;
				++(token->buflen);
				continue;
            case '\t':
			case ' ':
				if ((token->buflen)) {
                    classify_token(token);
					goto token_ready;
				} else {
					continue;
				}
			case eol:
				if ((token->buflen)) {
					classify_token(token);
					unget_char();
				} else {
					token->type = EOL_TOKEN;
					++line_count;
					col_count = 0;
				}
				goto token_ready;
            case ';':
				if ((token->buflen)) {
                    classify_token(token);
                    unget_char();
					goto token_ready;
				} else {
                    for (;;) {
                        get_char();
                        if ((ch == eof) || (ch == eol))
                            break;
                    }
					goto switch_start;
				}
			case '=':
				if ((token->buflen)) {
                    classify_token(token);
                    if (token->type == SYMBOL_EVAL_TOKEN)
                        token->type = SYMBOL_DEF_TOKEN;
				} else {
					token->type = ERROR_TOKEN;
				}
                goto token_ready;
			case ':':
				if ((token->buflen)) {
                    classify_token(token);
                    if (token->type == SYMBOL_EVAL_TOKEN)
                        token->type = LABEL_DEF_TOKEN;
				} else {
					token->type = ERROR_TOKEN;
				}
                goto token_ready;
			case eof:
				if ((token->buflen)) {
					classify_token(token);
				} else {
					token->type = EOF_TOKEN;
                }
				goto token_ready;
		}
	}

	token_ready:
		++token_count;
		token->num  = token_count;
		token->line = line_count;
		token->col  = col_count - (token->buflen);
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
	token->num     = 0;
	token->type    = 0;
	token->subtype = 0;
	token->buf[0]  = 0;
	token->buflen  = 0;

	return token;
}

void free_token(token_t* token)
{
    free(token);
}


void print_token(token_t* token)
{
	fprintf(stdout, "\n\nnumber: %d | type: %s | buflen: %d | line: %d | col: %d",
		token->num, token_strmap[token->type], token->buflen, token->line, token->col);

	if (token->buflen) {
		fprintf(stdout, " | str: %s", token->buf);

        if (token->type == OPCODE_TOKEN)
            fprintf(stdout, " | opcode: %s", opcode_strmap[token->subtype]);
    }

	fprintf(stdout, "\n");
}

void classify_token(token_t* token)
{
    ((token)->buf)[(token)->buflen] = 0;

    if (is_opcode(token->buf)) {
        token->subtype = get_opcode(token->buf);
        token->type    = OPCODE_TOKEN;
    }

    else if (is_cid(token->buf)) {
        token->type = SYMBOL_EVAL_TOKEN;
    }

    else if (is_int(token->buf)) {
        token->type = INT_TOKEN;
    }

    else if(is_real(token->buf)) {
        token->type = REAL_TOKEN;
    }

    else {
        token->type = ERROR_TOKEN;
    }
}


/* Returns 1 if str is a valid c identifier, -1 if not. */
int is_cid(const char* str)
{
	int i;
	int str_len = strlen(str);

	if (str_len < 1)
		return 0;
	if (isdigit(str[0]))
		return 0;

	for (i=0; i < str_len; ++i) {
		if (ispunct(str[i])) {
			if (str[i] != '_')
				return 0;
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
		return 0;
	if (str[0] == 0)
		return 0;

	for (i=0; i < str_len; ++i) {
		if (!isdigit(str[i]))
			return 0;
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
					return 0;
				else
					decp_found = 1;
			} else {
				return 0;
			}
		}
	}

	if (decp_found)
		return 1;
	else
		return 0;
}
