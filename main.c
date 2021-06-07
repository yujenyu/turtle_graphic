#include "header/general.h"
#include "header/parser.h"

#ifdef INTERPRET
#include "header/cursor.h"
#endif

int main(int argc, char* argv[])
{
    /* File descriptor */
    FILE *fd;
    /* The second element of the argument vector */
    char* filename = argv[1];
    bool success = false;

#ifdef INTERPRET
    Cursor *cursor_head;
    Cursor* csr;
#endif

    /* Check the argument */
    if(argc != 2){
        fprintf(stderr, "Usage : %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

#ifdef INTERPRET
    /* SDL_Log("Input filename: %s\n", filename); */
#else
    /* printf("Input filename: %s\n", filename); */
#endif

    /* Open file to read and test if success */
    if ((fd = fopen(filename, "r")) == NULL) {

#ifdef INTERPRET
        SDL_Log("Cannot open file.\n");
#else
        printf("Cannot open file.\n");
#endif
        exit(EXIT_FAILURE);
    }

#ifdef INTERPRET
    /* Initialize the linked list of cursors */
    cursor_head = cursor_init(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
#endif

    /* Start parsing/matching */
    if (match_main(fd)) {
        success = true;
    }

    /* Close the file */
    fclose(fd);

    /* End the parser if the input program is invalid */
    if (!success) {

#ifdef INTERPRET
        SDL_Log("Parser failed.\n");
#else
        printf("Parser failed.\n");
#endif
        exit(EXIT_FAILURE);
    }

#ifdef INTERPRET
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
    /* SDL_Log("Parser succeeded!\n"); */
#else
    /* printf("Parser succeeded!\n"); */
#endif
    return 0;
}
