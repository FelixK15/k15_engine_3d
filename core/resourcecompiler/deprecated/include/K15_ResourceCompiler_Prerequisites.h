#ifndef _K15_ResourceCompiler_Prequisites_h_
#define _K15_ResourceCompiler_Prequisites_h_

#include <vector>
#include <string>
#include <set>
#include <array>
#include <map>
#include <fstream>
#include <time.h>
#include <algorithm>
#include <assert.h>

#include <cstdarg>
#include <conio.h>

#define DEBUG_FUNCTION

#define VERSION_MAJOR "1"
#define VERSION_MINOR "0"

typedef std::string String;
typedef std::ifstream InputFileStream;
typedef std::ofstream OutputFileStream;
typedef std::set<String> StringSet;

typedef unsigned char byte;

#ifdef _WIN32
	#pragma warning(disable: 4996) //posix deprecated

	//8 bit types
	typedef signed    __int8  int8;
	typedef unsigned  __int8  uint8;
	//16 bit types
	typedef signed    __int16 int16;
	typedef unsigned  __int16 uint16;
	//32 bit types
	typedef signed    __int32 int32;
	typedef unsigned  __int32 uint32;
	//64 bit types
	typedef signed    __int64 int64;
	typedef unsigned  __int64 uint64;
#else 
	//8 bit types
	typedef signed		char		int8;
	typedef unsigned	char		uint8;
	//16 bit types
	typedef signed		short		int16;
	typedef unsigned	short		uint16;
	//32 bit types
	typedef signed		int			int32;
	typedef unsigned	int			uint32;
	//64 bit types
	typedef signed		long long	int64;
	typedef unsigned	long long	uint64;
#endif //_WIN32


#define DynamicArray(T) std::vector<T>
#define FixedArray(T, C) std::array<T, C>
#define HashMap(K, V) std::map<K, V>
#define HashPair(K, V) std::pair<K, V>

#define K15_NO_DEFAULT_CONSTRUCTOR(T)  protected: T() {};
#define K15_NON_COPYABLE(T) protected: T(const T&) {}; T& operator=(const T&){return *this;};
#define K15_NON_MOVEABLE(T) protected: T(T&&) {}; T& operator=(T&&){return *this;};

#ifdef _WIN32
namespace K15_ResourceCompiler
{
	class Win32_FileFinder;
	typedef Win32_FileFinder FileFinder;
}
	
#endif //_WIN32

#endif //_K15_ResourceCompiler_Prequisites_h_