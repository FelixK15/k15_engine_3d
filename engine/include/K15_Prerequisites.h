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

#ifdef __ANDROID__
#	define K15_OS_ANDROID
#elif defined _WIN32
#	define K15_OS_WINDOWS
#elif defined __linux__
#	define K15_OS_LINUX
#elif defined __APPLE__
#	define K15_OS_MACOS
#endif //_WIN32

#if defined (K15_OS_WINDOWS)
	#ifdef _WIN64
	#	define K15_64_BIT
	#endif //_WIN64
#else
#	include <limits.h>
#	ifdef __LP64__
#		define K15_64_BIT
#	endif //__LP64__
#endif //K15_OS_WINDOWS

#if defined (K15_64_BIT)
#	define K15_PTR_SIZE 64
#	define K15_PTR_HEX_MASK 0xFFFFFFFFFFFFFFFF
#else
#	define K15_PTR_SIZE 32
#	define K15_PTR_HEX_MASK 0xFFFFFFFF
#endif //K15_64_BIT

//edit export defines
#if defined K15_CODE_GENERATOR
#	define expose		__attribute__((annotate("expose")))
#	define expose_read  __attribute__((annotate("expose_readonly")));
#else
#	define expose 
#	define expose_read 
#endif //K15_CODE_GENERATOR

namespace K15_Engine
{
	/*********************************************************************************/
	namespace Core
	{
		class HashedString;
		class Application;
		class ApplicationOSLayer;
		class ApplicationModule;
		class TaskManager;
		class TaskBase;
		class GameTime;
#		if defined K15_OS_ANDROID
			class DynamicLibrary_Linux;
			class ApplicationOSLayer_Android;
			class RenderWindow_Android;
#		elif defined K15_OS_WINDOWS
			class DynamicLibrary_Win32;
			class ApplicationOSLayer_Win32;
			class RenderWindow_Win32;
#		endif //K15_OS_WINDOWS
		class DynamicLibraryBase;
		class DynamicLibraryManager;
		class EventManager;
		class EventListener;
		class GameEvent;
		class InputManager;
		class InputBindingBase;
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
		class EventTask;
		class Node;
		class Image;
		class ProfilingNode;
		class ProfilingManager;
		class GameObject;
		class GameObjectComponentBase;
		class EventManager;
		class RenderWindowBase;
		class MemoryProfiler;
		class MemoryProfilingTask;
		class MemoryPools;
		class FontManager;
		class Font;
		class TrueTypeFont;
		class StackAllocator;

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
		struct Resolution;
	} // end of K15_Engine::Core namespace
	/*********************************************************************************/
	namespace Rendering
	{
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
		class AABB;
		class VertexDeclaration;
		class VertexDeclarationImplBase;
		class VertexBuffer;
		class IndexBuffer;
		class RenderQueue;
		class AlphaState;
		class DepthState;
		class VertexBuffer;
		class Vertex;
		struct RenderOperation;
		struct VertexElement;
		struct TextureCreationOptions;
		struct ColorRGBA;

		// surface description flags
		const unsigned int DDSF_CAPS           = 0x00000001l;
		const unsigned int DDSF_HEIGHT         = 0x00000002l;
		const unsigned int DDSF_WIDTH          = 0x00000004l;
		const unsigned int DDSF_PITCH          = 0x00000008l;
		const unsigned int DDSF_PIXELFORMAT    = 0x00001000l;
		const unsigned int DDSF_MIPMAPCOUNT    = 0x00020000l;
		const unsigned int DDSF_LINEARSIZE     = 0x00080000l;
		const unsigned int DDSF_DEPTH          = 0x00800000l;

		// pixel format flags
		const unsigned int DDSF_ALPHAPIXELS    = 0x00000001l;
		const unsigned int DDSF_FOURCC         = 0x00000004l;
		const unsigned int DDSF_RGB            = 0x00000040l;
		const unsigned int DDSF_RGBA           = 0x00000041l;

		// dwCaps1 flags
		const unsigned int DDSF_COMPLEX         = 0x00000008l;
		const unsigned int DDSF_TEXTURE         = 0x00001000l;
		const unsigned int DDSF_MIPMAP          = 0x00400000l;

		// dwCaps2 flags
		const unsigned int DDSF_CUBEMAP         = 0x00000200l;
		const unsigned int DDSF_CUBEMAP_POSITIVEX  = 0x00000400l;
		const unsigned int DDSF_CUBEMAP_NEGATIVEX  = 0x00000800l;
		const unsigned int DDSF_CUBEMAP_POSITIVEY  = 0x00001000l;
		const unsigned int DDSF_CUBEMAP_NEGATIVEY  = 0x00002000l;
		const unsigned int DDSF_CUBEMAP_POSITIVEZ  = 0x00004000l;
		const unsigned int DDSF_CUBEMAP_NEGATIVEZ  = 0x00008000l;
		const unsigned int DDSF_CUBEMAP_ALL_FACES  = 0x0000FC00l;
		const unsigned int DDSF_VOLUME          = 0x00200000l;

		// compressed texture types
		const unsigned int FOURCC_DXT1 = 0x31545844l; //(MAKEFOURCC('D','X','T','1'))
		const unsigned int FOURCC_DXT3 = 0x33545844l; //(MAKEFOURCC('D','X','T','3'))
		const unsigned int FOURCC_DXT5 = 0x35545844l; //(MAKEFOURCC('D','X','T','5'))

		struct DXTColBlock
		{
			unsigned short col0;
			unsigned short col1;

			unsigned char row[4];
		};

		struct DXT3AlphaBlock
		{
			unsigned short row[4];
		};

		struct DXT5AlphaBlock
		{
			unsigned char alpha0;
			unsigned char alpha1;

			unsigned char row[6];
		};

		struct DDS_PIXELFORMAT
		{
			unsigned int dwSize;
			unsigned int dwFlags;
			unsigned int dwFourCC;
			unsigned int dwRGBBitCount;
			unsigned int dwRBitMask;
			unsigned int dwGBitMask;
			unsigned int dwBBitMask;
			unsigned int dwABitMask;
		};

		struct DDS_HEADER
		{
			unsigned int dwSize;
			unsigned int dwFlags;
			unsigned int dwHeight;
			unsigned int dwWidth;
			unsigned int dwPitchOrLinearSize;
			unsigned int dwDepth;
			unsigned int dwMipMapCount;
			unsigned int dwReserved1[11];
			DDS_PIXELFORMAT ddspf;
			unsigned int dwCaps1;
			unsigned int dwCaps2;
			unsigned int dwReserved2[3];
		};
	} //end of K15_Engine::Rendering namespace
// 	/*********************************************************************************/
// 	namespace Math
// 	{
// 		class Vector2;
// 		class Vector3;
// 		class Vector4;
// 		class Matrix3;
// 		class Matrix4;
// 		class Quaternion;
// 	} //end of K15_Engine::Math namespace
// 	/*********************************************************************************/
 }

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
	/*using namespace Math;*/
}// end of K15_Engine namespace

#ifdef __GNUC__
//http://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
# define K15_GCC_VERSION (__GNUC__ * 10000 \
                          + __GNUC_MINOR__ * 100 \
                          + __GNUC_PATCHLEVEL__)
#endif //__GNUC__

#if defined _DEBUG || defined DEBUG
#	define K15_DEBUG
#endif //_DEBUG

#if __cplusplus > 199711L || _MSC_VER >= 1700 || K15_GCC_VERSION > 40800
#	define K15_CPP11_SUPPORT
#endif //__cplusplus > 199711L || _MSC_VER >= 1700 || K15_GCC_VERSION > 40800

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

//c std libs
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
#include <malloc.h>

//Container
#if defined K15_DONT_USE_STL
#	include "K15_DynamicArray.h"
#	include "K15_HashMap.h"
#	include "K15_List.h"
#	include "K15_NTree.h"
#	include "K15_Stack.h"
#	include "K15_FixedArray.h"

#	define DynamicArray(T)	K15_Engine::Container::DynamicArray<T>
#	define HashMap(K,V)		K15_Engine::Container::HashMap<K,V>
#	define List(T)			K15_Engine::Container::List<T>
#	define Stack(T)			K15_Engine::Container::Stack<T>
//#	define Set(T)			K15_Engine::Container::Set<T>
#	define FixedArray(T,C)  K15_Engine::Container::FixedArray<T,C>
#else
#	include <list>
#	include <map>
#	include <stack>
#	include <vector>
#	include <set>
#	include <array>
#	include <algorithm>
#	include <sys\stat.h>
#	include <cerrno>

#	define DynamicArray(T)	std::vector<T>
#	define HashMap(K,V)		std::map<K,V>
#	define List(T)			std::list<T>
#	define Stack(T)			std::stack<T>
#	define Deque(T)			std::deque<T>
#	define UniqueSet(T)		std::set<T>
#	define Pair(K,V)		std::pair<K,V>
#	define FixedArray(T,C)	std::array<T,C>
	
#endif //K15_DONT_USE_STL

 //Strings
#if defined K15_DONT_USE_STL
#	include "K15_String.h"
	typedef K15_Engine::Core::String String;
#else
#	include <string>
#	include <algorithm>
	typedef std::string String;
#endif //K15_DONT_USE_STL

//Threading
#if defined K15_DONT_USE_STL || (!defined K15_DONT_USE_STL && !defined K15_CPP11_SUPPORT)
#   include "..\..\dependencies\TinyThread\include\tinythread.h"
#   define g_CurrentThread tthread::this_thread
    typedef tthread::thread Thread;
    typedef tthread::mutex Mutex;
    typedef tthread::lock_guard<Mutex> LockGuard;

#else
#	  include <thread>		
#   include <mutex>
#   include <chrono>
#   define g_CurrentThread std::this_thread
	typedef std::thread Thread;
    typedef std::mutex Mutex;
    typedef std::lock_guard<Mutex> LockGuard;
#endif //K15_DONT_USE_STL || (!defined K15_DONT_USE_STL && !defined K15_CPP11_SUPPORT)

//Streams
#if defined K15_DONT_USE_STL
#	include "K15_FileStream.h"
#	include "K15_StringStream.h"
	typedef K15_Engine::Core::FileStream FileStream;
	typedef K15_Engine::Core::StringStream StringStream;
#else
#	include <fstream>
#	include <sstream>
	typedef std::fstream FileStream;
	typedef std::ofstream WriteFileStream;
	typedef std::ifstream ReadFileStream;
	typedef std::stringstream StringStream;
#endif //K15_DONT_USE_STL

#if defined K15_OS_WINDOWS
#	if defined K15_BUILD
#		define K15_CORE_API __declspec(dllexport)
#	else
#		define K15_CORE_API __declspec(dllimport)
#	endif //K15_BUILD
#else
#	define K15_CORE_API
#endif //K15_OS_WINDOWS
  
#if defined K15_OS_WINDOWS
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#endif //K15_OS_WINDOWS

#if defined K15_OS_WINDOWS
#	define K15_DEBUG_MESSAGEBOX(msg,title) MessageBox(0,msg,title,MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_TASKMODAL)
#endif //K15_OS_WINDOWS

#if defined K15_OS_WINDOWS
#	define K15_ID_ABORT IDABORT
#	define K15_ID_RETRY IDRETRY
#	define K15_ID_IGNORE IDIGNORE
#endif //K15_OS_WINDOWS

#if defined K15_OS_WINDOWS
#	define K15_BREAK_APPLICATION() __debugbreak()
#	define K15_TERMINATE_APPLICATION() abort()
#endif //K15_OS_WINDOWS

#if defined K15_OS_WINDOWS
#	include "windows.h"
#elif defined K15_OS_ANDROID

	/*#	include <android_native_app_glue.h>
#	include <android\sensor.h>
#	include <android\input.h>
#	include <EGL\egl.h>
#	include <GLES2\gl2.h>*/

	struct android_app;
	struct ANativeWindow;
	struct ASensorEventQueue;
	struct ASensor;
	struct ASensorManager;
#	include "android_native_app_glue.h"
#	include <android\sensor.h>
#	include <android\input.h>
#	include <android\log.h>
#	include <dlfcn.h>
#	include <jni.h>
#	include <unistd.h>
#endif //K15_OS_WINDOWS

//include glm math
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\quaternion.hpp"

typedef glm::fmat3 Matrix3;
typedef glm::fmat4 Matrix4;
typedef glm::fvec2 Vector2;
typedef glm::fvec3 Vector3;
typedef glm::fvec4 Vector4;
typedef glm::fquat Quaternion;

#if defined K15_OS_WINDOWS
	typedef K15_Engine::Core::DynamicLibrary_Win32 DynamicLibraryType;
	typedef K15_Engine::Core::ApplicationOSLayer_Win32 ApplicationOSLayerType;
	typedef K15_Engine::Core::RenderWindow_Win32 RenderWindowType;
#elif defined K15_OS_ANDROID
	typedef K15_Engine::Core::DynamicLibrary_Linux DynamicLibraryType;
	typedef K15_Engine::Core::ApplicationOSLayer_Android ApplicationOSLayerType;
	typedef K15_Engine::Core::RenderWindow_Android RenderWindowType;
#endif //K15_OS_WINDOWS
 
#if defined K15_DEBUG
#	define K15_NEW	  new(__FILE__,__LINE__,__FUNCTION__) 
#	define K15_DELETE delete

#	define K15_NEW_T2(objType) new(Allocators[Category]->allocateDebug(sizeof(objType),__FILE__,__LINE__,false,__FUNCTION__))
#	define K15_NEW_T(allocator,objType) new(allocator->allocateDebug(sizeof(objType),__FILE__,__LINE__,false,__FUNCTION__))
#	define K15_NEW_SIZE(allocator,size) new(allocator->allocateDebug(size,__FILE__,__LINE__,false,__FUNCTION__))

#	define K15_DELETE_SIZE(allocator,ptr,size) if(ptr){allocator->deallocateDebug((void*)ptr,size,__FILE__,__LINE__,false,__FUNCTION__);}
#	define K15_DELETE_T(allocator,ptr,type) if(ptr){((type*)ptr)->~type();K15_DELETE_SIZE(allocator,ptr,sizeof(type));}
#else
#	define K15_NEW    new
#	define K15_DELETE delete

#	define K15_NEW_SIZE(allocator,size) new(allocator->allocate(size))
#	define K15_NEW_T(allocator,objType) new(allocator->allocate(sizeof(objType)))
#	define K15_NEW_T2(objType) new(Allocators[Category]->allocate(sizeof(objType)))

#	define K15_DELETE_SIZE(allocator,ptr,size) if(ptr){allocator->deallocate((void*)ptr,size);}
#endif //K15_DEBUG


#define K15_MALLOC(size) ApplicationOSLayerType::os_malloc(size)
#define K15_FREE(ptr) ApplicationOSLayerType::os_free(ptr)
#define K15_MEMCPY(destination,source,size) memcpy(destination,source,size)

# define K15_PLACEMENT_NEW new(ptr)

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

typedef unsigned char byte;

#ifdef K15_OS_WINDOWS

typedef signed    __int8  int8;
typedef unsigned  __int8  uint8;

typedef signed    __int16 int16;
typedef unsigned  __int16 uint16;

typedef signed    __int32 int32;
typedef unsigned  __int32 uint32;

typedef signed    __int64 int64;
typedef unsigned  __int64 uint64;

#else 

typedef signed		char		int8;
typedef unsigned	char		uint8;

typedef signed		short		int16;
typedef unsigned	short		uint16;

typedef signed		int			int32;
typedef unsigned	int			uint32;

typedef signed		long long	int64;
typedef unsigned	long long	uint64;

#endif //K15_OS_WINDOWS

typedef unsigned int Enum;

typedef K15_Engine::Core::HashedString ObjectName;
typedef K15_Engine::Core::HashedString TypeName;
typedef K15_Engine::Core::HashedString EventName;
typedef K15_Engine::Core::HashedString ProfilingName;
typedef K15_Engine::Core::HashedString ResourceName;

#if defined K15_NO_STRINGS
#	define _ON(x)  K15_Engine::Core::ObjectNames::ObjectNames::x
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

#define K15_SMART_POINTER(type) typedef K15_Engine::Core::Memory::Pointer<type> typePtr

#define K15_INVALID_RESOURCE_ID -1

// #if defined _MSC_VER || K15_GCC_VERSION
// #	define K15_USE_PRECOMPILED_HEADERS
// #endif //_MSC_VER || K15_GCC_VERSION

#define GIGABYTE	1073741824
#define MEGABYTE	1048576
#define KILOBYTE	1024

#define size_gigabyte(s) (uint32)(s*GIGABYTE)
#define size_megabyte(s) (uint32)(s*MEGABYTE)
#define size_kilobyte(s) (uint32)(s*KILOBYTE)

#define K15_PTR(T) typedef Pointer<T> T##Ptr;

#define K15_NON_COPYABLE(T) protected: T(const T&){}; T& operator=(const T&){return *this;};

#define K15_EXECUTE_CRITICAL_FUNCTION(obj,func) {obj.lockMutex();obj.func;obj.unlockMutex();}
#define K15_EXECUTE_CRITICAL_FUNCTION_PTR(obj,func) {obj->lockMutex();obj->func;obj->unlockMutex();}

#if defined (K15_CPP11_SUPPORT)
#	define OVERRIDE override
#else
#	define OVERRIDE
#endif //K15_CPP11_SUPPORT

#if defined (K15_NO_INLINE)
#	define INLINE
#else
#	if defined (K15_FORCE_INLINE)
#		define INLINE __forceinline
#	else
#		define INLINE inline
#	endif //K15_FORCE_INLINE
#endif //K15_NO_INLINE

#endif //_K15Engine_Prerequisites_h_