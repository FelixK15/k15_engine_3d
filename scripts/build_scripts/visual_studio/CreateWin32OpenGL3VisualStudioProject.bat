@echo off

mkdir ..\..\..\build\win32_x86
cd ..\..\..\build\win32_x86

cmake ../../. -G "Visual Studio 11" -DK15_PLATFORM=Win32 -DK15_RENDERER=OpenGL -DK15_RENDERER_VERSION=3 -Wno-dev

cd ../../scripts/build_scripts/visual_studio