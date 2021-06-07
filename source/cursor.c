#include "../header/cursor.h"

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
