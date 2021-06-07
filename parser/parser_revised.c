/*******************************************************************************
Previous:  <SET> ::= "SET" <VAR> ":=" <POLISH>
Revised:   <SET> ::= "SET" <VAR> ":=" <POLISH> ";"

Previous:  <POLISH> ::= <OP> <POLISH> | <VARNUM> <POLISH> | ";"
Revised:   <POLISH> ::= <OP> <POLISH> <POLISH> | <VARNUM>
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

typedef enum bool {false, true} bool;

/**************************** Variable Definitions ****************************/

/* Parser input buffer size */
#define BUFSIZE (32)

/****************************** Token Definitions *****************************/

/* Operators */
#define ADD "+"
#define SUB "-"
#define MUL "*"
#define DIV "/"

/* Instructions */
#define FD "FD"
#define LT "LT"
#define RT "RT"

/* Delimiters */
#define LBRC "{"
#define RBRC "}"
#define SEMI ";"

/* Assign */
#define ASSIGN ":="

/* Keywords */
#define DO "DO"
#define FROM "FROM"
#define TO "TO"
#define SET "SET"

/* Check if the input string is an integer */
bool is_int(char* in);
/* Check if the input string is a float */
bool is_float(char* in);
/* Check if the input string is an operator */
bool is_op(char* in);
/* Check if the input string is an instruction */
bool is_inst(char* in);
/* Check if the input string is a keyword */
bool is_keyword(char *in);
/* Check if the input string is a delimiter */
bool is_deli(char* in);
/* Check if the input string is an assign */
bool is_assign(char *in);
/* Check if the input string is a name of variable */
bool is_var(char* in);
/* Check if the input string is a number */
bool is_num(char *in);
/* Check if the input string is a number or a variable */
bool is_varnum(char *in);

bool match_fd(FILE *fd);
bool match_lt(FILE *fd);
bool match_rt(FILE *fd);
bool match_do(FILE *fd);
bool match_set(FILE *fd);
bool match_polish(FILE *fd);
bool match_instrct(FILE *fd, char* buf);
bool match_instrctlst(FILE *fd);
bool match_main(FILE *fd);

int main(int argc, char* argv[])
{
    /* File descriptor */
    FILE *fd;
    /* The second element of the argument vector */
    char* filename = argv[1];
    bool success = false;

    /* Check the argument */
    if(argc<2){
        printf("Can't accept less than two argument");
    }
    if(argc>2){
        printf("Can't accept more than two argument");
    }

    assert(argc==2);

    /* printf("Input filename: %s\n", filename); */

    /* Open file to read and test if success */
    if ((fd = fopen(filename, "r")) == NULL) {
        printf("Cannot open file\n");
        exit(EXIT_FAILURE);
    }

    if (match_main(fd)) {
        success = true;
        printf("Parser succeeded and return 0\n");
    }

    /* Close the file */
    fclose(fd);

    /* End the parser */
    if (!success) {
        printf("Parser failed and return 1\n");
        exit(EXIT_FAILURE);
    }

    return 0;
/*
    (Read each token and print it)
    while (fscanf(fd, "%s", buf) != EOF) {
        printf("|%s| ", buf);

        (check if the input is a number by checking the first digit)
        if (is_int(buf)) {
            printf("--int--: %d\n", atoi(buf));
        }
        else if (is_float(buf)) {
            printf("--float--: %f\n", atof(buf));
        }
        (compare the string to string)
        else{
            if (is_op(buf)) {
                printf("--op--: %s\n", buf);
            }
            if (is_inst(buf)) {
                printf("--inst--: %s\n", buf);
            }
            if (is_var(buf)) {
                printf("--variable--: %s\n", buf);
            }
            if (is_deli(buf)) {
                printf("--deli--: %s\n", buf);
            }
            if (is_assign(buf)) {
                printf("--assign--: %s\n", buf);
            }
            if (is_keyword(buf)) {
                printf("--keyword--: %s\n", buf);
            }
        }
    }
*/
}

bool is_int(char* in)
{
    size_t i;

    /* Check if all the char's are digits */
    for (i = 0; i < strlen(in); ++i) {
        if (!isdigit(in[i])) {
            return false;
        }
    }
    return true;
}

bool is_float(char* in)
{
    /* end pointer*/
    char *ep = NULL;

    strtod(in, &ep);

    if (!ep || *ep) {
        return false;
    }
    return true;
}

bool is_op(char* in)
{
    if (!strcmp(in, ADD) ||
        !strcmp(in, SUB) ||
        !strcmp(in, MUL) ||
        !strcmp(in, DIV)) {
        return true;
    }
    return false;
}

bool is_inst(char* in)
{
    if (!strcmp(in, FD) ||
        !strcmp(in, LT) ||
        !strcmp(in, RT)) {
        return true;
        }
    return false;
}

bool is_keyword(char* in)
{
    if (!strcmp(in, DO) ||
        !strcmp(in, FROM) ||
        !strcmp(in, TO) ||
        !strcmp(in, SET)) {
        return true;
    }
    return false;
}

bool is_deli(char* in)
{
    if (!strcmp(in, LBRC) ||
        !strcmp(in, RBRC) ||
        !strcmp(in, SEMI)) {
        return true;
        }
    return false;
}

bool is_assign(char *in)
{
    if (!strcmp(in, ASSIGN)) {
        return true;
    }
    return false;
}

bool is_var(char* in)
{
    if (strlen(in) == 1 && in[0]>='A' && in[0]<='Z'){
        return true;
    }
    return false;
}

bool is_num(char* in)
{
    if (is_int(in) || is_float(in)) {
        return true;
    }
    return false;
}

bool is_varnum(char* in)
{
    if (is_num(in) || is_var(in)) {
        return true;
    }
    return false;
}

bool match_fd(FILE *fd)
{
    char buf[BUFSIZE];

    fscanf(fd, "%s", buf);

    /* If the rule matched */
    if (is_varnum(buf)) {
        return true;
    }
    return false;
}

bool match_lt(FILE *fd)
{
    char buf[BUFSIZE];

    fscanf(fd, "%s", buf);

    /* If the rule matched */
    if (is_varnum(buf)) {
        return true;
    }
    return false;
}

bool match_rt(FILE *fd)
{
    char buf[BUFSIZE];

    fscanf(fd, "%s", buf);

    /* If the rule matched */
    if (is_varnum(buf)) {
        return true;
    }
    return false;
}

bool match_do(FILE *fd)
{
    char buf[BUFSIZE];

    fscanf(fd, "%s", buf);

    if (is_var(buf)) {
        fscanf(fd, "%s", buf);
        if (!strcmp(buf, FROM)) {
            fscanf(fd, "%s", buf);
            if (is_varnum(buf)) {
                fscanf(fd, "%s", buf);
                if (!strcmp(buf, TO)) {
                    fscanf(fd, "%s", buf);
                    if (is_varnum(buf)) {
                        fscanf(fd, "%s", buf);
                        if (!strcmp(buf, LBRC)) {
                            printf("A Do-loop starts\n");
                            if (match_instrctlst(fd)) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool match_set(FILE *fd)
{
    char buf[BUFSIZE];

    fscanf(fd, "%s", buf);

    if (is_var(buf)) {
        fscanf(fd, "%s", buf);
        if (is_assign(buf)) {
            if (match_polish(fd)) {
                fscanf(fd, "%s", buf);
                if (!strcmp(buf, SEMI)) {
                    printf("Matched SET\n");
                    return true;
                }
            }
        }
    }
    return false;
}

bool match_polish(FILE *fd)
{
    char buf[BUFSIZE];

    fscanf(fd, "%s", buf);

    printf("Matching polish with %s\n", buf);

    if (is_varnum(buf)) {
        printf("Matching variable with %s\n", buf);
        return true;
    }
    if (is_op(buf)) {
        printf("Matching operator with %s\n", buf);
        if (match_polish(fd)) {
            printf("Matching polish with %s\n", buf);
            if (match_polish(fd)) {
                printf("Matching polish with %s\n", buf);
                return true;
            }
        }
    }
    return false;
}

bool match_instrct(FILE *fd, char* buf)
{
    printf("Matching instuction with %s\n", buf);
    if (!strcmp(buf, FD)) {
        if (match_fd(fd)) {
            return true;
        }
    }
    if (!strcmp(buf, LT)) {
        if (match_lt(fd)) {
            return true;
        }
    }
    if (!strcmp(buf, RT)) {
        if (match_rt(fd)) {
            return true;
        }
    }
    if (!strcmp(buf, DO)) {
        if (match_do(fd)) {
            return true;
        }
    }
    if (!strcmp(buf, SET)) {
        if (match_set(fd)) {
            return true;
        }
    }
    return false;
}

bool match_instrctlst(FILE *fd)
{
    char buf[BUFSIZE];

    fscanf(fd, "%s", buf);

    printf("Matching instuction list with %s\n", buf);

    if (!strcmp(buf, RBRC)) {
        return true;
    }
    else {
        if (match_instrct(fd, buf)) {
            if (match_instrctlst(fd)) {
                return true;
            }
        }
    }
    return false;
}

bool match_main(FILE *fd)
{
    char buf[BUFSIZE];

    fscanf(fd, "%s", buf);

    /* printf("Matching main with %s\n", buf); */

    if (!strcmp(buf, LBRC)) {
        return match_instrctlst(fd);
    }
    return false;
}
