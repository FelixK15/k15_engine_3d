set(CORE_SRC "./core/src/K15_AABB.cpp"
"./core/src/K15_AllocatedObject.cpp"
"./core/src/K15_Application.cpp"
"./core/src/K15_ApplicationModule.cpp"
"./core/src/K15_BaseAllocator.cpp"
"./core/src/K15_BlockAllocator.cpp"
"./core/src/K15_ColorRGBA.cpp"
"./core/src/K15_DynamicLibraryBase.cpp"
"./core/src/K15_DynamicLibraryManager.cpp"
"./core/src/K15_EventHandler.cpp"
"./core/src/K15_EventManager.cpp"
"./core/src/K15_EventTask.cpp"
"./core/src/K15_Font.cpp"
"./core/src/K15_Frustum.cpp"
"./core/src/K15_GameEvent.cpp"
"./core/src/K15_GameObject.cpp"
"./core/src/K15_GameObjectComponentBase.cpp"
"./core/src/K15_GameStateBase.cpp"
"./core/src/K15_GameTime.cpp"
"./core/src/K15_HashedString.cpp"
"./core/src/K15_Image.cpp"
"./core/src/K15_IniFileParser.cpp"
"./core/src/K15_InputManager.cpp"
"./core/src/K15_IOUtil.cpp"
"./core/src/K15_JobBase.cpp"
"./core/src/K15_Keyboard.cpp"
"./core/src/K15_Language.cpp"
"./core/src/K15_LogBase.cpp"
"./core/src/K15_LogManager.cpp"
"./core/src/K15_MatrixUtil.cpp"
"./core/src/K15_MemoryBlock.cpp"
"./core/src/K15_MemoryProfilingTask.cpp"
"./core/src/K15_Mouse.cpp"
"./core/src/K15_NetworkManager.cpp"
"./core/src/K15_Node.cpp"
"./core/src/K15_Object.cpp"
"./core/src/K15_PhysicsTask.cpp"
"./core/src/K15_PrecompiledHeader.cpp"
"./core/src/K15_ProfilingManager.cpp"
"./core/src/K15_ProfilingNode.cpp"
"./core/src/K15_RawData.cpp"
"./core/src/K15_RenderWindow.cpp"
"./core/src/K15_Rtti.cpp"
"./core/src/K15_StackAllocator.cpp"
"./core/src/K15_StringUtil.cpp"
"./core/src/K15_SortUtil.cpp"
"./core/src/K15_TaskBase.cpp"
"./core/src/K15_TaskManager.cpp"
"./core/src/K15_ThreadSafe.cpp"
"./core/src/K15_ThreadWorker.cpp")

set(CORE_WIN32_SRC "./core/src/win32/K15_DynamicLibrary_Win32.cpp"
"./core/src/win32/K15_Keyboard_Win32.cpp"
"./core/src/win32/K15_Mouse_Win32.cpp"
"./core/src/win32/K15_OSLayer_Win32.cpp"
"./core/src/win32/K15_RenderWindow_Win32.cpp"
"./core/src/win32/K15_TextConsoleLog_Win32.cpp"
"./core/src/win32/K15_VisualStudioLog_Win32.cpp")

set(CORE_ANDROID_SRC "./core/src/android/K15_Keyboard_Android.cpp"
"./core/src/android/K15_Logcat_Android.cpp"
"./core/src/android/K15_Mouse_Android.cpp"
"./core/src/android/K15_OSLayer_Android.cpp"
"./core/src/android/K15_RenderWindow_Android.cpp")

set(CORE_LINUX_SRC "./core/src/linux/K15_DynamicLibrary_Linux.cpp"
"./core/src/linux/K15_RenderWindow_Linux.cpp"
"./core/src/linux/K15_OSLayer_Linux.cpp"
"./core/src/linux/K15_TextConsoleLog_Linux.cpp"
"./core/src/linux/K15_Mouse_Linux.cpp"
"./core/src/linux/K15_Keyboard_Linux.cpp")

set(CORE_INC "./core/include/K15_AABB.h"
"./core/include/K15_AABB.inl"
"./core/include/K15_AllocatedObject.h"
"./core/include/K15_AllocatedObject.inl"
"./core/include/K15_Application.h"
"./core/include/K15_Application.inl"
"./core/include/K15_ApplicationModule.h"
"./core/include/K15_ApplicationModuleDescription.h"
"./core/include/K15_ApplicationParameter.h"
"./core/include/K15_BaseAllocator.h"
"./core/include/K15_BaseAllocator.inl"
"./core/include/K15_BlockAllocator.h"
"./core/include/K15_ColorRGBA.h"
"./core/include/K15_ColorRGBA.inl"
"./core/include/K15_DynamicLibraryBase.h"
"./core/include/K15_DynamicLibraryBase.inl"
"./core/include/K15_DynamicLibraryManager.h"
"./core/include/K15_EnumStrings.h"
"./core/include/K15_EventHandler.h"
"./core/include/K15_EventHandler.inl"
"./core/include/K15_EventManager.h"
"./core/include/K15_EventManager.inl"
"./core/include/K15_EventTask.h"
"./core/include/K15_Font.h"
"./core/include/K15_Font.inl"
"./core/include/K15_FrameStatistic.h"
"./core/include/K15_Frustum.h"
"./core/include/K15_Frustum.inl"
"./core/include/K15_Functor.h"
"./core/include/K15_Functor.inl"
"./core/include/K15_GameEvent.h"
"./core/include/K15_GameEvent.inl"
"./core/include/K15_GameObject.h"
"./core/include/K15_GameObject.inl"
"./core/include/K15_GameObjectComponentBase.h"
"./core/include/K15_GameObjectComponentBase.inl"
"./core/include/K15_GameStateBase.h"
"./core/include/K15_GameTime.h"
"./core/include/K15_GameTime.inl"
"./core/include/K15_HashedString.h"
"./core/include/K15_HashedString.inl"
"./core/include/K15_Image.h"
"./core/include/K15_Image.inl"
"./core/include/K15_IniFileParser.h"
"./core/include/K15_IniFileParser.inl"
"./core/include/K15_InputManager.h"
"./core/include/K15_InputTriggerBase.h"
"./core/include/K15_IOUtil.h"
"./core/include/K15_JobBase.h"
"./core/include/K15_JobBase.inl"
"./core/include/K15_Keyboard.h"
"./core/include/K15_Language.h"
"./core/include/K15_Language.inl"
"./core/include/K15_LogBase.h"
"./core/include/K15_LogManager.h"
"./core/include/K15_LogManager.inl"
"./core/include/K15_MatrixUtil.h"
"./core/include/K15_MemoryBlock.h"
"./core/include/K15_MemoryHeader.h"
"./core/include/K15_MemoryProfilingTask.h"
"./core/include/K15_Mouse.h"
"./core/include/K15_NetworkManager.h"
"./core/include/K15_Node.h"
"./core/include/K15_Node.inl"
"./core/include/K15_Object.h"
"./core/include/K15_Object.inl"
"./core/include/K15_PhysicsProcessBase.h"
"./core/include/K15_PhysicsTask.h"
"./core/include/K15_PoolAllocator.h"
"./core/include/K15_PoolAllocator.inl"
"./core/include/K15_PrecompiledHeader.h"
"./core/include/K15_Prerequisites.h"
"./core/include/K15_ProfilingManager.h"
"./core/include/K15_ProfilingManager.inl"
"./core/include/K15_ProfilingNode.h"
"./core/include/K15_RawData.h"
"./core/include/K15_RenderWindow.h"
"./core/include/K15_RenderWindow.inl"
"./core/include/K15_Rtti.h"
"./core/include/K15_Rtti.inl"
"./core/include/K15_Singleton.h"
"./core/include/K15_StackAllocator.h"
"./core/include/K15_StringUtil.h"
"./core/include/K15_StringUtil.inl"
"./core/include/K15_SortUtil.h"
"./core/include/K15_TaskBase.h"
"./core/include/K15_TaskBase.inl"
"./core/include/K15_TaskManager.h"
"./core/include/K15_TaskManager.inl"
"./core/include/K15_ThreadSafe.h"
"./core/include/K15_ThreadSafe.inl"
"./core/include/K15_ThreadWorker.h")

set(CORE_WIN32_INC "./core/include/win32/K15_DynamicLibrary_Win32.h"
"./core/include/win32/K15_OSLayer_Win32.h"
"./core/include/win32/K15_RenderWindow_Win32.h"
"./core/include/win32/K15_TextConsoleLog_Win32.h"
"./core/include/win32/K15_VisualStudioLog_Win32.h")

set(CORE_ANDROID_INC "./core/include/android/K15_Logcat_Android.h"
"./core/include/android/K15_OSLayer_Android.h"
"./core/include/android/K15_OSLayer_Android.inl"
"./core/include/android/K15_RenderWindow_Android.h"
"./core/include/android/K15_RenderWindow_Android.inl")

set(CORE_LINUX_INC "./core/include/linux/K15_DynamicLibrary_Linux.h"
"./core/include/linux/K15_OSLayer_Linux.h"
"./core/include/linux/K15_TextConsoleLog_Linux.h"
"./core/include/linux/K15_RenderWindow_Linux.h")	
