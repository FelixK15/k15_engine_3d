#include "K15_OSLayer_SystemEvents.h"
#include "K15_OSLayer_OSContext.h"

/*********************************************************************************/
internal K15_SystemEvent g_EventList[K15_EVENT_TYPE_COUNT][K15_EVENTS_PER_BUCKET] = {0};
internal K15_SystemEvent g_MasterEventList[K15_EVENTS_PER_BUCKET] = {0};
internal uint32 g_MasterListSize = 0;
internal uint32 g_EventListSize[K15_EVENT_TYPE_COUNT] = {0};
/*********************************************************************************/



/*********************************************************************************/
uint8 K15_PumpSystemEvents()
{
	K15_OSLayerContext* osLayerContext = K15_GetOSLayerContext();

	return osLayerContext->pumpSystemEvents(osLayerContext);
}
/*********************************************************************************/
uint8 K15_GetSystemEventFromQueue(K15_SystemEvent* p_SystemEvent, uint32 p_Flags)
{
	uint32 index = g_MasterListSize;

	if (index == 0)
	{
		return K15_SYSTEM_EVENT_QUEUE_EMPTY;
	}
	
	index -= 1;

	*p_SystemEvent = g_MasterEventList[index];

	if ((p_Flags & K15_REMOVE_SYSTEM_EVENT_FLAG) > 0)
	{
		g_MasterListSize = index;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
uint8 K15_GetSystemEventFromQueueFiltered(K15_SystemEvent* p_SystemEvent, uint32 p_Flags, uint32 p_EventFilterFlag)
{
	uint32 listID = 0;
	uint32 index = 0;
	uint32 size = 0;
	K15_SystemEvent* eventList = 0;

	if ((p_EventFilterFlag & K15_SYSTEM_EVENT_FLAG) > 0)
	{
		listID = K15_SYSTEM_EVENT_ID;
		size = g_EventListSize[K15_SYSTEM_EVENT_ID];	
		eventList = (K15_SystemEvent*)g_EventList[K15_SYSTEM_EVENT_ID];
	}
	else if ((p_EventFilterFlag & K15_INPUT_EVENT_FLAG) > 0)
	{
		listID = K15_INPUT_EVENT_ID;
		size = g_EventListSize[K15_INPUT_EVENT_ID];	
		eventList = (K15_SystemEvent*)g_EventList[K15_INPUT_EVENT_ID];
	}
	else if ((p_EventFilterFlag & K15_WINDOW_EVENT_FLAG) > 0)
	{
		listID = K15_WINDOW_EVENT_ID;
		size = g_EventListSize[K15_WINDOW_EVENT_ID];	
		eventList = (K15_SystemEvent*)g_EventList[K15_WINDOW_EVENT_ID];
	}

	if (size == 0)
	{
		return K15_SYSTEM_EVENT_QUEUE_EMPTY;
	}

	index = size - 1;

	*p_SystemEvent = eventList[index];

	if ((p_Flags & K15_REMOVE_SYSTEM_EVENT_FLAG) > 0)
	{
		g_EventListSize[listID] -= 1;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
void K15_SynchronizeSystemEventQueues()
{
	g_EventListSize[K15_SYSTEM_EVENT_ID] = 0;
	g_EventListSize[K15_WINDOW_EVENT_ID] = 0;
	g_EventListSize[K15_INPUT_EVENT_ID] = 0;
	
	g_MasterListSize = 0;
}
/*********************************************************************************/
uint8 K15_AddSystemEventToQueue(K15_SystemEvent* p_SystemEvent)
{
	uint32 index = g_MasterListSize;
	
	if (index == K15_EVENTS_PER_BUCKET - 1)
	{
		return K15_SYSTEM_EVENT_QUEUE_FULL;
	}
	
	g_MasterEventList[index] = *p_SystemEvent;

	g_MasterListSize = index + 1;

	if ((p_SystemEvent->eventFlags & K15_INPUT_EVENT_FLAG) > 0)
	{
		index = g_EventListSize[K15_INPUT_EVENT_ID];
		g_EventList[K15_INPUT_EVENT_ID][index] = *p_SystemEvent;
		g_EventListSize[K15_INPUT_EVENT_ID] = index + 1;
	}

	if ((p_SystemEvent->eventFlags & K15_SYSTEM_EVENT_FLAG) > 0)
	{
		index = g_EventListSize[K15_SYSTEM_EVENT_ID];
		g_EventList[K15_SYSTEM_EVENT_ID][index] = *p_SystemEvent;
		g_EventListSize[K15_SYSTEM_EVENT_ID] = index + 1;
	}

	if ((p_SystemEvent->eventFlags & K15_WINDOW_EVENT_FLAG) > 0)
	{
		index = g_EventListSize[K15_WINDOW_EVENT_ID];
		g_EventList[K15_WINDOW_EVENT_ID][index] = *p_SystemEvent;
		g_EventListSize[K15_WINDOW_EVENT_ID] = index + 1;
	}

	return K15_SUCCESS;
}
/*********************************************************************************/

