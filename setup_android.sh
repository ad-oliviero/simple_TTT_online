#!/bin/bash
set -e

export START_DIR=$(pwd)

printf "Installing android sdkmanager...\n"
if [ ! -d android-sdk ]
then
	sleep 1
	mkdir -v android-sdk
	[ ! -f commandlinetools-linux-7583922_latest.zip ] && wget "https://dl.google.com/android/repository/commandlinetools-linux-7583922_latest.zip"
	unzip commandlinetools-linux-7583922_latest.zip
	mv cmdline-tools android-sdk/tools
	printf "\nInstalling sdk...\n"
	sleep 1
	cd android-sdk
	export ANDROID_SDK=$(pwd)
	cd tools/bin
	./sdkmanager --update --sdk_root=$ANDROID_SDK
	./sdkmanager --install "build-tools;30.0.3" --sdk_root=$ANDROID_SDK
	./sdkmanager --install platform-tools --sdk_root=$ANDROID_SDK
	./sdkmanager --install "platforms;android-30" --sdk_root=$ANDROID_SDK
	unset ANDROID_SDK
	cd $START_DIR
else
	cd android-sdk
	export ANDROID_SDK=$(pwd)
	cd $START_DIR
fi

printf "\nInstalling ndk...\n"
if [ ! -d android-ndk ]
then
	sleep 1
	[ ! -f android-ndk-r22b-linux-x86_64.zip ] && wget "https://dl.google.com/android/repository/android-ndk-r22b-linux-x86_64.zip"
	unzip android-ndk-r22b-linux-x86_64.zip
	mv android-ndk-r22b android-ndk
	cd android-ndk
	export ANDROID_NDK=$(pwd)
	cd build/tools
	python make_standalone_toolchain.py --arch arm --api 30 --install-dir=android_toolchain_ARM_API30
	mv android_toolchain_ARM_API30 $START_DIR
	export ANDROID_TOOLCHAIN=$START_DIR/android_toolchain_ARM_API30
	cd $START_DIR
else
	cd android-ndk
	export ANDROID_NDK=$(pwd)
	cd $START_DIR
fi

printf "\nSetup raylib...\n"
sleep 1
cd lib
./setup_raylib.sh
cd ..

printf "\nAll done, now you can build Simple_TTT for android!\n"
