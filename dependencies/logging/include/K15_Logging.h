#ifndef _K15_Logging_h_
#define _K15_Logging_h_

#define _CRT_SECURE_NO_WARNINGS

#define K15_LOG_NORMAL_MESSAGE(msg, ...) K15_LogWrite(msg, K15_LOG_PRIORITY_NORMAL, ##__VA_ARGS__)
#define K15_LOG_WARNING_MESSAGE(msg, ...) K15_LogWrite(msg, K15_LOG_PRIORITY_WARNING, ##__VA_ARGS__)
#define K15_LOG_ERROR_MESSAGE(msg, ...) K15_LogWrite(msg, K15_LOG_PRIORITY_ERROR, ##__VA_ARGS__)
#define K15_LOG_DEBUG_MESSAGE(msg, ...) K15_LogWrite(msg, K15_LOG_PRIORITY_DEBUG, ##__VA_ARGS__)
#define K15_LOG_SUCCESS_MESSAGE(msg, ...) K15_LogWrite(msg, K15_LOG_PRIORITY_SUCCESS, ##__VA_ARGS__)


#define K15_LOG_CUSTOM_1_MESSAGE(msg, ...) K15_LogWrite(msg, K15_LOG_PRIORITY_CUSTOM_1, ##__VA_ARGS__)
#define K15_LOG_CUSTOM_2_MESSAGE(msg, ...) K15_LogWrite(msg, K15_LOG_PRIORITY_CUSTOM_2, ##__VA_ARGS__)
#define K15_LOG_CUSTOM_3_MESSAGE(msg, ...) K15_LogWrite(msg, K15_LOG_PRIORITY_CUSTOM_3, ##__VA_ARGS__)
#define K15_LOG_CUSTOM_4_MESSAGE(msg, ...) K15_LogWrite(msg, K15_LOG_PRIORITY_CUSTOM_4, ##__VA_ARGS__)

enum LogPriority
{
	K15_LOG_PRIORITY_NORMAL = 0x001,
	K15_LOG_PRIORITY_WARNING = 0x002,
	K15_LOG_PRIORITY_ERROR = 0x004,
	K15_LOG_PRIORITY_DEBUG = 0x008,
	K15_LOG_PRIORITY_SUCCESS = 0x010,
	  
	K15_LOG_PRIORITY_DEFAULT = K15_LOG_PRIORITY_NORMAL | K15_LOG_PRIORITY_WARNING | K15_LOG_PRIORITY_ERROR | K15_LOG_PRIORITY_DEBUG | K15_LOG_PRIORITY_SUCCESS,

	K15_LOG_PRIORITY_CUSTOM_1 = 0x020,
	K15_LOG_PRIORITY_CUSTOM_2 = 0x040,
	K15_LOG_PRIORITY_CUSTOM_3 = 0x080,
	K15_LOG_PRIORITY_CUSTOM_4 = 0x100,

	K15_LOG_PRIORITY_ALL = K15_LOG_PRIORITY_DEFAULT | K15_LOG_PRIORITY_CUSTOM_1 | K15_LOG_PRIORITY_CUSTOM_2 | K15_LOG_PRIORITY_CUSTOM_3 | K15_LOG_PRIORITY_CUSTOM_4
};

typedef void (*K15_LogFnc)(const char*, LogPriority);

void K15_LogRegisterLogFnc(K15_LogFnc p_LogFunction, unsigned int p_LogPriorityFilterMask);
void K15_LogWrite(const char* p_Message, LogPriority p_LogPriority, ...);

#endif //_K15_Logging_h_