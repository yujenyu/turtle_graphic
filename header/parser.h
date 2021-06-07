#ifndef PARSER_H
#define PARSER_H

#include "general.h"

#ifdef INTERPRET
#include "symtb.h"
#endif

/* Parser input buffer size */
#define BUFSIZE (32)

/* Operators */
#define ADD "+"
#define SUB "-"
#define MUL "*"
#define DIV "/"

/* Instructions */
#define FD "FD"
#define LT "LT"
#define RT "RT"

/* Keywords */
#define DO "DO"
#define FROM "FROM"
#define TO "TO"
#define SET "SET"

/* Delimiters */
#define LBRC "{"
#define RBRC "}"
#define SEMI ";"

/* Assign */
#define ASSIGN ":="

#ifdef INTERPRET
/* Symbol table variables */
Symtb* symtb_head;
static int global_scope = 0;
#endif

/* Check if the input match <MAIN> */
bool match_main(FILE *fd);
/* Check if the input match <INSTRCTLST> */
bool match_instrctlst(FILE *fd);
/* Check if the input match <INSTRUCTION> */
bool match_instrct(FILE *fd, char* buf);
/* The input matched <FD>, check if the next input is <VARNUM> */
bool match_fd(FILE *fd);
/* The input matched <LT>, check if the next input is <VARNUM> */
bool match_lt(FILE *fd);
/* The input matched <RT>, check if the next input is <VARNUM> */
bool match_rt(FILE *fd);
/* The input matched <DO>, check if the next input is following the grammar */
bool match_do(FILE *fd);
/* The input matched <SET>, check if the next input is following the grammar */
bool match_set(FILE *fd);
/* Check if the input match <POLISH> grammar */
bool match_polish(FILE *fd, double* fptr);
/* Check if the input string is a number or a variable */
bool is_varnum(char* in, double* fptr);
/* Check if the input string is a name of variable */
bool is_var(char* in);
/* Check if the input string is a number */
bool is_num(char* in, double* fptr);
/* Check if the input string is an integer */
bool is_int(char* in, double* fptr);
/* Check if the input string is a float */
bool is_float(char* in, double* fptr);
/* Check if the input string is an operator */
bool is_op(char* in);
/* Check if the input string is an assign */
bool is_assign(char *in);

#endif
