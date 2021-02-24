# [Simple TTT](https://github.com/TheDarkBug/simple_TTT) is a simple tic tac toe (as the name suggests) online.

Made with [raylib](raylib.com) (precompiled library for windows included) and unix (or windows) sockets.

The code used in this project is not taken from other sources and you can use it by accepting the terms of the [license](https://github.com/TheDarkBug/simple_TTT_online/blob/main/LICENSE).

# Contributing

All kinds of contribution is accepted, just make sure to test it before pushing.

# Downloading and Building

## Linux

Download the repository with

```shell
git clone https://github.com/TheDarkBug/simple_TTT_online.git
```

[Install raylib](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux)

Use this commands in the terminal to build for linux

```shell
cd simple_TTT_online
chmod +x build.sh
./build.sh
```

or this commands to build with make

```shell
cd simple_TTT_online/src

make linux      #build for linux, both server ad client
make server     #build only linux server
make all        #build everything, windows included
```

## Windows

To build for windows i used mingw on linux, if you want to build directly on windows, the steps are similar

First of all, we need to install dependances, here is how you can do it on arch based distributions

```shell
sudo pacman -S mingw-w64-gcc mingw-w64-winpthreads mingw-w64-binutils
```

[Install raylib](https://github.com/raysan5/raylib/wiki/Working-on-Windows)

After that you can build your exe with

```shell
cd simple_TTT_online/src
make windows
```

# Usage

After building you must run the server executable on a linux machine, than you can start the two clients.
