# [Simple TTT](https://github.com/TheDarkBug/simple_TTT) is a simple tic tac toe (as the name suggests) online.

Made with [raylib](raylib.com) and sockets.

The code used in this project is not taken from other sources and you can use it by accepting the terms of the [license](https://github.com/TheDarkBug/simple_TTT_online/blob/main/LICENSE).

# Contributing

All kinds of contribution is accepted, just make sure to test it before pushing.

# Downloading and Building

## Building on Linux

Download the repository with

```shell
$ git clone https://github.com/TheDarkBug/simple_TTT_online.git
$ cd simple_TTT_online
```

### Build *for* android
Run the `setup_android.sh` script

After that, you can compile:
```shell
make PLATFORM=android
```
### Build *for* linux
Download and build [raylib](https://github.com/raysan5/raylib):
```shell
$ cd lib
$ ./setup_raylib.sh
$ cd ..
```
Compile:

```shell
$ make
```
### Build *for* windows
--- Currently broken ---
---
## Mac OsX

I haven't a mac, so i don't know how you should build the program, anyway i think that the procedure is similar to the [linux](https://github.com/TheDarkBug/simple_TTT_online/blob/main/README.md#linux) one. Good luck!
