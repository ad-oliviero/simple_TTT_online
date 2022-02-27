#!/bin/bash
set -e

export START_DIR=$(pwd)
export SDK_REL_NUM=28

printf "Installing android sdkmanager...\n"
if [ ! -d android-sdk ]
then
	sleep 1
	mkdir -v android-sdk
	[ ! -f commandlinetools-linux-8092744_latest.zip ] && wget "https://dl.google.com/android/repository/commandlinetools-linux-8092744_latest.zip"
	unzip commandlinetools-linux-8092744_latest.zip
	mv cmdline-tools android-sdk/tools
	printf "\nInstalling sdk...\n"
	sleep 1
	cd android-sdk
	export ANDROID_SDK=$(pwd)
	cd tools/bin
	./sdkmanager --update --sdk_root=$ANDROID_SDK
	./sdkmanager --install "build-tools;$SDK_REL_NUM.0.0" --sdk_root=$ANDROID_SDK
	./sdkmanager --install platform-tools --sdk_root=$ANDROID_SDK
	./sdkmanager --install "platforms;android-$SDK_REL_NUM" --sdk_root=$ANDROID_SDK
	unset ANDROID_SDK
	cd $START_DIR
fi

printf "\nInstalling ndk...\n"
if [ ! -d android-ndk ]
then
	sleep 1
	[ ! -f android-ndk-r23b-linux.zip ] && wget "https://dl.google.com/android/repository/android-ndk-r23b-linux.zip"
	unzip android-ndk-r23b-linux.zip
	mv android-ndk-r23b android-ndk
	cp dx android-sdk/build-tools/$SDK_REL_NUM.0.0/dx
	cp dx.jar android-sdk/build-tools/$SDK_REL_NUM.0.0/lib/dx.jar
	cd $START_DIR
fi

printf "\nSetup raylib...\n"
sleep 1
cd lib
./setup_raylib.sh
cd ..

printf "\nAll done, now you can build Simple_TTT for android!\n"
