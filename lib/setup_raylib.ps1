# this is a script to download and build raylib for the chosen platform

if (![System.IO.File]::Exists(".\raylib")) {
    git clone https://github.com/raysan5/raylib.git raylib
}

Set-Location raylib/src

if (Test-Path -Path "windows") {
	Remove-Item -Recurse windows
}

# building for windows
printf "\x1b[32mBuilding for windows...\x1b[0m\n"
Start-Sleep 1
make clean
mkdir windows
make PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=STATIC BUILD_MODE=RELEASE
Move-Item libraylib.a windows
make clean
make PLATFORM=PLATFORM_DESKTOP BUILD_MODE=RELEASE RAYLIB_LIBTYPE=SHARED PLATFORM_OS=WINDOWS RAYLIB_RES_FILE=
Move-Item raylib.dll windows
Copy-Item windows\raylib.dll ../../..

Copy-Item raylib.h extras

Set-Location ../..
