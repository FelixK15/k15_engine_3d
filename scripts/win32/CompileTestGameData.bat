@echo off

if exist "../../build/win32_x64_vs2015" set BIN_DIR=win32_x64_vs2015
if exist "../../build/win32_x32_vs2015" set BIN_DIR=win32_x32_vs2015
if exist "../../build/win32_x64_vs2012" set BIN_DIR=win32_x64_vs2012
if exist "../../build/win32_x32_vs2012" set BIN_DIR=win32_x32_vs2012


if exist "../../build/%BIN_DIR%/bin/Debug" set CONF_DIR=Debug
if exist "../../build/%BIN_DIR%/bin/Release" set CONF_DIR=Release

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/resourcecompiler.exe" -u -r -i "../../testgame/data/"
