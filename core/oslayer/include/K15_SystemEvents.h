#ifndef _K15_SystemEvent_h_
#define _K15_SystemEvent_h_

#include "K15_Prerequisites.h"
#include "K15_EventTypes.h"

#define K15_REMOVE_SYSTEM_EVENT_FLAG 0x001

struct K15_SystemEvent		// 96 Bits
{
	int32 event;		//32 bit
	int32 eventFlags;   //32 bit

	union 
	{
		uint32 windowHandle;
		uint32 key;
		uint32 mouseButton;
		uint32 controllerButton; //high order:controller ID | low order: button ID
		uint32 wheelDelta;
		uint32 controllerIndex;
		float batteryPercentage;
		float triggerValue;
		float thumbValue;
		union 
		{
			uint8 utf8Char;
			uint16 utf16Char;
			uint32 utf32Char;
		};
		struct 
		{
			int16 x;
			int16 y;
		} position;
		struct  
		{
			uint16 width;
			uint16 height;
		} size;
	} params; //32 bit
}; // end of K15_SystemEvent struct declaration

uint8 K15_PumpSystemEvents();
uint8 K15_GetSystemEventFromQueue(K15_SystemEvent* p_SystemEvent, uint32 p_Flags);
uint8 K15_AddSystemEventToQueue(K15_SystemEvent* p_SystemEvent);
#endif //_K15_SystemEvent_h_
