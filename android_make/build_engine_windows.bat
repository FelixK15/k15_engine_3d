@echo off
set LPATH=%~dp0
echo %LPATH%EngineTest
cd %LPATH%EngineTest

::delete old *.so files
if exist %LPATH%\EngineTest\libs\ del %CD%\libs\armeabi\*.so  /f 

::#build c++ code
call ndk-build NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk NDK_MODULE_PATH=%LPATH%;%LPATH%\Dependencies

if %ERRORLEVEL% NEQ 0 (
	cd ..
	echo =============
	echo Build failed
	echo =============
) else (
	::#build apk
	call ant debug

	::#uninstall old apk
	call adb uninstall com.k15games.EngineTest

	::#install new apk
	call adb install %CD%\bin\EngineTest-debug.apk

	cd ..
)
