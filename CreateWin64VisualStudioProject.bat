@echo off

mkdir build\win32_x64
cd build\win32_x64

cmake ../../. -G "Visual Studio 11 Win64" -DK15_PLATFORM=Win32 -Wno-dev

cd ../..