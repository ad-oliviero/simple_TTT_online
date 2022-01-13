CC = gcc
SRC_DIR = src
SRCS = main.c gui.c server.c client.c bot.c shapes.c gameplay.c
BUILD_TYPE ?= DEBUG
ifeq ($(BUILD_TYPE), DEBUG)
	CFLAGS = -g -Wall -Wextra
else
	CFLAGS = -O2
endif

ifeq ($(PLATFORM), Linux)
	RAYLIB_PATH = lib/raylib/src/linux
else ifeq ($(PLATFORM), windows32)
	RAYLIB_PATH = lib/raylib/src/windows
else ifeq ($(PLATFORM), linux_win)
	RAYLIB_PATH = lib/raylib/src/windows
else ifeq ($(PLATFORM), web)
	RAYLIB_PATH = lib/raylib/src/web
endif
LIBRAYLIB = libraylib.a
LDFLAGS += -L $(RAYLIB_PATH) -l:$(LIBRAYLIB)
OBJS = $(SRCS:.c=.o)
OBJ_DIR = obj
BUILD_DIR = build
PLATFORM ?= $(shell uname)
TARGET = Simple_TTT

ifeq ($(PLATFORM), Linux)
	LDFLAGS += -lpthread -lm -ldl
else ifeq ($(PLATFORM), windows32)
	CFLAGS += -Wl,--subsystem,windows
	LDFLAGS += -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32
else ifeq ($(PLATFORM), linux_win)
	CC = x86_64-w64-mingw32-gcc
	CFLAGS += -Wl,--subsystem,windows
	LDFLAGS += -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32
else ifeq ($(PLATFORM), web) # currently not working
	EMSDK_PATH = /usr/lib/emsdk/upstream/emscripten
	CC = $(EMSDK_PATH)/emcc
	CFLAGS = -std=c99 -Os -s -O1 -s ASYNCIFY -s USE_GLFW=3 -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 # -lwebsocket.js -s PROXY_POSIX_SOCKETS=1 -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD=1
	LDFLAGS = $(RAYLIB_PATH)/$(LIBRAYLIB)
	TARGET = Simple_TTT.html
else ifeq ($(PLATFORM), android)
	CC = ../../android_toolchain_ARM_API30/bin/arm-linux-androideabi-gcc
	CFLAGS = -I. -I../../lib/raylib/src -I../../android-ndk/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/android/ -I../../android-ndk/sources/android/native_app_glue -std=c99 -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall -Wa,--noexecstack -Wformat -Werror=format-security -no-canonical-prefixes -D__ANDROID_API__=30 --sysroot=../../android_toolchain_ARM_API30/sysroot
endif
.PHONY: all clean

ifeq ($(PLATFORM), web)
color_warn=$(shell echo -e "\033[1;33m")
color_reset=$(shell echo -e "\033[0;0m")
all: $(info $(color_warn)You need to install and setup emsdk.) $(info On arch linux you can use the emsdk aur package and it's path will be "/usr/lib/emsdk/upstream/emscripten" $(color_reset))
else ifeq ($(PLATFORM), android)
all: native_app_glue
native_app_glue:
	$(CC) $(CFLAGS) -c -o $(SRC)/$@.o ../../android-ndk/sources/android/native_app_glue/android_native_app_glue.c
# all:
# 	if [ ! -d "../obj/$(NAME)" ]; then mkdir ../obj/$(NAME); fi
# 	mv *.o ../obj/$(NAME)
endif
all: dirs $(TARGET)
ifeq ($(PLATFORM), linux_win)
all:
	cp $(RAYLIB_PATH)/raylib.dll .
	cp /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll .
endif

dirs:
	mkdir -pv $(BUILD_DIR) $(OBJ_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(TARGET) $(OBJ_DIR)/*.o $(LDFLAGS)

ifeq ($(PLATFORM), web)
$(OBJS): CFLAGS=
endif
$(OBJS): dirs
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $(SRC_DIR)/$(@F:.o=.c)

run: $(TARGET)
ifeq ($(PLATFORM), web)
	$(EMSDK_PATH)/emrun $(BUILD_DIR)/$(TARGET)
else
	$(BUILD_DIR)/$(TARGET) #&$(BUILD_DIR)/$(TARGET)
endif

clean:
	rm -r $(BUILD_DIR) $(OBJ_DIR)
