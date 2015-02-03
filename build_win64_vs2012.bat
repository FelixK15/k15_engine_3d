@echo off

mkdir build
cd build
cmake ../. -G "Visual Studio 11 Win64"

cd build
msbuild k15_engine_3d.sln /property:Configuration=Debug /property:Platform=x64 /m:4 /nologo
msbuild k15_engine_3d.sln /property:Configuration=Release /property:Platform=x64 /m:4 /nologo