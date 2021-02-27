BUILD_DIR = ../
CC = gcc
CCW64 = x86_64-w64-mingw32-$(CC)
FILES = main.c client.c gui.c shapes.c gameplay.c
FLAGS = -O3 -Wall -Wextra
FLAGS_W64 = $(FLAGS) -Wl,--subsystem,windows
LIBS = -lraylib -lpthread
LIBS_W64 = -L lib/ $(FLAGS) -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32
NAME = "Simple_TTT"
SRC_DIR = 

all: client server

client: linux windows

server:
	$(CC) $(FLAGS) $(LIBS) $(SRC_DIR)server.c $(SRC_DIR)server_gameplay.c $(FLAGS) -o $(BUILD_DIR)server

linux: server
	$(CC) $(FLAGS) $(LIBS) $(SRC_DIR)$(FILES) -o $(BUILD_DIR)$(NAME)
	@echo Compilation Successfull!

windows:
	$(CCW64) $(FLAGS_W64) $(LIBS_W64) $(SRC_DIR)$(FILES) -o $(BUILD_DIR)$(NAME).exe
