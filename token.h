/*
    Hydrogen Assembler Lexical Analyser Header
    Daniel W. Klopp August 2015
*/

#ifndef token_h_included
#define token_h_included

#include <stdio.h>

#define streql(s1, s2) (strcmp(s1, s2) == 0)

/* Token Type Definitions / Token Typecodes Declarations. */
#define TOKEN_TYPE_COUNT    9

#define ERROR_TOKEN         0
#define EOF_TOKEN           1
#define EOL_TOKEN           2
#define OPCODE_TOKEN        3
#define LABEL_DEF_TOKEN     4
#define SYM_DEF_TOKEN       5
#define SYM_EVAL_TOKEN      6
#define INT_TOKEN           7
#define REAL_TOKEN          8 

char* toktype_strmap[TOKEN_TYPE_COUNT];

#define MAX_LINE_SIZE     150
#define TOKEN_BUF_SIZE    80
#define TOKEN_BUF_MAXLEN  TOKEN_BUF_SIZE
#define TOKSTRM_SIZE      500
#define ID_MAXLEN         TOKEN_BUF_MAXLEN

typedef struct {
	int  num;
    int  type;
    int  subtype;
	int  line;
	int  col;
    char buf[TOKEN_BUF_SIZE];
    int  buflen;
} token_t;

typedef struct {
	token_t* strm[TOKSTRM_SIZE];
	size_t    count;
} tokstrm_t;


tokstrm_t* alloc_tokstrm();
token_t* alloc_token();



void     add_token(tokstrm_t* tokstrm, token_t* token);
void     free_token();
void     free_tokstrm();
token_t* next_token(FILE* hasmfile);
void     print_token(token_t* token);
void     print_tokstrm(tokstrm_t* tokstrm);
void     classify_token(token_t* token);
int      is_id(const char* str);
int      is_int(const char* str);
int      is_real(const char* str);


#define get_token()               \
	token = next_token(hasmfile); \
	tokstrm[token_count] = token; \
	++token_count

tokstrm_t* pull_tokstrm(FILE* hasmfile)
{
	token_t* token;
	get_token();

	while (token->type != EOF_TOKEN) {
		token = next_token
	}

	for (;;) {
		get_token()
		if
		switch (token->type) {
			case EOF_TOKEN:
		}
	}
}

#endif
