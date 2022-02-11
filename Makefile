CC = gcc
SRC_DIR = src
SRCS = main.c gui.c server.c client.c bot.c shapes.c gameplay.c
BUILD_TYPE ?= DEBUG
ifeq ($(BUILD_TYPE), DEBUG)
	CFLAGS = -g -Wall -Wextra
else
	CFLAGS = -O2
endif
OBJS = $(SRCS:.c=.o)
OBJ_DIR = obj
BUILD_DIR = build
LIBRAYLIB = libraylib.a
PLATFORM ?= $(shell uname)
ifeq ($(PLATFORM), Linux)
	RAYLIB_PATH = lib/raylib/src/linux
	LDFLAGS = -lpthread -lm -ldl -L $(RAYLIB_PATH) -l:$(LIBRAYLIB)
else ifeq ($(PLATFORM), windows32)
	RAYLIB_PATH = lib/raylib/src/windows
	CFLAGS += -Wl,--subsystem,windows
	LDFLAGS =  -L $(RAYLIB_PATH) -l:$(LIBRAYLIB) -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32
else ifeq ($(PLATFORM), linux_win)
	RAYLIB_PATH = lib/raylib/src/windows
	CC = x86_64-w64-mingw32-gcc
	CFLAGS += -Wl,--subsystem,windows
	LDFLAGS = -lopengl32 -lwinmm -lgdi32 -static -lwinpthread -lwsock32 -L $(RAYLIB_PATH) -l:$(LIBRAYLIB)
else ifeq ($(PLATFORM), web)
	RAYLIB_PATH = lib/raylib/src/web
	EMSDK_PATH = /usr/lib/emsdk/upstream/emscripten
	CC = $(EMSDK_PATH)/emcc
	CFLAGS = -std=c99 -Os -s -O1 -s ASYNCIFY -s USE_GLFW=3 -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 # -lwebsocket.js -s PROXY_POSIX_SOCKETS=1 -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD=1
	LDFLAGS = $(RAYLIB_PATH)/$(LIBRAYLIB)
else ifeq ($(PLATFORM), android)
	TEAM_NAME = thedarkbug
	JAVA_HOME = /usr/lib/jvm/java-17-openjdk/bin
	ANDROID_SDK_PATH = ../android-sdk
	ANDROID_NDK_PATH = ../android-ndk
	OBJ_DIR = obj
	SRC_DIR = src
	LIB_DIR = lib/armeabi-v7a
	ANDROID_TOOLCHAIN = ../android_toolchain_ARM_API30
	CC = $(ANDROID_TOOLCHAIN)/bin/arm-linux-androideabi-gcc
	CFLAGS = -I$(RAYLIB_PATH) -I$(ANDROID_NDK_PATH)/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/android/ -I$(ANDROID_NDK_PATH)/sources/android/native_app_glue -std=c99 -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall -Wa,--noexecstack -Wformat -Werror=format-security -no-canonical-prefixes -D__ANDROID_API__=30 --sysroot=$(ANDROID_TOOLCHAIN)/sysroot
endif
LDFLAGS += 
TARGET = Simple_TTT
ifeq ($(PLATFORM), web)
	TARGET += .html
endif

.PHONY: all clean

ifeq ($(PLATFORM), web)
color_warn=$(shell echo -e "\033[1;33m")
color_reset=$(shell echo -e "\033[0;0m")
all: $(info $(color_warn)You need to install and setup emsdk.) $(info On arch linux you can use the emsdk aur package and it's path will be "/usr/lib/emsdk/upstream/emscripten" $(color_reset))
else ifeq ($(PLATFORM), android)
all: $(OBJS)
	$(CC) -c $(ANDROID_NDK_PATH)/sources/android/native_app_glue/android_native_app_glue.c -o $(OBJ_DIR)/native_app_glue.o -std=c99 -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall -Wa,--noexecstack -Wformat -Werror=format-security -no-canonical-prefixes -DANDROID -DPLATFORM_ANDROID -D__ANDROID_API__=30

	$(ANDROID_TOOLCHAIN)/bin/arm-linux-androideabi-ar rcs $(OBJ_DIR)/libnative_app_glue.a $(OBJ_DIR)/native_app_glue.o

	$(CC) -o $(LIB_DIR)/lib$(TARGET).so $(OBJ_DIR)/*.o -shared -I. -I../raylib/release/include -I$(ANDROID_NDK_PATH)/sources/android/native_app_glue -Wl,-soname,lib$(TARGET).so -Wl,--exclude-libs,libatomic.a -Wl,--build-id -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--warn-shared-textrel -Wl,--fatal-warnings -u ANativeActivity_onCreate -L. -L$(OBJ_DIR) -L$(LIB_DIR) -lraylib -lnative_app_glue -llog -landroid -lEGL -lGLESv2 -lOpenSLES -latomic -lc -lm -ldl

	$(ANDROID_SDK_PATH)/build-tools/30.0.3/aapt package -f -m -S res -J src -M $(SRC_DIR)/AndroidManifest.xml -I $(ANDROID_SDK_PATH)/platforms/android-30/android.jar

	$(JAVA_HOME)/javac -verbose -source 1.7 -target 1.7 -d $(OBJ_DIR) -bootclasspath /usr/lib/jvm/java-8-openjdk-amd64/jre/lib/jre/lib/rt.jar -classpath $(ANDROID_SDK_PATH)/platforms/android-30/android.jar:$(OBJ_DIR) -sourcepath src src/com/$(TEAM_NAME)/$(TARGET)/R.java src/com/$(TEAM_NAME)/$(TARGET)/NativeLoader.java

	$(ANDROID_SDK_PATH)/build-tools/30.0.3/dx --verbose --dex --output=dex/classes.dex $(OBJ_DIR) 

	$(ANDROID_SDK_PATH)/build-tools/30.0.3/aapt package -f -M $(SRC_DIR)/AndroidManifest.xml -S res -A assets -I $(ANDROID_SDK_PATH)/platforms/android-30/android.jar -F $(BUILD_DIR)/$(TARGET).unsigned.apk dex

	$(ANDROID_SDK_PATH)/build-tools/30.0.3/aapt add $(BUILD_DIR)/$(TARGET).unsigned.apk $(LIB_DIR)/lib$(TARGET).so

	$(JAVA_HOME)/jarsigner -keystore $(TARGET).keystore -storepass whatever -keypass whatever -signedjar $(BUILD_DIR)/$(TARGET).signed.apk $(BUILD_DIR)/$(TARGET).unsigned.apk $(TARGET)Key # generic keystore, will be distributed with a private one

	$(ANDROID_SDK_PATH)/build-tools/30.0.3/zipalign -f 4 $(BUILD_DIR)/$(TARGET).signed.apk $(BUILD_DIR)/$(TARGET).apk

else
all: dirs $(TARGET)
endif
ifeq ($(PLATFORM), linux_win)
all:
	cp $(RAYLIB_PATH)/raylib.dll .
	cp /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll .
endif

ifeq ($(PLATFORM), windows32)
dirs:
	IF not exist $(BUILD_DIR) ( mkdir $(BUILD_DIR) )
	IF not exist $(OBJ_DIR) ( mkdir $(OBJ_DIR) )
else
dirs:
	mkdir -pv $(BUILD_DIR) $(OBJ_DIR) dex assets $(LIB_DIR)
endif

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(TARGET) $(OBJ_DIR)/*.o $(LDFLAGS)

ifeq ($(PLATFORM), web)
$(OBJS): CFLAGS=
endif
$(OBJS): dirs
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/$@ $(SRC_DIR)/$(@F:.o=.c)

ifeq ($(PLATFORM), web)
run: $(TARGET)
	$(EMSDK_PATH)/emrun $(BUILD_DIR)/$(TARGET)
else ifeq ($(PLATFORM), windows32)
run: $(TARGET)
	.\$(BUILD_DIR)\$(TARGET)
else ifeq ($(PLATFORM), android)
run: all
	$(ANDROID_SDK_PATH)/platform-tools/adb install -r $(BUILD_DIR)/$(TARGET).apk
	$(ANDROID_SDK_PATH)/platform-tools/adb shell am force-stop com.$(TEAM_NAME).$(TARGET)
	$(ANDROID_SDK_PATH)/platform-tools/adb shell am start -n com.$(TEAM_NAME).$(TARGET)/com.$(TEAM_NAME).$(TARGET).NativeLoader

else
run: $(TARGET)
	$(BUILD_DIR)/$(TARGET)
endif

clean:
	rm -r $(BUILD_DIR) $(OBJ_DIR) dex
