#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifdef INTERPRET
/* For M_PI, sin(), and cos() */
#include <math.h>
/* For SDL */
#include <SDL2/SDL.h>
/* For "basename" func */
#include <libgen.h>
#endif

#ifdef EXTENS
#include <time.h>
#endif

typedef enum bool {false, true} bool;

#endif
