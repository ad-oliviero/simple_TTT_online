SRC = .
CC = gcc
NAME = Simple_TTT
ifeq ($(shell uname), linux)
	CFLAGS = -O3
	LDFLAGS = -L $(SRC)/lib/raylib -l:libraylib.a -lpthread -lm -ldl
else ifeq ($(shell uname), windows32)
	CFLAGS = -Wl,--subsystem,windows
	LDFLAGS = -L $(SRC)/lib/raylib -l:libraylib.a -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32
endif

.PHONY: server

build: main client gui shapes gameplay server
	$(CC) $(CFLAGS) -o $(NAME) *.o $(LDFLAGS)


main:
	$(CC) -c -o $(SRC)/$@.o $(SRC)/$@.c

client:
	$(CC) -c -o $(SRC)/$@.o $(SRC)/$@.c

gui:
	$(CC) -c -o $(SRC)/$@.o $(SRC)/$@.c

shapes:
	$(CC) -c -o $(SRC)/$@.o $(SRC)/$@.c

gameplay:
	$(CC) -c -o $(SRC)/$@.o $(SRC)/$@.c

server:
	$(CC) -c -o $(SRC)/$@.o $(SRC)/$@.c	

run:
	$(SRC)/$(NAME)&$(SRC)/$(NAME)

clean:
	rm $(NAME) *.o server *.exe

debug_build: CFLAGS = -g -Wall -Wextra
debug_build: build
debug_run: debug_build run
