CFLAGS = -Wall -Wextra -Wfloat-equal -pedantic -std=c90
OPT = -O3
SANITIZE = -fsanitize=address -fsanitize=undefined -g3
LDLIBS = -lm
CC = clang
SDL = -lSDL2

sdl_ex: sdl_ex.c
	$(CC) sdl_ex.c -o sdl_ex $(CFLAGS) $(LDLIBS) $(SDL)
sdl_original: sdl_original.c
	$(CC) sdl_original.c -o sdl_original $(CFLAGS) $(LDLIBS) $(SDL)
sdl_llist: sdl_llist.c
	$(CC) sdl_llist.c -o sdl_llist $(CFLAGS) $(LDLIBS) $(SDL)
sdl_triangle: sdl_triangle.c
	$(CC) sdl_triangle.c -o sdl_triangle $(CFLAGS) $(LDLIBS) $(SDL)
sdl_octloop: sdl_octloop.c
	$(CC) sdl_octloop.c -o sdl_octloop $(CFLAGS) $(LDLIBS) $(SDL)
sdl_var: sdl_var.c
	$(CC) sdl_var.c -o sdl_var $(CFLAGS) $(LDLIBS) $(SDL)
sdl_nestloop: sdl_nestloop.c
	$(CC) sdl_nestloop.c -o sdl_nestloop $(CFLAGS) $(LDLIBS) $(SDL)
clean:
	rm -f sdl_ex sdl_original sdl_llist sdl_triangle sdl_octloop sdl_var sdl_nestloop 
run: 