set LPATH = %CD%
echo %LPATH%
cd %LPATH%\EngineTest
del %CD%\libs\armeabi\*.so  /f 

::#build c++ code
ndk-build NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk NDK_MODULE_PATH=%LPATH%;%LPATH%\Dependencies

::#build apk
ant debug

::#uninstall old apk
adb uninstall com.k15games.EngineTest

::#install new apk
adb install %CD%\bin\EngineTest-debug.apk