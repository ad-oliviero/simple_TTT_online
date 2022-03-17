# this is a script to download and build raylib for the chosen platform

[ ! -d raylib ] && git clone https://github.com/raysan5/raylib.git raylib
cd raylib
# apply a patch to fix android touch
[ -z "$(git diff)" ] && git apply ../android-touch-fix.diff
cd src

# building for linux
if [ ! -d linux ]; then
	printf "\x1b[32mBuilding for linux...\x1b[0m\n"
	sleep 1
	make clean
	mkdir linux
	make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=STATIC BUILD_MODE=RELEASE
	mv libraylib.a linux
# make clean
# make PLATFORM=PLATFORM_DESKTOP BUILD_MODE=RELEASE RAYLIB_LIBTYPE=SHARED CFLAGS+=-fPIC
# mv libraylib.so linux
fi

# building for android
if [ ! -d ../../armeabi-v7a ]; then
	printf "\x1b[32mBuilding for android...\x1b[0m\n"
	sleep 1
	make clean
	export ANDROID_SDK=../../../android-sdk
	export ANDROID_NDK=../../../android-ndk
	export ANDROID_ARCH=arm
	make PLATFORM=PLATFORM_ANDROID
	mkdir ../../armeabi-v7a
	mv libraylib.a ../../armeabi-v7a
fi

if [ ! -d windows ]; then
	# building for windows
	printf "\x1b[32mBuilding for windows...\x1b[0m\n"
	sleep 1
	make clean
	mkdir windows
	make PLATFORM=PLATFORM_DESKTOP BUILD_MODE=RELEASE CC=x86_64-w64-mingw32-gcc
	mv libraylib.a windows
	make clean
	make PLATFORM=PLATFORM_DESKTOP BUILD_MODE=RELEASE CC=x86_64-w64-mingw32-gcc RAYLIB_LIBTYPE=SHARED PLATFORM_OS=WINDOWS RAYLIB_RES_FILE=
	mv raylib.dll windows
fi

# building for web
if [ ! -d web ]; then
	printf "\x1b[32mBuilding for web...\x1b[0m\n"
	sleep 1
	make clean
	mkdir web
	# a line from raylib's Makefile must be changed to use sockets
	# 	sed -i "s/CFLAGS = -Wall -D_DEFAULT_SOURCE -D\$(PLATFORM) -D\$(GRAPHICS) -Wno-missing-braces -Werror=pointer-arith -fno-strict-aliasing \$(CUSTOM_CFLAGS)/override CFLAGS += -Wall -D_DEFAULT_SOURCE -D\$(PLATFORM) -D\$(GRAPHICS) -Wno-missing-braces -Werror=pointer-arith -fno-strict-aliasing \$(CUSTOM_CFLAGS)/g" Makefile
	make EMSDK_PATH=/usr/lib/emsdk EMSCRIPTEN_PATH=\$\(EMSDK_PATH\)/upstream/emscripten PYTHON_PATH=/usr/lib/python3.10 CC=\$\(EMSCRIPTEN_PATH\)/emcc AR=\$\(EMSCRIPTEN_PATH\)/emar PLATFORM=PLATFORM_WEB PATH=\$\(shell\ printenv\ PATH\):\$\(EMSDK_PATH\):\$\(EMSCRIPTEN_PATH\):\$\(CLANG_PATH\):\$\(NODE_PATH\):\$\(PYTHON_PATH\) -B
	mv libraylib.a web
fi

cd ../..

# if [ ! -d imgui ]; then
printf "\x1b[32mInstalling imgui...\x1b[0m\n"
git clone --recursive git@github.com:ocornut/imgui.git
cd imgui
rm -f *.o
for f in *.cpp; do
	g++ $f -lGL -lglfw -c -o $f.o
done
g++ backends/imgui_impl_glfw.cpp -lGL -lglfw -I. -c -o imgui_impl_glfw.cpp.o
g++ backends/imgui_impl_opengl3.cpp -lGL -lglfw -I. -c -o imgui_impl_opengl3.cpp.o
ar rcs libimgui.a *.o

export JAVA_HOME=/usr/lib/jvm/java-17-openjdk/bin
export ANDROID_SDK_PATH=../../android-sdk
export ANDROID_NDK_PATH=../../android-ndk
export ANDROID_TOOLCHAIN=$ANDROID_NDK_PATH/toolchains/llvm/prebuilt/linux-x86_64
export CC=$ANDROID_TOOLCHAIN/bin/armv7a-linux-androideabi31-clang++
rm -f *.o
for f in *.cpp; do
	$CC $f -c -o $f.o -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall -Wa,--noexecstack -Wformat -Werror=format-security -no-canonical-prefixes -DPLATFORM_ANDROID
done
# $CC backends/imgui_impl_android.cpp -I. -c -o imgui_impl_android.cpp.o -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall -Wa,--noexecstack -Wformat -Werror=format-security -no-canonical-prefixes -DPLATFORM_ANDROID
$CC -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall -Wformat -Werror=format-security -no-canonical-prefixes -DPLATFORM_ANDROID -shared -fPIC *.o -o libimgui.so
# $ANDROID_TOOLCHAIN/bin/llvm-ar rcs ../armeabi-v7a/libimgui.a *.o
mv libimgui.so ../armeabi-v7a
# fi
