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

namespace K15_Engine
{
	namespace System
	{
		class Application;
		class ApplicationModule;
		struct ApplicationModuleDescription;
		class TaskManager;
		class Task;
		class GameTime;
#		if defined _WIN32
			class DynamicLibrary_Win32;
#		endif //_WIN32
		class DynamicLibraryBase;
		class DynamicLibraryManager;
		class EventManager;
		class EventListener;
		class GameEvent;
		class LogManager;
		class LogBase;
		class ResourceManager;
		class ResourceBase;
		class ResourceFileBase;
		class ResourceData;
		class ProfilingNode;
		class ProfilingManager;
		class EventManager;
		class RenderWindowBase;

		template<class ReturnType>
		class Functor0;

		struct MemoryHeader;
		class StackAllocator;

		template<unsigned __int16,unsigned __int32>
		class PageAllocator;

		template<class Allocator>
		class AllocatedObject;

	}
}

#ifdef __GNUC__
//http://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
# define K15_GCC_VERSION (__GNUC__ * 10000 \
                          + __GNUC_MINOR__ * 100 \
                          + __GNUC_PATCHLEVEL__)
#endif //__GNUC__

#ifdef _WIN32
# define K15_OS_WINDOWS
#endif //_WIN32

#ifdef K15_OS_WINDOWS
# ifdef _DEBUG
#   define K15_DEBUG
# endif //_DEBUG
#endif //K15_OS_WINDOWS

#if __cplusplus > 199711L || _MSC_VER >= 1700 || K15_GCC_VERSION > 40800
#	define K15_CPP11_SUPPORT
#endif

#include "K15_HashedString.h"

//c std libs
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <malloc.h>

//Container
#if defined K15_DONT_USE_STL
#	include "K15_DynamicArray.h"
#	include "K15_HashMap.h"
#	include "K15_List.h"
#	include "K15_NTree.h"
#	include "K15_Stack.h"
#	include "K15_FixedArray.h"

#	define K15_DynamicArray(T)	K15_Engine::Container::DynamicArray<T>
#	define K15_HashMap(K,V)		K15_Engine::Container::HashMap<K,V>
#	define K15_List(T)			K15_Engine::Container::List<T>
#	define K15_Stack(T)			K15_Engine::Container::Stack<T>
#	define K15_Set(T)			K15_Engine::Container::Set<T>
#	define K15_FixedArray(T,C)  K15_Engine::Container::FixedArray<T,C>
#else
#	include <list>
#	include <map>
#	include <stack>
#	include <vector>
#	include <set>

#	define K15_DynamicArray(T)	std::vector<T>
#	define K15_HashMap(K,V)		std::map<K,V>
#	define K15_List(T)			std::list<T>
#	define K15_Stack(T)			std::stack<T>
#	define K15_Set(T)			std::set<T>
#	define K15_Pair(K,V)		std::pair<K,V>

#	if defined K15_CPP11_SUPPORT
#		include <array>
#		define K15_FixedArray(T,C)	std::array<T,C>
#	else
#		define K15_FixedArray(T,C)  K15_Engine::Container::FixedArray<T,C>
#	endif //K15_CPP11_SUPPORT
#endif //K15_DONT_USE_STL

 //Strings
#if defined K15_DONT_USE_STL
#	include "K15_String.h"
	typedef K15_Engine::System::String String;
#else
#	include <string>
	typedef std::string String;
#endif //K15_DONT_USE_STL

//Threading
#if defined K15_DONT_USE_STL
#	define K15_Thread(func,args) K15_Engine::System::Thread<func,args>
#else
#	if defined K15_CPP11_SUPPORT
#		include <thread>		
		typedef std::thread Thread;
#	else
#		include "tinythread.h"
		typedef tinythread::thread Thread;
#	endif //K15_CPP11_SUPPORT
#endif //K15_DONT_USE_STL

//Streams
#if defined K15_DONT_USE_STL
#	include "K15_FileStream.h"
#	include "k15_StringStream.h"
	typedef K15_Engine::System::FileStream FileStream;
	typedef K15_Engine::System::StringStream StringStream;
#else
#	include <fstream>
#	include <sstream>
	typedef std::fstream FileStream;
	typedef std::stringstream StringStream;
#endif //K15_DONT_USE_STL

#if defined K15_OS_WINDOWS
#	if defined K15_BUILD
#		define K15_API_EXPORT __declspec(dllexport)
#	else
#		define K15_API_EXPORT __declspec(dllimport)
#	endif //K15_BUILD
#endif //K15_OS_WINDOWS

#if defined K15_OS_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#endif //K15_OS_WINDOWS

#if defined K15_OS_WINDOWS
#	define K15_DEBUG_MESSAGEBOX(msg,title) MessageBox(0,msg,title,MB_ABORTRETRYIGNORE | MB_ICONERROR)
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
#endif //K15_OS_WINDOWS

#if defined K15_OS_WINDOWS
	typedef K15_Engine::System::DynamicLibrary_Win32 DynamicLibraryType;
#endif //K15_OS_WINDOWS
 
#if defined K15_DEBUG
#	define K15_NEW	  new(__FILE__,__LINE__,__FUNCTION__) 
#	define K15_DELETE delete

#	define K15_NEW_T(objType) (objType*)objType::MemoryAllocator->allocateDebug(sizeof(objType),__FILE__,__LINE__,false,__FUNCTION__)
//#	define K15_NEW(objType,size) objType::MemoryAllocator->allocateDebug(size,__FILE__,__LINE__,false,__FUNCTION__)

#	define K15_DELETE_T(ptr) if(ptr){ptr->MemoryAllocator->deallocateDebug(ptr,__FILE__,__LINE__,false,__FUNCTION__);}
#else
#	define K15_NEW    new
#	define K15_DELETE delete

#	define K15_NEW_T(objType) objType::MemoryAllocator->allocate(sizeof(objType));
#endif //K15_DEBUG

# define K15_PLACEMENT_NEW new(ptr)

#define K15_ASSERT(condition,message)	\
	if(!(condition)){ \
		String debugMessage__ = "\""; \
		debugMessage__ += message; \
		debugMessage__ += "\"\n\n"; \
		debugMessage__ += "\""; \
		debugMessage__ += #condition; \
		debugMessage__ += "\" failed.\n"; \
		debugMessage__ += "\"abort\" will terminate the application, \"retry\" will break the application for debugging \n"; \
		debugMessage__ += "\"ignore\" will ignore the failed condition and continue processing the application."; \
		int returnValue__ = K15_DEBUG_MESSAGEBOX(debugMessage__.c_str(),"Assertion"); \
		if(returnValue__ == K15_ID_ABORT){ \
			K15_TERMINATE_APPLICATION(); \
		}else if(returnValue__ == K15_ID_RETRY){ \
			K15_BREAK_APPLICATION(); \
		} \
	} \

  
typedef K15_Engine::System::AllocatedObject<K15_Engine::System::Application> ApplicationAllocatedObject;
typedef K15_Engine::System::AllocatedObject<K15_Engine::System::EventManager> EventManagerAllocatedObject;
typedef K15_Engine::System::AllocatedObject<K15_Engine::System::TaskManager> TaskManagerAllocatedObject;
typedef K15_Engine::System::AllocatedObject<K15_Engine::System::DynamicLibraryManager> DynamicLibraryManagerAllocatedObject;

#define ApplicationAllocator K15_Engine::System::Application::getInstance()
#define EventManagerAllocator K15_Engine::System::EventManager::getInstance()
#define TaskManagerAllocator K15_Engine::System::TaskManager::getInstance()
#define DynamicLibraryManagerAllocator K15_Engine::System::DynamicLibraryManager::getInstance()

typedef signed char byte;

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

typedef K15_Engine::System::HashedString TypeName;
typedef K15_Engine::System::HashedString EventName;
typedef K15_Engine::System::HashedString ProfilingName;
typedef K15_Engine::System::HashedString ResourceName;

#if defined K15_NO_STRINGS
#	define _TN(x) K15_Engine::System::ObjectNames::TypeNames::x
#	define _EN(x) K15_Engine::System::ObjectNames::EventNames::x
#	define _RN(x) K15_Engine::System::ObjectNames::ResourceNames::x
#else
#	define _TN(x) TypeName(#x)
#	define _EN(x) EventName(#x)
#	define _RN(x) ResourceName(#x)
#endif // K15_NO_STRINGS

#define K15_SAFE_DELETE(ptr) if(ptr){ delete ptr; ptr = 0; }

#define K15_SMART_POINTER(type) typedef K15_Engine::System::Memory::Pointer<type> typePtr

#define K15_INVALID_RESOURCE_ID -1

#define K15_ERROR_BUFFER_SIZE 1024
#define K15_PLUGIN_INFO_BUFFER_SIZE 1024

#define GIGABYTE	1073741824
#define MEGABYTE	1048576
#define KILOBYTE	1024

#endif //_K15Engine_Prerequisites_h_