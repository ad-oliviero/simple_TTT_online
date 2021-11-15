# raylib
rm -rf raylib
git clone https://github.com/raysan5/raylib.git raylib-git
cd raylib-git/src
make PLATFORM=PLATFORM_DESKTOP CC=x86_64-w64-mingw32-gcc
cd ../../
mkdir -p raylib/include
cp raylib-git/src/raylib.h raylib/include
cp raylib-git/libraylib.a raylib/

# raygui
cd ../../
git clone https://github.com/raysan5/raygui.git
cp ./raylib/src/raylib.h ./raygui/src
