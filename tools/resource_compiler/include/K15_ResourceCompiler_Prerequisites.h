#ifndef _K15_ResourceCompiler_Prequisites_h_
#define _K15_ResourceCompiler_Prequisites_h_

#include <vector>
#include <string>
#include <set>
#include <array>
#include <map>
#include <fstream>
#include <time.h>

#include <cstdarg>
#include <conio.h>

#define DEBUG_FUNCTION

#define VERSION_MAJOR "1"
#define VERSION_MINOR "0"

typedef std::string String;
typedef std::ifstream InputFileStream;
typedef std::ofstream OutputFileStream;
typedef std::set<String> StringSet;

typedef int int32;
typedef unsigned int uint32;
typedef short int16;
typedef unsigned short uint16;
typedef char int8;
typedef unsigned char uint8;
typedef unsigned char byte;

#define DynamicArray(T) std::vector<T>
#define FixedArray(T, C) std::array<T, C>
#define HashMap(K, V) std::map<K, V>
#define HashPair(K, V) std::pair<K, V>

#define K15_NO_DEFAULT_CONSTRUCTOR(T)  private: T() {};
#define K15_NON_COPYABLE(T) private: T(const T&) {}; T& operator=(const T&){return *this;};
#define K15_NON_MOVEABLE(T) private: T(T&&) {}; T& operator=(T&&){return *this;};

#endif //_K15_ResourceCompiler_Prequisites_h_