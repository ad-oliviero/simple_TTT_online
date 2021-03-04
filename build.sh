#!/bin/bash
ls build
if [ $? != 0 ]
then
	mkdir build
fi
make linux server

if [ $? != 0 ]
then
	notify-send -t 1000 "Error compiling"
fi
