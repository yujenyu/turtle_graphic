#include "../header/parser.h"
#ifdef INTERPRET
#include "../header/cursor.h"
#endif

bool match_main(FILE *fd)
{
    char buf[BUFSIZE];

    fscanf(fd, "%s", buf);

    if (!strcmp(buf, LBRC)) {
        return match_instrctlst(fd);
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

bool match_fd(FILE *fd)
{
    char buf[BUFSIZE];
    /* Floating point number value */
    double fval;

    fscanf(fd, "%s", buf);

    /* If the rule matched */
    if (is_varnum(buf, &fval)) {
#ifdef INTERPRET
        cursor_forward(fval);
#endif
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
#ifdef INTERPRET
        cursor_turn_deg((-1) * fval);
#endif
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
#ifdef INTERPRET
        cursor_turn_deg((1) * fval);
#endif
        return true;
    }

    return false;
}

bool match_do(FILE *fd)
{
    char buf[BUFSIZE];
    double fval;
#ifdef INTERPRET
    /* Iterator */
    char it_name;
    double it_val;
    double it_start;
    double it_end;
    /* The current position of the file pointer */
    long cur_pos;
#endif

    fscanf(fd, "%s", buf);

    if (is_var(buf)) {
#ifdef INTERPRET
        it_name = buf[0];
#endif
        fscanf(fd, "%s", buf);
        if (!strcmp(buf, FROM)) {
            fscanf(fd, "%s", buf);
            if (is_varnum(buf, &fval)) {
#ifdef INTERPRET
                it_start = fval;
#endif
                fscanf(fd, "%s", buf);
                if (!strcmp(buf, TO)) {
                    fscanf(fd, "%s", buf);
                    if (is_varnum(buf, &fval)) {
#ifdef INTERPRET
                        it_end = fval;
#endif
                        fscanf(fd, "%s", buf);
                        if (!strcmp(buf, LBRC)) {
#ifdef INTERPRET
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

                            /* Go back to a more shallow scope when instruction
                             * list is matched and the loop ends (after '}' is read) */
                             
                            --global_scope;
                            /* SDL_Log("Scope back to %d\n", global_scope); */

                            /* SDL_Log("Do-loop ends.\n"); */
#endif
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
#ifdef INTERPRET
    char var_name;
#endif

    fscanf(fd, "%s", buf);

    if (is_var(buf)) {
#ifdef INTERPRET
        var_name = buf[0];
        /* SDL_Log("var_name: %c", var_name); */
#endif
        fscanf(fd, "%s", buf);
        if (is_assign(buf)) {
            if (match_polish(fd, &fval)) {
#ifdef INTERPRET
                /* SDL_Log("fval: %f", fval); */
                fscanf(fd, "%s", buf);
                if (!strcmp(buf, SEMI)) {
                    if (!symtb_insert(&symtb_head, var_name, global_scope, fval)) {
                        /* If insert failed, the variable already exists */
                        symtb_update_val(symtb_head, var_name, global_scope, fval);
                        /* SDL_Log("%c is set to %f\n", var_name, fval); */
                    }
                    return true;
                }
            }
#else
                return true;
            }
#endif
        }
    }
    return false;
}


bool match_polish(FILE *fd, double* fptr)
{
    char buf[BUFSIZE];
    double f1, f2;

    fscanf(fd, "%s", buf);

    /* SDL_Log("Matching polish with %s\n", buf); */

#ifdef INTERPRET
    if (is_varnum(buf, fptr)) {
        return true;
    }
    if (is_op(buf)) {
        if (match_polish(fd, &f1)) {
            /* SDL_Log("Matched <OP>, Matching polish with %f\n", f1); */
            if (match_polish(fd, &f2)) {
                /* SDL_Log("Matched <OP>, Matching polish with %f\n", f2); */
                if (!strcmp(buf, ADD)) {
                    *fptr = f1 + f2;
                }
                else if (!strcmp(buf, SUB)) {
                    *fptr = f1 - f2;
                }
                else if (!strcmp(buf, MUL)) {
                    *fptr = f1 * f2;
                }
                else if (!strcmp(buf, DIV)) {
                    *fptr = f1 / f2;
                }
                else {
                    *fptr = 0;
                }
                return true;
            }
        }
    }
#else
    if (!strcmp(buf, SEMI)) {
        return true;
    }
    if (is_op(buf)) {
        if (match_polish(fd, &f1)) {
            return true;
        }
    }
    if (is_varnum(buf, fptr)) {
        if (match_polish(fd, &f2)) {
            return true;
        }
    }
#endif
    return false;
}


bool is_varnum(char* in, double* fptr)
{
    if (is_num(in, fptr)) {
        return true;
    }
    if (is_var(in)) {
#ifdef INTERPRET
        symtb_find_val(symtb_head, in[0], global_scope, fptr);
#endif
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

bool is_assign(char *in)
{
    if (!strcmp(in, ASSIGN)) {
        return true;
    }
    return false;
}
