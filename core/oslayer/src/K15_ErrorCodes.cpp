#include "K15_ErrorCodes.h"

const char* K15_GetErrorCodeString(result8 p_Result)
{
	const char* string = 0;

	switch(p_Result)
	{
		case K15_SUCCESS:
		{
			string = "Success";
			break;
		}

		case K15_OS_ERROR_SYSTEM:
		{
			string = "System Error";
			break;
		}

		case K15_OS_ERROR_MONITOR_NOT_FOUND:
		{
			string = "Monitor not found";
			break;
		}

		case K15_OS_ERROR_RESOLUTION_NOT_SUPPORTED:
		{
			string = "Resolution not supported";
			break;
		}

		case K15_OS_ERROR_OUT_OF_MEMORY:
		{
			string = "Out of memory";
			break;
		}

		case K15_OS_ERROR_NO_WINDOW:
		{
			string = "No window";
			break;
		}

		case K15_OS_ERROR_FILE_NOT_FOUND:
		{
			string = "File not found";
			break;
		}

		case K15_OS_ERROR_THREAD_ALREADY_STARTED:
		{
			string = "Thread already started";
			break;
		}

		case K15_OS_ERROR_THREAD_NOT_JOINABLE:
		{
			string = "Thread not joinable";
			break;
		}

		case K15_OS_TIMEOUT:
		{
			string = "Timeout";
			break;
		}

		case K15_OS_ERROR_DYNAMIC_LIBRARY_NOT_LOADED:
		{
			string = "Library not loaded";
			break;
		}

		case K15_OS_ERROR_DYNAMIC_LIBRARY_NOT_RELOADABLE:
		{
			string = "Library not re loadable";
			break;
		}

		case K15_OS_ERROR_EMPTY_FILE:
		{
			string = "Empty file";
			break;
		}

		case K15_OS_ERROR_RENDER_CONTEXT_ALREADY_CREATED:
		{
			string = "Render context already created";
			break;
		}

		case K15_ERROR_MAX_RENDER_COMMAND_QUEUE_REACHED:
		{
			string = "Render command queue maximum reached";
			break;
		}

		case K15_ERROR_MAX_RENDER_COMMAND_BUFFER_FULL:
		{
			string = "Render command buffer is full";
			break;
		}

		case K15_ERROR_NO_RENDER_COMMAND:
		{
			string = "No render command";
			break;
		}

		case K15_ERROR_RENDER_COMMAND_NOT_IMPLEMENTED:
		{
			string = "Render command not implemented";
			break;
		}

		case K15_ERROR_INVALID_PARAMETER_SIZE:
		{
			string = "Invalid parameter size";
			break;
		}

		case K15_ERROR_INVALID_BUFFER_TYPE:
		{
			string = "Invalid buffer type";
			break;
		}

		case K15_ERROR_LAST_COMMAND_NOT_FINISHED:
		{
			string = "Last command not finished";
			break;
		}

		case K15_ERROR_RENDER_BUFFER_LOCKED:
		{
			string = "Render buffer locked";
			break;
		}

		case K15_ERROR_RENDER_API:
		{
			string = "Render API";
			break;
		}

		case K15_ERROR_RENDER_PROGRAM_COMPILATION:
		{
			string = "Render program compilation";
			break;
		}

		case K15_ERROR_RENDER_UNIFORM_NOT_FOUND:
		{
			string = "Render uniform not found";
			break;
		}
		
		case K15_ERROR_RENDER_COMMAND_QUEUE_ALREADY_DISPATCHED:
		{
			string = "Render command queue already dispatched";
			break;
		}

		default:
		{
			string = "No error code text.";
			break;
		}
	}

	return string;
}
