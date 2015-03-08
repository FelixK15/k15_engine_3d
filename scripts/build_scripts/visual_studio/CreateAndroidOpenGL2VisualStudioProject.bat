@echo off

mkdir ..\..\..\build\android
cd ..\..\..\build\android

cmake ../../. -G "Visual Studio 11" -Wno-dev -DK15_RENDERER=OpenGL -DK15_RENDERER_VERSION=2 -DCMAKE_SYSTEM_NAME=Android -DCMAKE_VS_PLATFORM_TOOLSET=7 -DCMAKE_ANDROID_GUI=True -DK15_PLATFORM=Android -DCMAKE_ANDROID_API=14 -DCMAKE_ANDROID_API_MIN=14

cd ../../scripts/build_scripts/visual_studio
