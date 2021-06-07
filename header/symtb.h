#ifndef SYMTB_H
#define SYMTB_H

#include "general.h"

typedef struct symbol_table {
    char sym_name;
    int sym_scope;
    double sym_val;
    struct symbol_table* next;
    struct symbol_table* prev;
} Symtb;

/* Initialize the symbol table */
Symtb* symtb_init(void);
/* The internal function to find an entry out of the symbol table */
bool symtb_find_entry(Symtb*, Symtb**, char, int);
/* The function to read the value for a given symbol from the symbol table. */
bool symtb_find_val(Symtb*, char, int, double*);
/* The function to write the value to a target symbol from the symbol table */
bool symtb_update_val(Symtb*, char, int, double);
/* The function insert a new entry to the symbol table */
bool symtb_insert(Symtb**, char, int, double);
/* The function delete invalid entries when the scope decreased */
void symtb_delete(Symtb*, int);

#endif
