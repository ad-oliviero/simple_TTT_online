#!/bin/bash
cd src
make all

if [ $? != 0 ]
then
	notify-send -t 1000 "Error compiling"
fi