#include "include/general.h"
#include "include/symtb.h"

/**************************** Variable Definitions ****************************/

/* Parser input buffer size */
#define BUFSIZE (32)

/* SDL window size */
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

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

/****************************** Global Variables ******************************/

/* The direction of the cursor wrt y axis, starting from pointing "up" */
static double global_deg = 180;

/*
 * Self-defined data type to store the turning points
 * The underlying data structure is a linked-list
 */
typedef struct cursor {
    double x;
    double y;
    struct cursor* next;
} Cursor;

/* The pointer to the end of the linked list */
Cursor *cursor_end;

/* Symbol table variables */
Symtb* symtb_head;
static int global_scope = 0;

/***************************** Function Prototypes ****************************/

/*** Parsing functions ***/

/* Check if the input string is an integer */
bool is_int(char* in, double* fptr);
/* Check if the input string is a float */
bool is_float(char* in, double* fptr);
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
bool is_num(char *in, double* fptr);
/* Check if the input string is a number or a variable */
bool is_varnum(char *in, double* fptr);

bool match_fd(FILE *fd);
bool match_lt(FILE *fd);
bool match_rt(FILE *fd);
bool match_do(FILE *fd);
bool match_set(FILE *fd);
bool match_polish(FILE *fd, double* fptr);
bool match_instrct(FILE *fd, char* buf);
bool match_instrctlst(FILE *fd);
bool match_main(FILE *fd);

/*** SDL control functions ***/

/* Initialize the linked list of cursors */
Cursor* cursor_init(double init_x, double init_y);
/* Input how many distance moving forward */
void cursor_forward(double len);
/* Input how many degree to turn */
void cursor_turn_deg(double deg);


int main(int argc, char* argv[])
{
/*
    / Add symbol table functions to test /
    symtb* head;
    double val;

    symtb_insert(&head, 'A', 0, 3.14);
    symtb_insert(&head, 'B', 1, 2.17);
    symtb_insert(&head, 'C', 2, 0.25);

    symtb_delete(head, 2);
    symtb_update_val(head, 'C', 2, 0.27);
    symtb_insert(&head, 'C', 3, 0.29);

    val = 0;
    if (symtb_find_val(head, 'C', 5, &val)) {
        SDL_Log("symtable table entry %f", val);
    }
    else {
        SDL_Log("symtb empty\n");
    }

    SDL_Log("symtb test end\n");
*/

    /* File descriptor */
    FILE *fd;
    /* The second element of the argument vector */
    char* filename = argv[1];
    bool success = false;
    Cursor *cursor_head;
    Cursor* csr;

    /* Check the argument */
    if(argc != 2){
        fprintf(stderr, "Usage : %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

/*
    SDL_Log("Input filename: %s\n", filename);
*/

    /* Open file to read and test if success */
    if ((fd = fopen(filename, "r")) == NULL) {
        SDL_Log("Cannot open file.\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize the linked list of cursors */
    cursor_head = cursor_init(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    if (match_main(fd)) {
        success = true;
        /* SDL_Log("Parser succeeded!\n"); */
    }

    /* Close the file */
    fclose(fd);

    /* End the parser */
    if (!success) {
        SDL_Log("Parser failed.\n");
        exit(EXIT_FAILURE);
    }

    /* If parsing succeeded, start a SDL window to draw output */
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        SDL_bool done;

        if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) == 0) {

            /* Extract the basename (filename) from a compelete path, use it as window name */
            SDL_SetWindowTitle(window, basename(filename));

            done = SDL_FALSE;

            while (!done) {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

                csr = cursor_head;

                if (csr != NULL) {
                    while (csr->next != NULL){
                        SDL_RenderDrawLine(renderer,
                                           csr->x,
                                           csr->y,
                                           csr->next->x,
                                           csr->next->y);
                        csr = csr->next;
                    }
                }
                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    }
                }
            }
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }

    /* Shut down SDL */
    SDL_Quit();
    return 0;
/*
    (Read each token and print it)
    while (fscanf(fd, "%s", buf) != EOF) {
        SDL_Log("|%s| ", buf);

        (check if the input is a number by checking the first digit)
        if (is_int(buf)) {
            SDL_Log("--int--: %d\n", atoi(buf));
        }
        else if (is_float(buf)) {
            SDL_Log("--float--: %f\n", atof(buf));
        }
        (compare the string to string)
        else{
            if (is_op(buf)) {
                SDL_Log("--op--: %s\n", buf);
            }
            if (is_inst(buf)) {
                SDL_Log("--inst--: %s\n", buf);
            }
            if (is_var(buf)) {
                SDL_Log("--variable--: %s\n", buf);
            }
            if (is_deli(buf)) {
                SDL_Log("--deli--: %s\n", buf);
            }
            if (is_assign(buf)) {
                SDL_Log("--assign--: %s\n", buf);
            }
            if (is_keyword(buf)) {
                SDL_Log("--keyword--: %s\n", buf);
            }
        }
    }
*/
}

/************************** Function Implementations **************************/

bool is_int(char* in, double* fptr)
{
    size_t i;

    /* Check if all the char's are digits */
    for (i = 0; i < strlen(in); ++i) {
        if (!isdigit(in[i])) {
            return false;
        }
    }

    *fptr = (double)atoi(in);

    /* SDL_Log("An integer is recognized: %d\n", atoi(in)); */

    return true;
}

bool is_float(char* in, double* fptr)
{
    /* End pointer*/
    char *ep = NULL;
    double f;

    f = strtod(in, &ep);

    if (!ep || *ep) {
        return false;
    }

    *fptr = f;

    /* SDL_Log("A float is recognized: %f\n", strtod(in, &ep)); */

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

bool is_num(char* in, double* fptr)
{
    if (is_int(in, fptr) || is_float(in, fptr)) {
        return true;
    }
    return false;
}

bool is_varnum(char* in, double* fptr)
{
    if (is_num(in, fptr)) {
        return true;
    }
    if (is_var(in)) {
        symtb_find_val(symtb_head, in[0], global_scope, fptr);
        return true;
    }
    return false;
}

bool match_fd(FILE *fd)
{
    char buf[BUFSIZE];
    /* Floating point number value */
    double fval;

    fscanf(fd, "%s", buf);


    /* If the rule matched */
    if (is_varnum(buf, &fval)) {
        cursor_forward(fval);
        return true;
    }

    return false;
}

bool match_lt(FILE *fd)
{
    char buf[BUFSIZE];
    /* Floating point number value */
    double fval;

    fscanf(fd, "%s", buf);

    /* If the rule matched */
    if (is_varnum(buf, &fval)) {
        cursor_turn_deg((-1) * fval);
        return true;
    }

    return false;
}

bool match_rt(FILE *fd)
{
    char buf[BUFSIZE];
    /* Floating point number value */
    double fval;

    fscanf(fd, "%s", buf);

    /* If the rule matched */
    if (is_varnum(buf, &fval)) {
        cursor_turn_deg((1) * fval);
        return true;
    }

    return false;
}

bool match_do(FILE *fd)
{
    char buf[BUFSIZE];
    double fval;
    /* Iterator */
    char it_name;
    double it_val;
    double it_start;
    double it_end;
    /* The current position of the file pointer */
    long cur_pos;

    fscanf(fd, "%s", buf);

    if (is_var(buf)) {
        it_name = buf[0];
        fscanf(fd, "%s", buf);
        if (!strcmp(buf, FROM)) {
            fscanf(fd, "%s", buf);
            if (is_varnum(buf, &fval)) {
                it_start = fval;
                fscanf(fd, "%s", buf);
                if (!strcmp(buf, TO)) {
                    fscanf(fd, "%s", buf);
                    if (is_varnum(buf, &fval)) {
                        it_end = fval;
                        fscanf(fd, "%s", buf);
                        if (!strcmp(buf, LBRC)) {
                            /* SDL_Log("A Do-loop starts.\n"); */

                            /* Go into a deeper scope when '{' is read */
                            ++global_scope;

                            /* SDL_Log("Scope go to %d\n", global_scope); */

                            /* Record the current cursor position*/
                            cur_pos = ftell(fd);

                            /* Set the iterator */
                            it_val = it_start;

                            if (!symtb_insert(&symtb_head, it_name, global_scope, it_val)) {
                                /* If insert failed, the var already exists */
                                symtb_update_val(symtb_head, it_name, global_scope, it_val);
                            }

                            /* Use known information to repeat the matching method */
                            for (it_val = it_start; it_val < it_end; it_val += 1) {
                                symtb_update_val(symtb_head, it_name, global_scope, it_val);
                                if (!match_instrctlst(fd)) {
                                    return false;
                                }
                                /* Move the cursor back */
                                fseek(fd, cur_pos, SEEK_SET);
                            }

                            /* Run through the loop again */
                            symtb_update_val(symtb_head, it_name, global_scope, it_val);
                            if (!match_instrctlst(fd)) {
                                return false;
                            }

                            /*
                             * Go back to a more shallow scope when instruction
                             * list is matched and the loop ends (after '}' is read)
                             */
                            --global_scope;
                            /* SDL_Log("Scope back to %d\n", global_scope); */

                            /* SDL_Log("Do-loop ends.\n"); */
                            return true;
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
    double fval;
    char var_name;

    fscanf(fd, "%s", buf);

    if (is_var(buf)) {
        var_name = buf[0];
        /* SDL_Log("var_name: %c", var_name); */
        fscanf(fd, "%s", buf);
        if (is_assign(buf)) {
            if (match_polish(fd, &fval)) {
                printf("fval: %f\n", fval);
                if (!symtb_insert(&symtb_head, var_name, global_scope, fval)) {
                    /* If insert failed, the variable already exists */
                    symtb_update_val(symtb_head, var_name, global_scope, fval);
                    /* SDL_Log("%c is set to %f\n", var_name, fval); */
                }
                return true;
            }
        }
    }
    return false;
}

bool match_polish(FILE *fd, double* fptr)
{
    char buf[BUFSIZE];
    double fval;

    fscanf(fd, "%s", buf);

    printf("Matching polish with %s\n", buf);

    if (!strcmp(buf, SEMI)) {
        return true;
    }
    if (is_op(buf)) {
        printf("Matched OP: %s\n", buf);
        if (match_polish(fd, &fval)) {
            printf("<OP> fval: %f\n", fval);
            return true;
        }
    }
    if (is_varnum(buf, fptr)) {
        printf("Matched VARNUM: %s\n", buf);
        if (match_polish(fd, &fval)) {
            printf("<VARNUM> fval: %f\n", fval);
            return true;
        }
    }
    return false;
}

bool match_instrct(FILE *fd, char* buf)
{
    /* SDL_Log("Matching instuction with %s\n", buf); */

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

    /* SDL_Log("Matching instuction list with %s\n", buf); */

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

    if (!strcmp(buf, LBRC)) {
        return match_instrctlst(fd);
    }
    return false;
}

Cursor* cursor_init(double init_x, double init_y)
{
    Cursor* head;

    head = (Cursor*)calloc(1,sizeof(Cursor));
    if (head == NULL) {
        SDL_Log("Cannot calloc() space.\n");
        exit(EXIT_FAILURE);
    }

    head->x = init_x;
    head->y = init_y;
    head->next = NULL;

    /*  Record the end of the cursor list */
    cursor_end = head;
    return head;
}

void cursor_forward(double len)
{
    double radian = global_deg * (M_PI / 180.0);
    Cursor* csr = cursor_end;

    /* Add a new node behind */
    csr->next = (Cursor*)calloc(1,sizeof(Cursor));
    if (csr->next == NULL) {
        SDL_Log("Cannot calloc() space.\n");
        exit(EXIT_FAILURE);
    }

    csr->next->x = csr->x + len * sin(radian);
    csr->next->y = csr->y + len * cos(radian);
    csr->next->next = NULL;

    /* Update the tailing pointer */
    cursor_end = csr->next;
}

void cursor_turn_deg(double deg)
{
    global_deg -= deg;

    /* Avoid overflow */
    while (global_deg > 360.0 || global_deg < -360.0) {
        if (global_deg > 360.0) {
            global_deg -= 360.0;
        }
        else if (global_deg < -360.0) {
            global_deg += 360.0;
        }
    }
}
