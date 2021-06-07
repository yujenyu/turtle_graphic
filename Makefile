CC = clang
COMMON = -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
SANITIZE = -fsanitize=address -fsanitize=undefined
DEBUG = -g3
LDLIBS = -lm
PRODUCTION = $(COMMON) -O3
SDL = -lSDL2

OBJS = main.c 
PARSE_SRCS = source/parser.c
INTERP_SRCS = source/parser.c source/cursor.c source/symtb.c
INTERP_DEF = INTERPRET
EXTENS_DEF = EXTENS
EXTENS = extension.c

parse: $(OBJS)
	$(CC) $(OBJS) $(PARSE_SRCS) -o parse $(PRODUCTION)
interp: $(OBJS)
	$(CC) $(OBJS) $(INTERP_SRCS) -o interp -D$(INTERP_DEF) $(PRODUCTION) $(LDLIBS) $(SDL)
extension:
	$(CC) $(EXTENS) $(INTERP_SRCS) -o extension -D$(INTERP_DEF) -D$(EXTENS_DEF) $(PRODUCTION) $(LDLIBS) $(SDL)
clean:
	rm -f parse interp extension

