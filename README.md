# [Simple TTT](https://github.com/TheDarkBug/simple_TTT) is a simple tic tac toe (as the name suggests) online.

Made with [raylib](raylib.com) and sockets.

The code used in this project is not taken from other sources and you can use it by accepting the terms of the [license](https://github.com/TheDarkBug/simple_TTT_online/blob/main/LICENSE).

# Contributing

All kinds of contribution is accepted, just make sure to test it before pushing.

# Downloading and Building

## Linux

Download the repository with

```shell
git clone https://github.com/TheDarkBug/simple_TTT_online.git && cd simple_TTT_online
```

Build [raylib for linux](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux) and move `libraylib.a` to `src/lib/raylinux`

and use buils.sh to build for linux

or this commands to build with make

```shell
cd src
make all        #build linux client and server, and windows client
make client     #build only linux and windows clients
make server     #build only server
make linux      #build only linux client
make windows    #build only windows client
```

## Windows

To build for windows i used mingw on linux, if you want to build directly on windows, the steps are similar

First of all, we need to install dependances, here is how you can do it on arch based distributions

```shell
sudo pacman -S mingw-w64-gcc mingw-w64-winpthreads mingw-w64-binutils
```

Build [raylib for windows](https://github.com/raysan5/raylib/wiki/Working-on-Windows) and move `libraylib.a` to `src/lib/raywindows`

After that you can build your exe with

```shell
cd simple_TTT_online/src
make windows
```

## Mac OsX

I haven't a mac, so i don't know how you should build the program, anyway i think that the procedure is similar to the [linux](https://github.com/TheDarkBug/simple_TTT_online/blob/main/README.md#linux) one. Good luck!

# Usage

After building you must run the server executable on a linux machine, than you can start the two clients.
