#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>

#define WINDOW_WIDTH  (640)
#define WINDOW_HEIGHT (480)

typedef struct cursor {
    double x;
    double y;
    struct cursor* next;
} Cursor;

/* The direction of the cursor wrt y axis, starting from pointing "up" */
static double global_deg = 180;

/* The pointer to the end of the linked list */
Cursor *cursor_end;

Cursor* cursor_init(double init_x, double init_y);
void cursor_forward(double len);
void cursor_turn_deg(double deg);

int main()
{
    int i;
    Cursor *cursor_head;
    Cursor* csr;

    /* Initialize the linked list of cursors */
    cursor_head = cursor_init(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    /* Draw image to simulate the output for a logo input program */
    for (i = 1; i <= 8; ++i) {
        cursor_forward(30);
        cursor_turn_deg(45);
    }

    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

                csr = cursor_head;
                if (csr != NULL) {
                    while (csr->next != NULL) {
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
    SDL_Quit();
    return 0;
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

/* Input how many degree to turn */
void cursor_turn_deg(double deg)
{
    global_deg -= deg;

    /* Avoid overflow */
    while (global_deg > 360.0 || global_deg < -360.0) {
        if (global_deg > 360.0) {
            global_deg -= 360.0;
        } else if (global_deg < -360.0) {
            global_deg += 360.0;
        }
    }
}
