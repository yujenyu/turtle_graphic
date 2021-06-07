#ifndef CURSOR_H
#define CURSOR_H

#include "general.h"

/* SDL window size */
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

/* Random number range */
#define MAX_NUMBER (255)
#define MIN_NUMBER (0)

/*
 * Self-defined data type to store the turning points
 * The underlying data structure is a linked-list
 */
typedef struct cursor {
    double x;
    double y;
    struct cursor* next;
} Cursor;

/* The direction of the cursor wrt y axis, starting from pointing "up" */
static double global_deg = 180;

/* The pointer to the end of the linked list */
Cursor *cursor_end;

/* Initialize the linked list of cursors */
Cursor* cursor_init(double init_x, double init_y);
/* Input how many distance moving forward */
void cursor_forward(double len);
/* Input how many degree to turn */
void cursor_turn_deg(double deg);

#endif
