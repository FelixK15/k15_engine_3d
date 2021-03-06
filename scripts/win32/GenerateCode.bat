@echo off

if exist "../../build/win32_x64" set BIN_DIR=win32_x64
if exist "../../build/win32_x32" set BIN_DIR=win32_x32

if exist "../../build/%BIN_DIR%/bin/Debug" set CONF_DIR=Debug
if exist "../../build/%BIN_DIR%/bin/Release" set CONF_DIR=Release

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/runtime/include/generated/K15_ConfigValueBuffer.h -t TYPE=K15_ConfigValue -t TYPE_NAME=ConfigValue -t TYPE_STRUCT_NAME=K15_ConfigValue

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/runtime/src/generated/K15_ConfigValueBuffer.cpp -t TYPE=K15_ConfigValue -t TYPE_NAME=ConfigValue -t INPUT_FILE=generated/K15_ConfigValueBuffer.h

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/oslayer/src/generated/K15_ThreadStretchBuffer.cpp -t TYPE=K15_Thread* -t TYPE_NAME=Thread -t INPUT_FILE=generated/K15_ThreadStretchBuffer.h

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/oslayer/include/generated/K15_ThreadStretchBuffer.h -t TYPE=K15_Thread* -t TYPE_NAME=Thread -t TYPE_STRUCT_NAME=K15_Thread

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_FixedBuffer.cpp.gen -o ../../core/oslayer/src/generated/K15_ThreadFixedBuffer.cpp -t TYPE=K15_Thread* -t TYPE_NAME=Thread -t INPUT_FILE=generated/K15_ThreadFixedBuffer.h

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_FixedBuffer.h.gen -o ../../core/oslayer/include/generated/K15_ThreadFixedBuffer.h -t TYPE=K15_Thread* -t TYPE_NAME=Thread -t TYPE_STRUCT_NAME=K15_Thread

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/oslayer/include/generated/K15_DynamicLibraryStretchBuffer.h -t TYPE=K15_DynamicLibrary* -t TYPE_NAME=DynamicLibrary -t TYPE_STRUCT_NAME=K15_DynamicLibrary

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/oslayer/src/generated/K15_DynamicLibraryStretchBuffer.cpp -t TYPE=K15_DynamicLibrary* -t TYPE_NAME=DynamicLibrary -t INPUT_FILE=generated/K15_DynamicLibraryStretchBuffer.h

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/oslayer/src/generated/K15_FileWatchEntryStretchBuffer.cpp -t TYPE=K15_FileWatchEntry -t TYPE_NAME=FileWatchEntry -t INPUT_FILE=generated/K15_FileWatchEntryStretchBuffer.h

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/oslayer/include/generated/K15_FileWatchEntryStretchBuffer.h -t TYPE=K15_FileWatchEntry -t TYPE_NAME=FileWatchEntry -t TYPE_STRUCT_NAME=K15_FileWatchEntry

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/oslayer/src/generated/K15_DirectoryWatchEntryStretchBuffer.cpp -t TYPE=K15_DirectoryWatchEntry -t TYPE_NAME=DirectoryWatchEntry -t INPUT_FILE=generated/K15_DirectoryWatchEntryStretchBuffer.h

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/oslayer/include/generated/K15_DirectoryWatchEntryStretchBuffer.h -t TYPE=K15_DirectoryWatchEntry -t TYPE_NAME=DirectoryWatchEntry -t TYPE_STRUCT_NAME=K15_DirectoryWatchEntry

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/oslayer/include/generated/K15_DynamicFunctionPointerStretchBuffer.h -t TYPE=K15_DynamicFunctionPointer -t TYPE_NAME=DynamicFunctionPointer -t TYPE_STRUCT_NAME=K15_DynamicFunctionPointer

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/oslayer/src/generated/K15_DynamicFunctionPointerStretchBuffer.cpp -t TYPE=K15_DynamicFunctionPointer -t TYPE_NAME=DynamicFunctionPointer -t INPUT_FILE=generated/K15_DynamicFunctionPointerStretchBuffer.h

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_StretchBuffer.h.gen -o ../../core/runtime/include/generated/K15_AsyncOperationStretchBuffer.h -t TYPE=K15_AsyncOperation* -t TYPE_NAME=AsyncOperation -t TYPE_STRUCT_NAME=K15_AsyncOperation

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen -o ../../core/runtime/src/generated/K15_AsyncOperationStretchBuffer.cpp -t TYPE=K15_AsyncOperation* -t TYPE_NAME=AsyncOperation -t INPUT_FILE=generated/K15_AsyncOperationStretchBuffer.h

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_MemoryPool.cpp.gen -o ../../core/runtime/src/generated/K15_AsyncOperationMemoryPool.cpp -t TYPE=K15_AsyncOperation -t TYPE_NAME=AsyncOperation -t INPUT_FILE=generated/K15_AsyncOperationMemoryPool.h

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_MemoryPool.h.gen -o ../../core/runtime/include/generated/K15_AsyncOperationMemoryPool.h -t TYPE=K15_AsyncOperation -t TYPE_NAME=AsyncOperation -t TYPE_STRUCT_NAME=K15_AsyncOperation

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/src/K15_FixedBuffer.cpp.gen -o ../../core/runtime/src/generated/K15_SubMeshFixedBuffer.cpp -t TYPE=K15_SubMesh -t TYPE_NAME=SubMesh -t INPUT_FILE=generated/K15_SubMeshFixedBuffer.h

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i ../../core/codegen/templates/include/K15_FixedBuffer.h.gen -o ../../core/runtime/include/generated/K15_SubMeshFixedBuffer.h -t TYPE=K15_SubMesh -t TYPE_NAME=SubMesh -t TYPE_STRUCT_NAME=K15_SubMesh

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i "../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen" -o "../../core/guilayer/src/generated/K15_GUIElementStretchBuffer.cpp" -t TYPE=K15_GUIElement* -t TYPE_NAME=GUIElement -t INPUT_FILE="generated/K15_GUIElementStretchBuffer.h"

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i "../../core/codegen/templates/include/K15_StretchBuffer.h.gen" -o "../../core/guilayer/include/generated/K15_GUIElementStretchBuffer.h" -t TYPE=K15_GUIElement* -t TYPE_NAME=GUIElement -t TYPE_STRUCT_NAME=K15_GUIElement

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i "../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen" -o "../../core/runtime/src/generated/K15_ResourceStretchBuffer.cpp" -t TYPE=K15_Resource -t TYPE_NAME=Resource -t INPUT_FILE="generated/K15_ResourceStretchBuffer.h"

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i "../../core/codegen/templates/include/K15_StretchBuffer.h.gen" -o "../../core/runtime/include/generated/K15_ResourceStretchBuffer.h" -t TYPE=K15_Resource -t TYPE_NAME=Resource -t TYPE_STRUCT_NAME=K15_Resource

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i "../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen" -o "../../core/resourcecompiler/src/generated/K15_ResourceDependencyStretchBuffer.cpp" -t TYPE=K15_ResourceDependency -t TYPE_NAME=ResourceDependency -t INPUT_FILE="generated/K15_ResourceDependencyStretchBuffer.h"

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i "../../core/codegen/templates/include/K15_StretchBuffer.h.gen" -o "../../core/resourcecompiler/include/generated/K15_ResourceDependencyStretchBuffer.h" -t TYPE=K15_ResourceDependency -t TYPE_NAME=ResourceDependency -t TYPE_STRUCT_NAME=K15_ResourceDependency

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i "../../core/codegen/templates/src/K15_StretchBuffer.cpp.gen" -o "../../core/rendering/src/generated/K15_RenderMaterialDataDescStretchBuffer.cpp" -t TYPE=K15_RenderMaterialDataDesc -t TYPE_NAME=RenderMaterialDataDesc -t INPUT_FILE="generated/K15_RenderMaterialDataDescStretchBuffer.h"

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i "../../core/codegen/templates/include/K15_StretchBuffer.h.gen" -o "../../core/rendering/include/generated/K15_RenderMaterialDataDescStretchBuffer.h" -t TYPE=K15_RenderMaterialDataDesc -t TYPE_NAME=RenderMaterialDataDesc -t TYPE_STRUCT_NAME=K15_RenderMaterialDataDesc

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i "../../core/codegen/templates/include/K15_FixedBuffer.h.gen" -o "../../core/runtime/include/generated/K15_ResourceLoaderFixedBuffer.h" -t TYPE=K15_ResourceLoader -t TYPE_NAME=ResourceLoader -t TYPE_STRUCT_NAME=K15_ResourceLoader

call "../../build/%BIN_DIR%/bin/%CONF_DIR%/codegen.exe" -i "../../core/codegen/templates/src/K15_FixedBuffer.cpp.gen" -o "../../core/runtime/src/generated/K15_ResourceLoaderFixedBuffer.cpp" -t TYPE=K15_ResourceLoader -t TYPE_NAME=ResourceLoader -t TYPE_STRUCT_NAME=K15_ResourceLoader -t INPUT_FULE="generated/K15/K15_ResourceLoaderFixedBuffer.h"
