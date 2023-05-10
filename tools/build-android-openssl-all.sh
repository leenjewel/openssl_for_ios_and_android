export ANDROID_NDK_ROOT=/home/arakawa/Android/android-ndk-r22b
#export PATH=$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
export api=21
./build-android-openssl.sh x86_64
./build-android-openssl.sh x86
./build-android-openssl.sh arm
./build-android-openssl.sh arm64