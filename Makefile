SRC = .
CC = gcc
NAME = Simple_TTT
PLATFORM = $(shell uname)
ifeq ($(PLATFORM), Linux)
	CFLAGS = -O3
	LDFLAGS = -L $(SRC)/lib/raylib/src -l:libraylib.a -lpthread -lm -ldl
else ifeq ($(PLATFORM), windows32)
	CFLAGS = -Wl,--subsystem,windows
	LDFLAGS = -L $(SRC)/lib/raylib/src -l:libraylib.a -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32
else ifeq ($(PLATFORM), linux_win)
	CC = x86_64-w64-mingw32-gcc
	CFLAGS = -Wl,--subsystem,windows
	LDFLAGS = -L $(SRC)/lib/raylib/src -l:libraylib.a -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32
endif
.PHONY: server

build: main client gui shapes gameplay server bot
	$(CC) $(CFLAGS) -o $(NAME) *.o $(LDFLAGS)

main:
	$(CC) $(CFLAGS) -c -o $(SRC)/$@.o $(SRC)/$@.c

client:
	$(CC) $(CFLAGS) -c -o $(SRC)/$@.o $(SRC)/$@.c

gui:
	$(CC) $(CFLAGS) -c -o $(SRC)/$@.o $(SRC)/$@.c

shapes:
	$(CC) $(CFLAGS) -c -o $(SRC)/$@.o $(SRC)/$@.c

gameplay:
	$(CC) $(CFLAGS) -c -o $(SRC)/$@.o $(SRC)/$@.c

server:
	$(CC) $(CFLAGS) -c -o $(SRC)/$@.o $(SRC)/$@.c	

bot:
	$(CC) $(CFLAGS) -c -o $(SRC)/$@.o $(SRC)/$@.c	

run:
	$(SRC)/$(NAME) #&$(SRC)/$(NAME)

clean:
	rm $(NAME) *.o *.exe

debug_build: CFLAGS = -g -Wall -Wextra
debug_build: build
debug_run: debug_build run

android: CC = ../../android_toolchain_ARM_API30/bin/arm-linux-androideabi-gcc
android: OBJ_FLAGS = -I. -I../../lib/raylib/src -I../../android-ndk/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/android/ -I../../android-ndk/sources/android/native_app_glue -std=c99 -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall -Wa,--noexecstack -Wformat -Werror=format-security -no-canonical-prefixes -D__ANDROID_API__=30 --sysroot=../../android_toolchain_ARM_API30/sysroot
android: CFLAGS = $(OBJ_FLAGS)

native_app_glue:
	$(CC) $(CFLAGS) -c -o $(SRC)/$@.o ../../android-ndk/sources/android/native_app_glue/android_native_app_glue.c
	
android: native_app_glue main client gui shapes gameplay server bot
android:
	if [ ! -d "../obj/$(NAME)" ]; then mkdir ../obj/$(NAME); fi
	mv *.o ../obj/$(NAME)
