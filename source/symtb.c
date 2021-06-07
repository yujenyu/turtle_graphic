#include "../header/symtb.h"

Symtb* symtb_init(void)
{
    Symtb* new = (Symtb*)calloc(1,sizeof(Symtb));
    if (new == NULL) {
        SDL_Log("Cannot calloc() space.\n");
        exit(EXIT_FAILURE);
    }
    return new;
}

bool symtb_find_entry(Symtb* tb, Symtb** rt, char var_name, int var_scope)
{
    *rt = NULL;

    /* Traverse along the list */
    while (tb != NULL) {
        if (tb->sym_name == var_name && tb->sym_scope <= var_scope) {
            if (*rt == NULL) {
                *rt = tb;
                /* Not break immediately while find a qualified one since
                   there may be a more suitable one (in a deeper scope) */
            }
            else {
                if ((*rt)->sym_scope < tb->sym_scope) {
                    /* Update only when the applicable symbol is more "local" */
                    *rt = tb;
                }
            }
        }
        tb = tb->next;
    }

    if (*rt == NULL) {
        return false;
    }

    return true;
}

bool symtb_find_val(Symtb* tb, char var_name, int var_scope, double* rt_val)
{
    Symtb* target;

    if (symtb_find_entry(tb, &target, var_name, var_scope)) {
        *rt_val = target->sym_val;
        return true;
    }

    return false;
}

bool symtb_update_val(Symtb* tb, char var_name, int var_scope, double var_val)
{
    Symtb* target;

    if (symtb_find_entry(tb, &target, var_name, var_scope)) {
        target->sym_val = var_val;
        return true;
    }

    return false;
}

bool symtb_insert(Symtb** tb, char var_name, int var_scope, double var_val)
{
    Symtb* endptr = NULL;
    /* Iterator */
    Symtb* it = *tb;

    /* Traverse along the list */
    while (it != NULL) {
        if (it->sym_name == var_name && it->sym_scope == var_scope) {
            return 0;
            /* The target entry already existed, failed to insert */
        }
        endptr = it;
        it = it->next;
    }

    /* The list is empty */
    if (endptr == NULL) {
        *tb = symtb_init();
        endptr = *tb;
        endptr->prev = NULL;
    }
    else {
        endptr->next = symtb_init();
        endptr->next->prev = endptr;
        endptr = endptr->next;
    }

    endptr->sym_name  = var_name;
    endptr->sym_scope = var_scope;
    endptr->sym_val   = var_val;

    return true;
}

void symtb_delete(Symtb* tb, int var_scope)
{
    /* Iterator */
    Symtb* it = tb;
    Symtb* temp;

    /* Traverse along the list */
    while (it != NULL) {
        if (it->sym_scope >= var_scope) {
            if (it->prev != NULL) {
                it->prev->next = it->next;
            }
            if (it->next != NULL) {
                it->next->prev = it->prev;
            }
            temp = it->next;
            free(it);
            it = temp;
        }
        else {
            it = it->next;
        }
    }
}
