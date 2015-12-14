@echo off

if exist "../../build/win32_x64" set BIN_DIR=win32_x64
if exist "../../build/win32_x32" set BIN_DIR=win32_x32

if exist "../../build/%BIN_DIR%/bin/Debug" set CONF_DIR=Debug
if exist "../../build/%BIN_DIR%/bin/Release" set CONF_DIR=Release

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/binary_cpp_generator.exe" -i "../../testgame/data/textures/backup_texture.png.k15c" -o "../../core/runtime/src/generated/K15_BackupTexture.cpp"
call "../../build/%BIN_DIR%/bin/%CONF_DIR%/binary_cpp_generator.exe" -i "../../testgame/data/samplers/backup_sampler.k15c" -o "../../core/runtime/src/generated/K15_BackupSampler.cpp"
call "../../build/%BIN_DIR%/bin/%CONF_DIR%/binary_cpp_generator.exe" -i "../../testgame/data/fonts/backup_font.ttf.k15c" -o "../../core/runtime/src/generated/K15_BackupFont.cpp"
