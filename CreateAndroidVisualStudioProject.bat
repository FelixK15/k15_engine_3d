@echo off

mkdir build\android
cd build\android

cmake ../../. -G "Visual Studio 11" -Wno-dev -DCMAKE_SYSTEM_NAME=Android -DCMAKE_VS_PLATFORM_TOOLSET=7 -DCMAKE_ANDROID_GUI=True -DK15_PLATFORM=Android -DCMAKE_ANDROID_API=14 -DCMAKE_ANDROID_API_MIN=14

cd ../..
