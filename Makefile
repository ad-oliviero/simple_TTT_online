SRC = .
CC = gcc
CFLAGS = -O3
CFLAGS_W64 = -Wl,--subsystem,windows
LDFLAGS = -L $(SRC)/lib/raylib -l:libraylib.a -lpthread -lm -ldl
LDFLAGS_W64 = -L $(SRC)/lib/raylib -l:libraylib.a -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32
NAME = Simple_TTT
.PHONY: server

build: main client gui shapes gameplay server
	$(CC) $(CFLAGS) -o $(NAME) *.o $(LDFLAGS)

build_windows: CFLAGS = $(CFLAGS_W64)
build_windows: LDFLAGS = $(LDFLAGS_W64)
build_windows: build

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
	rm $(NAME) *.o server

debug_build: CFLAGS = -g -Wall -Wextra
debug_build: build
debug_run: debug_build run

debug_windows: CFLAGS = -g -Wall -Wextra
debug_windows: LDFLAGS = $(LDFLAGS_W64)
debug_windows: build
