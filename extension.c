#include "header/general.h"
#include "header/parser.h"
#include "header/cursor.h"

int main(int argc, char* argv[])
{
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

    /* Open file to read and test if success */
    if ((fd = fopen(filename, "r")) == NULL) {
        SDL_Log("Cannot open file.\n");
        exit(EXIT_FAILURE);
    }

    /* Initialize the linked list of cursors */
    cursor_head = cursor_init(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    /* Start parsing/matching */
    if (match_main(fd)) {
        success = true;
    }

    /* Close the file */
    fclose(fd);

    /* End the parser if the input program is invalid */
    if (!success) {
        SDL_Log("Parser failed.\n");
        exit(EXIT_FAILURE);
    }

    /* If parsing succeeded, start a SDL window to draw output */
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        SDL_bool done;

        /* Random numbers */
        int r1, r2, r3;

        if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) == 0) {

            /* Extract the basename (filename) from a compelete path, use it as window name */
            SDL_SetWindowTitle(window, basename(filename));

            done = SDL_FALSE;

            while (!done) {
                SDL_Event event;

                /* Create random numbers in a range of 0 to 255 */
                srand(time(NULL));
                r1 = rand() % (MAX_NUMBER + 1 - MIN_NUMBER) + MIN_NUMBER;
                r2 = rand() % (MAX_NUMBER + 1 - MIN_NUMBER) + MIN_NUMBER;
                r3 = rand() % (MAX_NUMBER + 1 - MIN_NUMBER) + MIN_NUMBER;

                SDL_SetRenderDrawColor(renderer, r1, r2, r3, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, r3, r1, r2, SDL_ALPHA_OPAQUE);

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
    /* SDL_Log("Parser succeeded!\n"); */

    return 0;
}
