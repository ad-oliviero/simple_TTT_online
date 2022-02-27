# Simple_TTT is a simple tic tac toe online game.

The game was built using [raylib](raylib.com), you can use this code by accepting the terms of the [license](https://github.com/TheDarkBug/simple_TTT_online/blob/main/LICENSE).

# Contributing

All kinds of contribution is accepted, just make sure to test it before sending a pull request.

# Downloading and Building

## Building on Linux
Download the repository with
```shell
$ git clone https://github.com/TheDarkBug/simple_TTT_online.git
$ cd simple_TTT_online
```
Download and build [raylib](https://github.com/raysan5/raylib):
```shell
$ cd lib
$ ./setup_raylib.sh
$ cd ..
```

### Build *for* android
Run the `setup_android.sh` script

After that, you can compile:
```shell
$ make PLATFORM=android
```

### Build *for* linux
```shell
$ make
```

### Build *for* windows
```shell
$ make PLATFORM=linux_win
```
---

## Building on Windows
Currently none of my computers has windows installed. Some time in the future I'll write a guide, I promise.

---

## Building on Mac OsX

I do not own a mac, so I don't know how you should build the program, anyway i think that the procedure is similar to the [linux](https://github.com/TheDarkBug/simple_TTT_online/blob/main/README.md#linux) one. Good luck!
