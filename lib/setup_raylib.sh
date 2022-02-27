# this is a script to download and build raylib for the chosen platform

[ ! -d raylib ] && git clone https://github.com/raysan5/raylib.git raylib
cd raylib/src

[ -d linux ] && rm -r linux windows web

# building for linux
printf "\x1b[32mBuilding for linux...\x1b[0m\n"
sleep 1
make clean
mkdir linux
make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=STATIC BUILD_MODE=RELEASE
mv libraylib.a linux
make clean
make PLATFORM=PLATFORM_DESKTOP BUILD_MODE=RELEASE RAYLIB_LIBTYPE=SHARED CFLAGS+=-fPIC
mv libraylib.so linux

# building for android
printf "\x1b[32mBuilding for android...\x1b[0m\n"
sleep 1
make clean
export ANDROID_SDK=../../../android-sdk
export ANDROID_NDK=../../../android-ndk
export ANDROID_ARCH=arm
make PLATFORM=PLATFORM_ANDROID
mkdir ../../armeabi-v7a
mv libraylib.a ../../armeabi-v7a

# building for windows
printf "\x1b[32mBuilding for windows...\x1b[0m\n"
sleep 1
make clean
mkdir windows
make PLATFORM=PLATFORM_DESKTOP BUILD_MODE=RELEASE CC=x86_64-w64-mingw32-gcc
make PLATFORM=PLATFORM_DESKTOP BUILD_MODE=RELEASE CC=x86_64-w64-mingw32-gcc RAYLIB_LIBTYPE=SHARED PLATFORM_OS=WINDOWS RAYLIB_RES_FILE=
mv -t windows raylib.dll libraylib.a libraylibdll.a

# building for web
printf "\x1b[32mBuilding for web...\x1b[0m\n"
sleep 1
make clean
mkdir web
# a line from raylib's Makefile must be changed to use sockets
# 	sed -i "s/CFLAGS = -Wall -D_DEFAULT_SOURCE -D\$(PLATFORM) -D\$(GRAPHICS) -Wno-missing-braces -Werror=pointer-arith -fno-strict-aliasing \$(CUSTOM_CFLAGS)/override CFLAGS += -Wall -D_DEFAULT_SOURCE -D\$(PLATFORM) -D\$(GRAPHICS) -Wno-missing-braces -Werror=pointer-arith -fno-strict-aliasing \$(CUSTOM_CFLAGS)/g" Makefile
make EMSDK_PATH=/usr/lib/emsdk EMSCRIPTEN_PATH=\$\(EMSDK_PATH\)/upstream/emscripten PYTHON_PATH=/usr/lib/python3.10 CC=\$\(EMSCRIPTEN_PATH\)/emcc AR=\$\(EMSCRIPTEN_PATH\)/emar PLATFORM=PLATFORM_WEB PATH=\$\(shell\ printenv\ PATH\):\$\(EMSDK_PATH\):\$\(EMSCRIPTEN_PATH\):\$\(CLANG_PATH\):\$\(NODE_PATH\):\$\(PYTHON_PATH\) -B
mv libraylib.a web

cp raylib.h extras
