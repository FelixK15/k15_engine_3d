#include "K15_SystemEvents.h"
#include "K15_OSContext.h"

/*********************************************************************************/
//intern K15_SystemEvent g_EventList[K15_EVENT_FLAG_COUNT][K15_EVENTS_PER_BUCKET] = {0};
intern K15_SystemEvent g_MasterEventList[K15_EVENTS_PER_BUCKET] = {0};
intern uint32 g_MasterListSize = 0;
//intern uint32 g_EventListSize[K15_EVENT_TYPE_COUNT] = {0};
/*********************************************************************************/



/*********************************************************************************/
uint8 K15_PumpSystemEvents()
{
	K15_OSContext* osLayerContext = K15_GetOSLayerContext();

	return osLayerContext->events.pumpSystemEvents(osLayerContext);
}
/*********************************************************************************/
uint8 K15_GetSystemEventFromQueue(K15_SystemEvent* p_SystemEvent, uint32 p_Flags)
{
	if (g_MasterListSize == 0)
	{
		return K15_SYSTEM_EVENT_QUEUE_EMPTY;
	}
	
	*p_SystemEvent = g_MasterEventList[0];

	if ((p_Flags & K15_REMOVE_SYSTEM_EVENT_FLAG) > 0)
	{
		g_MasterListSize -= 1;
		memmove(g_MasterEventList, g_MasterEventList + 1, g_MasterListSize * sizeof(K15_SystemEvent));
	}

	return K15_SUCCESS;
}
/*********************************************************************************/
// uint8 K15_GetSystemEventFromQueueFiltered(K15_SystemEvent* p_SystemEvent, uint32 p_Flags, uint32 p_EventFilterFlag)
// {
// 	uint32 listID = 0;
// 	uint32 index = 0;
// 	uint32 size = 0;
// 	K15_SystemEvent* eventList = 0;
// 
// 	if ((p_EventFilterFlag & K15_SYSTEM_EVENT_FLAG) > 0)
// 	{
// 		listID = K15_SYSTEM_EVENT_ID;
// 		size = g_EventListSize[K15_SYSTEM_EVENT_ID];	
// 		eventList = (K15_SystemEvent*)g_EventList[K15_SYSTEM_EVENT_ID];
// 	}
// 	else if ((p_EventFilterFlag & K15_INPUT_EVENT_FLAG) > 0)
// 	{
// 		listID = K15_INPUT_EVENT_ID;
// 		size = g_EventListSize[K15_INPUT_EVENT_ID];	
// 		eventList = (K15_SystemEvent*)g_EventList[K15_INPUT_EVENT_ID];
// 	}
// 	else if ((p_EventFilterFlag & K15_WINDOW_EVENT_FLAG) > 0)
// 	{
// 		listID = K15_WINDOW_EVENT_ID;
// 		size = g_EventListSize[K15_WINDOW_EVENT_ID];	
// 		eventList = (K15_SystemEvent*)g_EventList[K15_WINDOW_EVENT_ID];
// 	}
// 
// 	if (size == 0)
// 	{
// 		return K15_SYSTEM_EVENT_QUEUE_EMPTY;
// 	}
// 
// 	index = size - 1;
// 
// 	assert(eventList);
// 
// 	*p_SystemEvent = eventList[index];
// 
// 	if ((p_Flags & K15_REMOVE_SYSTEM_EVENT_FLAG) > 0)
// 	{
// 		g_EventListSize[listID] -= 1;
// 	}
// 
// 	return K15_SUCCESS;
// }
/*********************************************************************************/
// void K15_SynchronizeSystemEventQueues()
// {
// 	g_EventListSize[K15_SYSTEM_EVENT_ID] = 0;
// 	g_EventListSize[K15_WINDOW_EVENT_ID] = 0;
// 	g_EventListSize[K15_INPUT_EVENT_ID] = 0;
// 	
// 	g_MasterListSize = 0;
// }
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

// 	if ((p_SystemEvent->eventFlags & K15_INPUT_EVENT_FLAG) > 0)
// 	{
// 		index = g_EventListSize[K15_INPUT_EVENT_ID];
// 		g_EventList[K15_INPUT_EVENT_ID][index] = *p_SystemEvent;
// 		g_EventListSize[K15_INPUT_EVENT_ID] = index + 1;
// 	}
// 
// 	if ((p_SystemEvent->eventFlags & K15_SYSTEM_EVENT_FLAG) > 0)
// 	{
// 		index = g_EventListSize[K15_SYSTEM_EVENT_ID];
// 		g_EventList[K15_SYSTEM_EVENT_ID][index] = *p_SystemEvent;
// 		g_EventListSize[K15_SYSTEM_EVENT_ID] = index + 1;
// 	}
// 
// 	if ((p_SystemEvent->eventFlags & K15_WINDOW_EVENT_FLAG) > 0)
// 	{
// 		index = g_EventListSize[K15_WINDOW_EVENT_ID];
// 		g_EventList[K15_WINDOW_EVENT_ID][index] = *p_SystemEvent;
// 		g_EventListSize[K15_WINDOW_EVENT_ID] = index + 1;
// 	}

	return K15_SUCCESS;
}
/*********************************************************************************/

