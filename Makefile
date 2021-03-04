BUILD = build/
SRC = src/
CC = gcc
CCW64 = x86_64-w64-mingw32-$(CC)
FILES = $(SRC)main.c $(SRC)client.c $(SRC)gui.c $(SRC)shapes.c $(SRC)gameplay.c
FLAGS = -O3 -Wall -Wextra
FLAGS_W64 = $(FLAGS) -Wl,--subsystem,windows
LIBS = -L $(SRC)lib/raylinux/ -l:libraylib.a -lpthread -lm -ldl
LIBS_W64 = -L $(SRC)lib/raywindows/ -l:libraylib.a -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32
NAME = "Simple_TTT"

all: linux windows server
	@echo -e "\033[0;1m\033[0;92mAll done without errors!\033[0m"

client: linux windows

server:
	$(CC) $(FLAGS) server.c $(SRC)server_gameplay.c  $(LIBS) -o $(BUILD)server
	@echo -e "\033[0;33mServer compiled!\033[0m"

linux:
	$(CC) $(FLAGS) $(FILES) $(LIBS) -o $(BUILD)$(NAME)
	@echo -e "\033[0;33mLinux compiled!\033[0m"

windows:
	$(CCW64) $(FLAGS_W64) $(FILES) $(LIBS_W64) -o $(BUILD)$(NAME).exe
	@echo -e "\033[0;33mWindows compiled!\033[0m"