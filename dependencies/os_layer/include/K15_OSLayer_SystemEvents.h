#ifndef _K15_OSLayer_SystemEvent_h_
#define _K15_OSLayer_SystemEvent_h_

#include "K15_OSLayer_Prerequisites.h"
#include "K15_OSLayer_EventTypes.h"

#define K15_REMOVE_SYSTEM_EVENT_FLAG 0x001

struct K15_SystemEvent		// 96 Bits
{
	uint32 event;		//32 bit
	uint32 eventFlags;  //32 bit

	union 
	{
		uint32 windowHandle;
		uint32 key;
		uint32 button;
		uint32 wheelDelta;
		
		struct 
		{
			uint16 x;
			uint16 y;
		} position;
	}; //32 bit
}; // end of K15_SystemEvent struct declaration

uint8 K15_PumpSystemEvents();
uint8 K15_GetSystemEventFromQueue(K15_SystemEvent* p_SystemEvent, uint32 p_Flags);
uint8 K15_GetSystemEventFromQueueFiltered(K15_SystemEvent* p_SystemEvent, uint32 p_Flags, uint32 p_EventFilterFlag);
uint8 K15_AddSystemEventToQueue(K15_SystemEvent* p_SystemEvent);
void K15_SynchronizeSystemEventQueues();
#endif //_K15_OSLayer_SystemEvent_h_