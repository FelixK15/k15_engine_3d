#include "K15_Prerequisites.h"
#include "K15_Logging.h"
#include "K15_String.h"

#ifdef K15_OS_WINDOWS
	#include "windows.h"
	#include "win32/K15_HelperWin32.h"
#endif //K15_OS_WINDOWS

typedef bool8 (*K15_UnitTestFnc)(void);

int indent = 0;
int globalTestResult = 0;

#define K15_TEST_RESULT								_TESTVAR_

#define K15_PRINT_INDENT(s, p, ...)					{char buffer[512];int offset=0;for(int i = 0; i < indent;++i,++offset){sprintf(buffer + offset, "   ");} sprintf(buffer + offset , s, ##__VA_ARGS__); K15_LogWrite(buffer, p, 0);}
#define K15_START_TESTS(s)							bool8 _TESTVAR_ = TRUE; {const char* _TESTSUITE_ = s; K15_PRINT_INDENT("TEST SUITE '%s'", K15_LOG_PRIORITY_NORMAL, s); ++indent;
#define K15_END_TESTS								--indent; if (_TESTVAR_ == FALSE){K15_PRINT_INDENT("TEST SUITE '%s' FAILED", K15_LOG_PRIORITY_ERROR, _TESTSUITE_);globalTestResult = -1;}else{K15_PRINT_INDENT("TEST SUITE '%s SUCCEEDED", K15_LOG_PRIORITY_SUCCESS, _TESTSUITE_);}}

#define K15_TEST_FAILED(f)							K15_PRINT_INDENT("Test '%s' - FAILED.", K15_LOG_PRIORITY_ERROR, f); _TESTVAR_ = FALSE
#define K15_TEST_SUCCEEDED(f)						K15_PRINT_INDENT("Test '%s' - SUCCEEDED.", K15_LOG_PRIORITY_SUCCESS, f)

#define K15_TEST_BOOL_FUNCTION(f)					{bool8 fR = f(); if (fR != K15_TRUE){K15_TEST_FAILED(#f); K15_PRINT_INDENT("Test %s returned false.", K15_LOG_PRIORITY_ERROR, #f);}else{K15_TEST_SUCCEEDED(#f);}}
#define K15_TEST_EQUAL_FUNCTION(f, r)				{int32 fR = f(); if (fR != r){K15_TEST_FAILED(#f); K15_PRINT_INDENT("Test '%s' - Expected '%d' and got '%d'.", K15_LOG_PRIORITY_ERROR, #f, r, fR);}else{K15_TEST_SUCCEEDED(#f);}}
#define K15_TEST_FLOAT_FUNCTION_APPROX(f, r, a)		{real32 fR = f(); if (!(fR > (r - a) && fR < (r + a))){K15_TEST_FAILED(#f); K15_PRINT_INDENT("Test '%s' - Expected '%f' and got '%f'.", K15_LOG_PRIORITY_ERROR, #f, r, fR);}else{K15_TEST_SUCCEEDED(#f);}}

#include "K15_MathTest.cpp"
#include "K15_OSLayerTest.cpp"
#include "K15_RuntimeTest.cpp"
#include "K15_ContainerTest.cpp"
#include "K15_MemoryTest.cpp"

#ifdef K15_OS_WINDOWS
/*********************************************************************************/
void K15_Win32LogConsole(const char* p_Message, LogPriority p_Priority)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD colorCode = 0;

	switch (p_Priority)
	{
	case K15_LOG_PRIORITY_NORMAL:
		{
			colorCode = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
			break;
		}

	case K15_LOG_PRIORITY_ERROR:
		{
			colorCode = FOREGROUND_RED;
			break;
		}

	case K15_LOG_PRIORITY_WARNING:
		{
			colorCode = FOREGROUND_GREEN | FOREGROUND_RED;
			break;
		}

	case K15_LOG_PRIORITY_SUCCESS:
		{
			colorCode = FOREGROUND_GREEN;
			break;
		}

	case K15_LOG_PRIORITY_DEBUG:
		{
			colorCode = FOREGROUND_RED | FOREGROUND_BLUE;
			break;
		}
	}

	uint32 messageLength= (uint32)strlen(p_Message) + 1; //+1 for 0 terminator
	wchar_t* messageBuffer = (wchar_t*)alloca(messageLength * sizeof(wchar_t));

	K15_Win32ConvertStringToWString(p_Message, messageLength, messageBuffer);

	//set output color
	SetConsoleTextAttribute(consoleHandle,colorCode);
	printf("%ls\n", messageBuffer);

	//set color back to white
	SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN); 
} 
/*********************************************************************************/
#endif //K15_OS_WINDOWS

/*********************************************************************************/
void K15_InternalDoMathTests()
{
	K15_START_TESTS("Math Test")
		K15_TEST_EQUAL_FUNCTION(K15_Vector2Test, TRUE);
		K15_TEST_EQUAL_FUNCTION(K15_Vector3Test, TRUE);
	K15_END_TESTS
}
/*********************************************************************************/
void K15_InternalDoOSLayerTests()
{
	K15_START_TESTS("OSLayer Test")
		K15_TEST_EQUAL_FUNCTION(K15_FileSystemTest, TRUE);
	K15_END_TESTS
}
/*********************************************************************************/
void K15_InternalDoMemoryTests()
{
	K15_START_TESTS("Memory Test")
		K15_TEST_EQUAL_FUNCTION(K15_MemoryTest, TRUE);
	K15_END_TESTS
}
/*********************************************************************************/


/*********************************************************************************/
int main(int argc, char** argv)
{
#ifdef K15_OS_WINDOWS
	K15_LogRegisterLogFnc(K15_Win32LogConsole, K15_LOG_PRIORITY_ALL, K15_LOG_FLAG_NONE);
#endif //K15_OS_WINDOWS

	K15_InternalDoMathTests();
	K15_InternalDoOSLayerTests();
	K15_InternalDoMemoryTests();

	return globalTestResult;
}
/*********************************************************************************/