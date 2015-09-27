#ifndef parser_h
#define parser_h

#include <stdio.h>
#include <stdlib.h>

#define EOL '\n'

#define LINE_MAX_SIZE       100
#define TOKEN_BUFFER_SIZE   LINE_MAX_SIZE
#define TOKEN_STREAM_SIZE   600
#define TOKENTYPE_COUNT    9

/*  Token Typecodes. */
#define ERROR_TOKEN        0
#define EOF_TOKEN          1
#define EOL_TOKEN          2
#define OPCODE_TOKEN       3
#define LABEL_DEF_TOKEN    4
#define SYMBOL_DEF_TOKEN   5
#define SYMBOL_EVAL_TOKEN  6
#define INT_TOKEN          7
#define REAL_TOKEN         8

/* Token string-map simply maps typecodes to strings for human reading. */
char* token_strmap[9];

/* Token struct used to represent tokens during parsing runtime. */
typedef struct {
	int  num;
    int  type;
    int  subtype;
	int  line;
	int  col; /* this refers to the col number of the very first character of the buffer. */
    char buf[TOKEN_BUFFER_SIZE];
    int  buflen;
} token_t;

typedef struct {
    token_t* strm[TOKEN_STREAM_SIZE];
    int len;
} tstream_t;


FILE* open_hasm(const char* path);
tstream_t* parse_hasm(FILE* hasmfile);
tstream_t* alloc_tstream();
void print_tstream(tstream_t* tstream);
void free_tstream(tstream_t* tstream);

token_t* next_token(FILE* hasmfile);
token_t* alloc_token();
void free_token(token_t* token);
void print_token(token_t* token);
void classify_token(token_t* token);
int is_cid(const char* str);
int is_int(const char* str);
int is_real(const char* str);


#endif
