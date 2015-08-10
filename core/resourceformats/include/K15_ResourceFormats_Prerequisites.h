#ifndef _K15_ResourceFormats_Prerequisites_h_
#define _K15_ResourceFormats_Prerequisites_h_

#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <cmath>
#include <io.h>
//#include <string>
//#include <fstream>

#include "K15_ResourceFormats_Config.h"

#include "enums\K15_ReturnValues.h"

//Format specifier
#define K15_RESOURCE_IDENTIFIER "K15_RES"
#define K15_IDENTIFIER_LENGTH 7

//format types
#define K15_RESOURCE_FORMAT_MESH 1
#define K15_RESOURCE_FORMAT_MATERIAL 2
#define K15_RESOURCE_FORMAT_TEXTURE 3
#define K15_RESOURCE_FORMAT_FONT 4
#define K15_RESOURCE_FORMAT_SAMPLER 5

//versioning
#define K15_FORMAT_MESH_MAJOR_VERSION 1
#define K15_FORMAT_MESH_MINOR_VERSION 1

//Save Flags
#define K15_SAVE_FLAG_FREE_DATA 0x01

//typedef std::string String;

typedef unsigned char byte;
typedef unsigned char bool8;

#ifndef K15_FALSE
	#define K15_FALSE 0
#endif //K15_FALSE

#ifndef K15_TRUE
	#define K15_TRUE 1
#endif //K15_TRUE

#ifndef intern
	#define intern static
#endif //intern1

#ifdef _WIN32
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

struct K15_DataAccessContext;

#endif //_K15_ResourceFormats_Prerequisites_h_