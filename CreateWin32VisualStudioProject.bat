@echo off

mkdir build\win32_x86
cd build\win32_x86

cmake ../../. -G "Visual Studio 11" -DK15_PLATFORM=Win32 -Wno-dev

cd ../..