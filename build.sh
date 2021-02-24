#!/bin/bash
cd src
make linux

if [ $? != 0 ]
then
	notify-send -t 1000 "Error compiling"
fi
