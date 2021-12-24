# raylib
rm -rf raylib
git clone https://github.com/raysan5/raylib.git raylib
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
cp raylib.h extras
