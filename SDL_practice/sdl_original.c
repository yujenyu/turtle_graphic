#include <SDL2/SDL.h>
#include <math.h>

#define WINDOW_WIDTH  (640)
#define WINDOW_HEIGHT (480)

static double global_deg;
static double global_cursor_x = WINDOW_WIDTH/2;
static double global_cursor_y = WINDOW_HEIGHT/2;

/* Input how many distance moving forward */
void SDL_Forward(SDL_Renderer* renderer, double len);
/* Input how many degree to turn */
void SDL_Turn_Deg(double deg);

int main()
{
    int i;

    /* Move the cursor direction to "up" */
    SDL_Turn_Deg(210);

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

                for (i = 0; i < 3; ++i) {
                    SDL_Turn_Deg(120);
                    SDL_Forward(renderer, 40);
                }
                /* SDL_RenderDrawLine(renderer, 320, 200, 300, 240); */
                /* SDL_RenderDrawLine(renderer, 300, 240, 340, 240); */
                /* SDL_RenderDrawLine(renderer, 340, 240, 320, 200); */
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

void SDL_Forward(SDL_Renderer* renderer, double len) {
    double radian = global_deg * (M_PI / 180.0);

    SDL_RenderDrawLine(renderer,
                       global_cursor_x,
                       global_cursor_y,
                       global_cursor_x + len * sin(radian),
                       global_cursor_y + len * cos(radian));

    global_cursor_x += len * sin(radian);
    global_cursor_y += len * cos(radian);
}

void SDL_Turn_Deg(double deg) {
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
