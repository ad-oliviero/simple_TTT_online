SRC = .
CC = gcc
FILES = $(SRC)/main.c $(SRC)/client.c $(SRC)/gui.c $(SRC)/shapes.c $(SRC)/gameplay.c
OBJS = $(SRC)/main.o $(SRC)/client.o $(SRC)/gui.o $(SRC)/shapes.o $(SRC)/gameplay.o
CFLAGS = -O3
CFLAGS_W64 = -Wl,--subsystem,windows
LDFLAGS = -L $(SRC)/lib/raylib -l:libraylib.a -lpthread -lm -ldl
LDFLAGS_W64 = -L $(SRC)/lib/raylib -l:libraylib.a -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32
NAME = Simple_TTT

build: main client gui shapes gameplay
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

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
	$(CC) $(CFLAGS) -o $(SRC)/$@ $(SRC)/$@.c $(SRC)/$@_gameplay.c -lpthread

run:
	@#$(SRC)/server&$(SRC)/$(NAME)&$(SRC)/$(NAME)
	$(SRC)/$(NAME)

clean:
	rm $(SRC)/$(NAME) $(OBJS) server

debug_build: CFLAGS = -g -Wall -Wextra
debug_build: build
debug_run: debug_build run

debug_windows: CFLAGS = -g -Wall -Wextra
debug_windows: LDFLAGS = $(LDFLAGS_W64)
debug_windows: build