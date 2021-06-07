CFLAGS = -Wall -Wextra -Wfloat-equal -pedantic -std=c90
OPT = -O3
SANITIZE = -fsanitize=address -fsanitize=undefined -g3
LDLIBS = -lm
CC = gcc
SDL = -lSDL2

parser: parser.c 
	$(CC) parser.c -o parser $(CFLAGS)
parser_o: parser.c 
	$(CC) parser.c -o parser_o $(CFLAGS) $(OPT)
parser_s: parser.c 
	$(CC) parser.c -o parser_s $(CFLAGS) $(OPT) $(SANITIZE)
parser_revised: parser_revised.c 
	$(CC) parser_revised.c -o parser_revised $(CFLAGS)
all:
	
clean:
	rm -f parser parser_o parser_s parser_revised
run: