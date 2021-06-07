CC = clang
COMMON = -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
SANITIZE = -fsanitize=address -fsanitize=undefined
DEBUG = -g3
LDLIBS = -lm
PRODUCTION = $(COMMON) -O3
SDL = -lSDL2
OBJ_NAME = interp
SRCS = ../source/symtb.c
OBJS = $(OBJ_NAME).c $(SRCS)

interp: $(OBJS)
	$(CC) $(OBJS) -o $(OBJ_NAME) $(PRODUCTION) $(LDLIBS) $(SDL)
interp_s: $(OBJS)
	$(CC) $(OBJS) -o interp_s $(COMMON) $(LDLIBS) $(SDL) $(SANITIZE) $(DEBUG)
clean:
	rm -f interp interp_s
run: