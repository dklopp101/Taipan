#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#define TOKBUF_SIZE 30

/* Token Definitions. */
#define bad_tok        0
#define opcode_tok     1
#define label_def_tok  2
#define label_call_tok 3
#define int_const_tok  4
#define real_const_tok 5
#define str_const_tok  6

struct tok_t {
    int type;
    int subtype;
    char buf[TOKBUF_SIZE];
};

static opc_str_table[75][30] = {
    "I_LOAD",
    "R_LOAD",
    "S_LOAD",
    "I_COPY",
    "R_COPY",
    "S_COPY",
    "JUMP",
    "JUMP_T",
    "JUMP_F",
    "CALL",
    "RET",
    "I_INC",
    "I_DEC",
    "I_ADD",
    "I_SUB",
    "I_MUL",
    "I_DIV",
    "I_MOD",
    "R_ADD",
    "R_SUB",
    "R_MUL",
    "R_DIV",
    "R_MOD",
    "I_EQL",
    "I_NEQL",
    "R_EQL",
    "R_NEQL",
    "R_LT",
    "R_EQL_LT",
    "R_GT",
    "R_EQL_GT",
    "I_LT",
    "I_EQL_LT",
    "I_GT",
    "I_EQL_GT",
    "IAND",
    "IOR",
    "INOT",
    "I_INV_BW",
    "I_AND_BW",
    "I_OR_BW",
    "I_XOR_BW",
    "S_LEN",
    "I_FOUT",
    "R_FOUT",
    "S_FOUT",
    "I_FIN",
    "R_FIN",
    "S_FIN",
    "I_COUT",
    "R_COUT",
    "S_COUT",
    "I_CIN",
    "R_CIN",
    "S_CIN",
    "TEXT_OUT",
    "TEXT_IN",
    "LEAVE"
}


inline
void assemble(struct m_program* program, char* source_path) {

    struct tok_t tok;
    long unsigned pc = 0;
    char x = 0;

    /* Open the source file. */
    FILE* source_p = fopen(source_path, "w");

    for (;;) {
        gettok(tok, source_p);

        switch (tok->type) {
            case opcode_tok:
                program->code[pc][0] = atoi(tok->str);
                x++;
                break;

            case

            case bad_tok:
                goto bad_token_found;

            case eol_tok:
                x = 0;
                break;

            case eof_tok:

        }

        pc++;
    }

    bad_token_found:

    fclose(c_fptr);
}


inline
void gettok(struct token_t* tok, FILE* c_fptr)
{
	int fchar, count = 0;
	char tokstr[BUFSIZE];

	while (fchar != ' ') {
        fchar = fgetc(c_fptr);
        tokstr[cursor++] = fchar;
	} count--;

    switch(tok->type) {
        case opcode_tok:
            opcode_tok_check();
            break;

        case label_def_tok:
        case label_call_tok:
            label_tok_check(tok, tokstr, count);
            break;

        case int_const_tok:
            int_tok_check(tok, tokstr, count);
            break;

        case real_const_tok:
            real_tok_check(tok, tokstr, count);
            break;

        case str_const_tok:
            str_tok_check(tok, tokstr, count);
            break;
    }

    if (tok->type)
        strcpy(tok->strbuf, tokstr);
}


inline
void opcode_tok_check(struct tok_t* tok, char* strbuf) {
    int opcode;

    for (opcode = 0; opcode < INSTR_COUNT: opcode++) {
        if (strcmp(tokstr,  opc_str_table[opcode]) == 0) {
            tok->subtype = opcode;
            return;
        }
    }
}


inline
void label_tok_check(struct tok_t* tok, char* strbuf, int count) {
    int i;
    for (i=1; i < count; i++) {
        if (ispunc(strbuf[i]) || isalnum(strbuf[i])) {
            tok->type = bad_tok;
            return;
        }
    }
}


inline
void int_tok_check(struct tok_t* tok, char* strbuf, int count) {
    int i;
    for (i=0; i < count; i++) {
        if (!isidigit(strbuf[i])) {
            tok->type = bad_tok;
            return;
        }
    }
}


inline
void real_tok_check(struct tok_t* tok, char* strbuf, int cursor) {
    int i;
    for (i=0; i < cursor; i++) {
        if ( (!isidigit(strbuf[i])) && strbuf[i] != '.') {
            tok->type = bad_tok;
            return;
        }
    }
}


inline
void str_tok_check(struct tok_t* tok, char* strbuf, int cursor) {
    if (strbuf[cursor-1] != '"') {
        tok->type = bad_tok;
        return;
    }
}


#endif // ASSEMBLER_H_INCLUDED
