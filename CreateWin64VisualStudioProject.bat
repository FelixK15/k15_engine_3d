@echo off

mkdir build\win32_x64_debug
cd build\win32_x64_debug

cmake ../../. -G "Visual Studio 11 Win64" -DCMAKE_BUILD_TYPE=Debug -DK15_PLATFORM=Win32

cd ../..