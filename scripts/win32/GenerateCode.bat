@echo off

if exist "../../build/win32_x64" set BIN_DIR=win32_x64
if exist "../../build/win32_x32" set BIN_DIR=win32_x32

if exist "../../build/%BIN_DIR%/bin/Debug" set CONF_DIR=Debug
if exist "../../build/%BIN_DIR%/bin/Release" set CONF_DIR=Release

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/runtime/include/generated/K15_ConfigValueBuffer.h -t TYPE=K15_ConfigValue -t TYPE_NAME=ConfigValue -t TYPE_STRUCT_NAME=K15_ConfigValue

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/runtime/src/generated/K15_ConfigValueBuffer.cpp -t TYPE=K15_ConfigValue -t TYPE_NAME=ConfigValue -t INPUT_FILE=generated/K15_ConfigValueBuffer.h -t FREE_FNC=free -t MALLOC_FNC=malloc

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/oslayer/src/generated/K15_ThreadStretchBuffer.cpp -t TYPE=K15_Thread* -t TYPE_NAME=Thread -t INPUT_FILE=generated/K15_ThreadStretchBuffer.h -t FREE_FNC=K15_OS_FREE -t MALLOC_FNC=K15_OS_MALLOC

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/oslayer/include/generated/K15_ThreadStretchBuffer.h -t TYPE=K15_Thread* -t TYPE_NAME=Thread -t TYPE_STRUCT_NAME=K15_Thread

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/oslayer/include/generated/K15_DynamicLibraryStretchBuffer.h -t TYPE=K15_DynamicLibrary* -t TYPE_NAME=DynamicLibrary -t TYPE_STRUCT_NAME=K15_DynamicLibrary

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/oslayer/src/generated/K15_DynamicLibraryStretchBuffer.cpp -t TYPE=K15_DynamicLibrary* -t TYPE_NAME=DynamicLibrary -t INPUT_FILE=generated/K15_DynamicLibraryStretchBuffer.h -t FREE_FNC=K15_OS_FREE -t MALLOC_FNC=K15_OS_MALLOC

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/oslayer/src/generated/K15_FileWatchEntryStretchBuffer.cpp -t TYPE=K15_FileWatchEntry -t TYPE_NAME=FileWatchEntry -t INPUT_FILE=generated/K15_FileWatchEntryStretchBuffer.h -t FREE_FNC=K15_OS_FREE -t MALLOC_FNC=K15_OS_MALLOC

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/oslayer/include/generated/K15_FileWatchEntryStretchBuffer.h -t TYPE=K15_FileWatchEntry -t TYPE_NAME=FileWatchEntry -t TYPE_STRUCT_NAME=K15_FileWatchEntry

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/oslayer/src/generated/K15_DirectoryWatchEntryStretchBuffer.cpp -t TYPE=K15_DirectoryWatchEntry -t TYPE_NAME=DirectoryWatchEntry -t INPUT_FILE=generated/K15_DirectoryWatchEntryStretchBuffer.h -t FREE_FNC=K15_OS_FREE -t MALLOC_FNC=K15_OS_MALLOC

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/oslayer/include/generated/K15_DirectoryWatchEntryStretchBuffer.h -t TYPE=K15_DirectoryWatchEntry -t TYPE_NAME=DirectoryWatchEntry -t TYPE_STRUCT_NAME=K15_DirectoryWatchEntry

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/oslayer/include/generated/K15_DynamicFunctionPointerStretchBuffer.h -t TYPE=K15_DynamicFunctionPointer -t TYPE_NAME=DynamicFunctionPointer -t TYPE_STRUCT_NAME=K15_DynamicFunctionPointer

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/oslayer/src/generated/K15_DynamicFunctionPointerStretchBuffer.cpp -t TYPE=K15_DynamicFunctionPointer -t TYPE_NAME=DynamicFunctionPointer -t INPUT_FILE=generated/K15_DynamicFunctionPointerStretchBuffer.h -t FREE_FNC=K15_OS_FREE -t MALLOC_FNC=K15_OS_MALLOC

