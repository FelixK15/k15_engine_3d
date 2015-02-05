/**
 * @file K15_Prerequisites.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Prerequisites_h_
#define _K15Engine_Prerequisites_h_

#include <K15_OSLayer_Prerequisites.h>
#include <K15_Logging.h>

//edit export defines
#if defined K15_CODE_GENERATOR
#	define expose		__attribute__((annotate("expose")))
#	define expose_enum(x) __attribute__((annotate("expose")))
#	define expose_read  __attribute__((annotate("expose_readonly")));
#else
#	define expose 
#	define expose_read 
#	define expose_enum(x)
#endif //K15_CODE_GENERATOR

namespace K15_Engine
{
	/*********************************************************************************/
	namespace Core
	{
		class HashedString;
		class Application;
		class ApplicationModule;
		class TaskManager;
		class RenderWindow;
		class TaskBase;
		class GameTime;
		class EventManager;
		class EventListener;
		class GameEvent;
		class InputManager;
		class InputEvent;
		class LogManager;
		class LogBase;
		class NetworkManager;
		class MemoryProfiler;
		class JobBase;
		class ThreadWorker;
		class MemoryProfilingTask;
		class ResourceManager;
		class PhysicsProcessBase;
		class RenderProcessBase;
		class RenderTask;
		class InputManager;
		class InputTriggerBase;
		class PhysicsTask;
		class ResourceBase;
		class ResourceArchiveBase;
		class ResourceImporterBase;
		class ResourceImporterBitmap;
		class SceneGraph;
		class EventTask;
		class NodeComponent;
		class ModelComponent;
		class Image;
		class ProfilingNode;
		class ProfilingManager;
		class GameObject;
		class GameObjectComponentBase;
		class LightComponent;
		class GameStateBase;
		class GameStateManager;
		class EventManager;
		class MemoryProfiler;
		class MemoryProfilingTask;
		class MemoryPools;
		class FontManager;
		class Font;
		class TrueTypeFont;
		class StackAllocator;
		class Frustum;
		class Model;

		template<class T>
		class Singleton;

		template<class ReturnType>
		class Functor0;

		template<unsigned __int16,unsigned __int32>
		class PageAllocator;

		template<unsigned int Category>
		class AllocatedObject;

		struct RawData;
		struct MemoryHeader;
		struct MemoryBlock;
		struct ApplicationModuleDescription;
        struct MouseEventArguments;
        struct KeyboardEventArguments;
		struct Resolution;
		struct EventHandlerArrayEntry;
	} // end of K15_Engine::Core namespace
	/*********************************************************************************/
	namespace Rendering
	{
		class DebugRenderer;
		class MeshManager;
		class MaterialManager;
		class MaterialData;
		class RenderTarget;
		class CameraComponent;
		class RendererBase;
		class GpuProgram;
		class GpuProgramImplBase;
		class GpuProgramBatch;
		class GpuProgramBatchImplBase;
		class GpuProgramParameter;
		class GpuBuffer;
		class GpuBufferImplBase;
		class Texture;
		class TextureImplBase;
		class TextureSampler;
		class TextureSamplerImplBase;
		class Material;
		class SubMesh;
		class Mesh;
        class MeshInstance;
        class SubMeshInstance;
        class IndexData;
		class AABB;
		class VertexDeclaration;
		class VertexDeclarationImplBase;
		class VertexBuffer;
		class IndexBuffer;
		class RenderQueue;
		class AlphaState;
		class DepthState;
		class VertexBuffer;
        class VertexData;
		class Vertex;
		class RendererBase;
		struct RenderOperation;
		struct VertexElement;
		struct TextureCreationOptions;
		struct ColorRGBA;
	} //end of K15_Engine::Rendering namespace
	/*********************************************************************************/
	namespace Math
	{
		class Vector2;
		class Vector3;
		class Vector4;
		class Matrix3;
		class Matrix4;
		class Quaternion;
        class MatrixUtil;
        class MathUtil;
	} //end of K15_Engine::Math namespace
	/*********************************************************************************/
 }

 //jsoncpp forward declaration
 /*********************************************************************************/
 namespace Json 
 {
	 class FastWriter;
	 class StyledWriter;
	 class Reader;
	 class Features;
	 typedef unsigned int ArrayIndex;
	 class StaticString;
	 class Path;
	 class PathArgument;
	 class Value;
	 class ValueIteratorBase;
	 class ValueIterator;
	 class ValueConstIterator;
 }
 /*********************************************************************************/

namespace K15_Engine
{
	using namespace Core;
	using namespace Rendering;
	using namespace Math;
}// end of K15_Engine namespace


#define K15_VERSION_MAJOR 1
#define K15_VERSION_MINOR 0

#define K15_ENGINE_VERSION (K15_VERSION_MAJOR * 10 + K15_VERSION_MINOR)

#if defined _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS //don't throw a "unsafe function" warning when using printf, sprintf, etc. 
#	pragma warning(disable : 4251) //dll export for std::string, std::list etc.
#	pragma warning(disable : 4530) //c++ exception used, but not enabled (xlocale.h)
#	pragma warning(disable : 6255) //unprotected use of alloca
#	pragma warning(disable : 4291) //no matching operator delete found
#	pragma warning(disable : 6330) //'char' passes as _Param_(1) when 'unsigned char' is required in call to 'isdigit'
#endif //_MSC_VER

#if defined __GNUC__
#	pragma GCC diagnostic ignored "-Wwrite-strings"
#	pragma GCC diagnostic ignored "-Wformat-security"
#endif

//Threading
#if !defined K15_CPP11_SUPPORT
	#pragma message "The compiler does NOT support C++11 features..."
	#include "..\..\dependencies\TinyThread\include\tinythread.h"
	#define g_CurrentThread tthread::this_thread
    typedef tthread::thread Thread;
    typedef tthread::mutex Mutex;
    typedef tthread::lock_guard<Mutex> LockGuard;
#else
	#pragma message("The compiler does support C++11 features...")
	#include <thread>		
	#include <mutex>
	#include <chrono>
	#define g_CurrentThread std::this_thread
	typedef std::thread Thread;
    typedef std::mutex Mutex;
    typedef std::lock_guard<Mutex> LockGuard;
#endif //K15_DONT_USE_STL || (!defined K15_DONT_USE_STL && !defined K15_CPP11_SUPPORT)

#define NOMINMAX

//std libs
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <list>
#include <map>
#include <stack>
#include <vector>
#include <set>
#include <array>
#include <memory>
#include <climits>
#include <algorithm>
#include <sys/stat.h>
#include <cstdarg>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DynamicArray(T)		std::vector<T>
#define HashMap(K,V)		std::map<K,V>
#define List(T)				std::list<T>
#define Stack(T)			std::stack<T>
#define Deque(T)			std::deque<T>
#define UniqueSet(T)		std::set<T>
#define Pair(K,V)			std::pair<K,V>
#define FixedArray(T,C)		std::array<T,C>
#define Sort(S,E,F)			std::sort(S,E,F)
#define Find(S,E,O)			std::find(S,E,O)
typedef std::string			String;
typedef std::fstream		FileStream;
typedef std::ofstream		WriteFileStream;
typedef std::ifstream		ReadFileStream;
typedef std::stringstream	StringStream;

#if defined K15_OS_WINDOWS
	
	#if defined K15_BUILD
		#define K15_CORE_API __declspec(dllexport)
	#else
		#define K15_CORE_API __declspec(dllimport)
	#endif //K15_BUILD

	#define K15_DEBUG_MESSAGEBOX(msg,title) MessageBox(0,msg,title,MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_TASKMODAL)
	#define K15_ID_ABORT IDABORT
	#define K15_ID_RETRY IDRETRY
	#define K15_ID_IGNORE IDIGNORE
	#define K15_BREAK_APPLICATION() __debugbreak()
	#define K15_TERMINATE_APPLICATION() abort()

#else
	#define K15_CORE_API
#endif //K15_OS_WINDOWS
  
#if defined K15_OS_LINUX
    #pragma GCC diagnostic ignored "-fpermissive"
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <dlfcn.h>
    #include <unistd.h>
    struct _XDisplay;
    typedef _XDisplay Display;
    typedef unsigned long GLXWindow;
    typedef unsigned long Window;
#endif //K15_OS_LINUX
	

#if defined K15_OS_ANDROID
	#pragma message("Compiling for android")
	struct android_app;
	struct ANativeWindow;
	struct ASensorEventQueue;
	struct ASensor;
	struct ASensorManager;
	#include "android_native_app_glue.h"
	#include <android\sensor.h>
	#include <android\input.h>
	#include <android\log.h>
	#include <android\asset_manager.h>
	#include <android\asset_manager_jni.h>
	#include <dlfcn.h>
	#include <jni.h>
	#include <unistd.h>
#endif //K15_OS_ANDROID


#ifndef K15_OS_WINDOWS
	//TEXT macro from windows
	#define TEXT(x) x
#endif //K15_OS_WINDOWS
 
#if defined K15_DEBUG
	#pragma message("Using debug version")
	#define K15_NEW	  new(__FILE__,__LINE__,__FUNCTION__) 
	#define K15_DELETE delete

	#define K15_NEW_T2(objType) new(Allocators[Category]->allocateDebug(sizeof(objType),__FILE__,__LINE__,false,__FUNCTION__))
	#define K15_NEW_T(allocator,objType) new(allocator->allocateDebug(sizeof(objType),__FILE__,__LINE__,false,__FUNCTION__))
	#define K15_NEW_SIZE(allocator,size) new(allocator->allocateDebug(size,__FILE__,__LINE__,false,__FUNCTION__))

	#define K15_DELETE_SIZE(allocator,ptr,size) if(ptr){allocator->deallocateDebug((void*)ptr,size,__FILE__,__LINE__,false,__FUNCTION__);}
	#define K15_DELETE_T(allocator,ptr,type) if(ptr){((type*)ptr)->~type();K15_DELETE_SIZE(allocator,ptr,sizeof(type));}
#else
	#pragma message("Using release version")
	#define K15_NEW    new
	#define K15_DELETE delete

	#define K15_NEW_SIZE(allocator,size) new(allocator->allocate(size))
	#define K15_NEW_T(allocator,objType) new(allocator->allocate(sizeof(objType)))
	#define K15_NEW_T2(objType) new(Allocators[Category]->allocate(sizeof(objType)))

	#define K15_DELETE_SIZE(allocator,ptr,size) if(ptr){allocator->deallocate((void*)ptr,size);}
#endif //K15_DEBUG


#define K15_MALLOC(size)	OSLayer::os_malloc(size)
#define K15_FREE(ptr)		OSLayer::os_free(ptr)
#define K15_MEMCPY(destination,source,size) memcpy(destination,source,size)

#if defined K15_DEBUG
	#if defined K15_OS_WINDOWS
		#define K15_ASSERT(condition,message)	\
			  if(!(condition)){ \
			  __analysis_assume(condition); \
				  String debugMessage__ = message; \
				  debugMessage__ += "\n\n"; \
				  debugMessage__ += "The expression \""; \
				  debugMessage__ += #condition; \
				  debugMessage__ += "\" failed.\n"; \
				  debugMessage__ += "\"abort\" will terminate the application, \"retry\" will break the application for debugging and"; \
				  debugMessage__ += "\"ignore\" will ignore the failed condition and continue processing the application (application may be in an unstable state)."; \
				  int returnValue__ = K15_DEBUG_MESSAGEBOX(debugMessage__.c_str(),"Assertion"); \
				  if(returnValue__ == K15_ID_ABORT){ \
					  K15_TERMINATE_APPLICATION(); \
				  }else if(returnValue__ == K15_ID_RETRY){ \
					  K15_BREAK_APPLICATION(); \
				  } \
			  } 

	#elif defined K15_OS_ANDROID
		#define K15_ASSERT(condition,message) \
			if(!(condition)){ \
				String debugMessage__ = message; \
				debugMessage__ += "\n\n"; \
				debugMessage__ += "The expression \""; \
				debugMessage__ += #condition; \
				debugMessage__ += "\" failed. File:\""; \
				debugMessage__ += __BASE_FILE__; \
				debugMessage__ += "\" ("; \
				debugMessage__ += __LINE__; \
				debugMessage__ += ") Function:\""; \
				debugMessage__ += __FUNCTION__; \
				debugMessage__ += "\"."; \
				__android_log_assert(#condition,"K15_Engine",debugMessage__.c_str()); \
			}
    #elif defined K15_OS_LINUX
        #define K15_ASSERT(condition, message) \
            if(!(condition)){ \
                String __dbgmsg = message; \
                printf("The expression \"%s\" failed.\nFile:%s(%d)\nFunction:%s\nMessage:%s", \
                    #condition, __BASE_FILE__, __LINE__, __FUNCTION__, __dbgmsg.c_str()); \
                exit(-1); \
            }
	#endif //K15_OS_WINDOWS
#else
	#define K15_ASSERT(condition,message0)
#endif //K15_DEBUG

#ifdef K15_DEBUG
#	define K15_DEBUG_ONLY(code) {code}
#else
#	define K15_DEBUG_ONLY(code)
#endif

typedef std::set<String> StringSet;

#define g_DebugRenderer K15_Engine::Rendering::DebugRenderer::getInstance()
#define g_FontManager K15_Engine::Core::FontManager::getInstance()
#define g_InputManager K15_Engine::Core::InputManager::getInstance()
#define g_Application K15_Engine::Core::Application::getInstance()
#define g_EventManager K15_Engine::Core::EventManager::getInstance()
#define g_TaskManager K15_Engine::Core::TaskManager::getInstance()
#define g_DynamicLibraryManager K15_Engine::Core::DynamicLibraryManager::getInstance()
#define g_LogManager K15_Engine::Core::LogManager::getInstance()
#define g_ProfileManager K15_Engine::Core::ProfilingManager::getInstance()
#define g_ThreadWorker K15_Engine::Core::ThreadWorker::getInstance()
#define g_MaterialManager K15_Engine::Rendering::MaterialManager::getInstance()
#define g_ResourceManager K15_Engine::Core::ResourceManager::getInstance()
#define g_MeshManager K15_Engine::Rendering::MeshManager::getInstance()
#define g_GameStateManager K15_Engine::Core::GameStateManager::getInstance()

typedef unsigned int Enum;

typedef K15_Engine::Core::HashedString ObjectName;
typedef K15_Engine::Core::HashedString TypeName;
typedef K15_Engine::Core::HashedString EventName;
typedef K15_Engine::Core::HashedString ProfilingName;
typedef K15_Engine::Core::HashedString ResourceName;

#if defined K15_NO_STRINGS
#	define _ON(x) K15_Engine::Core::ObjectNames::ObjectNames::x
#	define _TN(x) K15_Engine::Core::ObjectNames::TypeNames::x
#	define _EN(x) K15_Engine::Core::ObjectNames::EventNames::x
#	define _RN(x) K15_Engine::Core::ObjectNames::ResourceNames::x
#else
#	define _ON(x)  ObjectName(#x)
#	define _TN(x) TypeName(#x)
#	define _EN(x) EventName(#x)
#	define _RN(x) ResourceName(#x)
#endif // K15_NO_STRINGS

#define K15_SAFE_DELETE(ptr) if(ptr){ delete ptr; ptr = 0; }

#define K15_INVALID_RESOURCE_ID -1

#define K15_EPSILON 1e-5

#define K15_PX_TO_NDC(px_pos, dimension_max) ((((float)px_pos / (float)dimension_max) - 0.5f) * 2.f)

#define K15_PTR(T)		 typedef std::shared_ptr<T> T##Ptr;
#define K15_WEAKPTR(T)	 typedef std::weak_ptr<T>	T##WeakPtr;
#define K15_UNIQUEPTR(T) typedef std::unique_ptr<T> T##UniquePtr;

#define K15_NON_COPYABLE(X) \
	protected: \
	X(const X& rhs){} \
	X& operator=(const X& rhs){return *this;} \
	X(X&& rhs){} \
	X& operator=(X&& rhs){return *this;}

#if defined (K15_NO_INLINE)
	#define INLINE
#else
	#if defined (K15_FORCE_INLINE)
		#define INLINE __forceinline
	#else
		#define INLINE inline
	#endif //K15_FORCE_INLINE
#endif //K15_NO_INLINE

#ifdef K15_USE_MEMORY_MANAGEMENT
	#pragma message("K15 Engine Memory Management is used.")
#else
	#pragma message("K15 Engine Memory Management is NOT used.")

	#undef K15_NEW
	#undef K15_DELETE
	#undef K15_NEW_T2
	#undef K15_NEW_T
	#undef K15_NEW_SIZE
	#undef K15_DELETE_SIZE
	#undef K15_DELETE_T

	#define K15_NEW		::new
	#define K15_DELETE	::delete

	#define K15_NEW_T2(objType)				::new(malloc(sizeof(objType)))
	#define K15_NEW_T(allocator,objType)	K15_NEW_T2(objType)
	#define K15_NEW_SIZE(allocator,size)	::new(malloc(size))

	#define K15_DELETE_SIZE(allocator,ptr,size) ::delete ptr
	#define K15_DELETE_T(allocator,ptr,type)	::delete ptr
#endif //K15_USE_MEMORY_MANAGEMENT


#endif //_K15Engine_Prerequisites_h_
