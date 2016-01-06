@echo off

mkdir ..\..\..\build\win32_x86_vs2015
cd ..\..\..\build\win32_x86_vs2015

cmake ../../. -G "Visual Studio 14" -DK15_PLATFORM=Win32 -DK15_RENDERER=OpenGL -DK15_RENDERER_VERSION=3 -DK15_BUILD_DYNAMIC_GAME_LIBRARY=0 -Wno-dev

cd ../../scripts/build_scripts/visual_studio