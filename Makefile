SRC = .
CC = gcc
NAME = Simple_TTT
CFLAGS = -Wall -Wextra -std=c99
OBJ_FLAGS = -c $(CFLAGS)
PLATFORM = $(shell uname)
ifeq ($(PLATFORM), Linux)
	CFLAGS = -O3
	LDFLAGS = -L $(SRC)/lib/raylib -l:libraylib.a -lpthread -lm -ldl
else ifeq ($(PLATFORM), windows32)
	CFLAGS = -Wl,--subsystem,windows
	LDFLAGS = -L $(SRC)/lib/raylib -l:libraylib.a -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32
else ifeq ($(PLATFORM), linux_win)
	CC = x86_64-w64-mingw32-gcc
	CFLAGS = -Wl,--subsystem,windows
	LDFLAGS = -L $(SRC)/lib/raylib -l:libraylib.a -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32
endif
.PHONY: server

build: main client gui shapes gameplay server bot
	$(CC) $(CFLAGS) -o $(NAME) *.o $(LDFLAGS)

main:
	$(CC) -c $(SRC)/$@.c -o $(SRC)/$@.o	$(OBJ_FLAGS)

client:
	$(CC) -c $(SRC)/$@.c -o $(SRC)/$@.o	$(OBJ_FLAGS)

gui:
	$(CC) -c $(SRC)/$@.c -o $(SRC)/$@.o	$(OBJ_FLAGS)

shapes:
	$(CC) -c $(SRC)/$@.c -o $(SRC)/$@.o	$(OBJ_FLAGS)

gameplay:
	$(CC) -c $(SRC)/$@.c -o $(SRC)/$@.o	$(OBJ_FLAGS)

server:
	$(CC) -c $(SRC)/$@.c -o $(SRC)/$@.o	$(OBJ_FLAGS)

bot:
	$(CC) -c $(SRC)/$@.c -o $(SRC)/$@.o	$(OBJ_FLAGS)

run:
	$(SRC)/$(NAME) #&$(SRC)/$(NAME)

clean:
	rm $(NAME) *.o *.exe

debug_build: CFLAGS = -g -Wall -Wextra
debug_build: build
debug_run: debug_build run

android: CC = ../../android_toolchain_ARM_API22/bin/arm-linux-androideabi-gcc
android: OBJ_FLAGS = -I. -I../../raylib/release/include -I../../raylib/src/external/android/native_app_glue -std=c99 -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall -Wa,--noexecstack -Wformat -Werror=format-security -no-canonical-prefixes -DANDROID -D__ANDROID_API__=22 --sysroot=../../android_toolchain_ARM_API22/sysroot
android: main client gui shapes gameplay server bot
android:
	if [ ! -d "../obj/$(NAME)" ]; then mkdir ../obj/$(NAME); fi
	mv *.o ../obj/$(NAME)

