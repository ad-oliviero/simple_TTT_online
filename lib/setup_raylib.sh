# this is a script to download and build raylib for the chosen platform

[ ! -n "$1" ] && printf "Usage: $0 <platform>\nAvailable platforms:\n	'windows'\n	'linux'\n	'web'\n" && exit 1
[ ! -d raylib ] && git clone https://github.com/raysan5/raylib.git raylib
cd raylib/src

if [ $1 == "linux" ]
then
	make PLATFORM=PLATFORM_DESKTOP BUILD_MODE=RELEASE
elif [ $1 == "windows" ] # compiling raylib for windows from linux
then
	make PLATFORM=PLATFORM_DESKTOP BUILD_MODE=RELEASE CC=x86_64-w64-mingw32-gcc
elif [ $1 == "web" ] # before building you should install emsdk and set it's path
then
	# a line from raylib's Makefile must be changed to use sockets

	sed -i "s/CFLAGS = -Wall -D_DEFAULT_SOURCE -D\$(PLATFORM) -D\$(GRAPHICS) -Wno-missing-braces -Werror=pointer-arith -fno-strict-aliasing \$(CUSTOM_CFLAGS)/override CFLAGS += -Wall -D_DEFAULT_SOURCE -D\$(PLATFORM) -D\$(GRAPHICS) -Wno-missing-braces -Werror=pointer-arith -fno-strict-aliasing \$(CUSTOM_CFLAGS)/g" Makefile
	
	make CFLAGS="" EMSDK_PATH=/usr/lib/emsdk EMSCRIPTEN_PATH=\$\(EMSDK_PATH\)/upstream/emscripten PYTHON_PATH=/usr/lib/python3.10 CC=\$\(EMSCRIPTEN_PATH\)/emcc AR=\$\(EMSCRIPTEN_PATH\)/emar PLATFORM=PLATFORM_WEB PATH=\$\(shell\ printenv\ PATH\):\$\(EMSDK_PATH\):\$\(EMSCRIPTEN_PATH\):\$\(CLANG_PATH\):\$\(NODE_PATH\):\$\(PYTHON_PATH\) -B
else
	printf "ERROR: $1 is not a valid platform.\nAvailable platforms:\n	'windows'\n	'linux'\n	'web'\n"
fi
cp raylib.h extras
