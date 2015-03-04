@echo off

mkdir build\win32_x86_debug
cd build\win32_x86_debug

cmake ../../. -G "Visual Studio 11" -DCMAKE_BUILD_TYPE=Debug -DK15_PLATFORM=Win32

cd ../..