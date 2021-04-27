#!/bin/bash
ls build 2>/dev/null
if [ $? != 0 ]
then
	mkdir build
fi
cd src
../setup_raylib.sh
cd ..
make linux server

if [ $? != 0 ]
then
	notify-send -t 1000 "Error compiling"
fi
