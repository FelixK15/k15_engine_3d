#include "K15_OSContext.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>

intern K15_OSContext g_InternalOSContext = {};
intern uint8 g_InternalOSContextInitialized = K15_FALSE;

/*********************************************************************************/
K15_OSContext* K15_GetOSLayerContext()
{
	K15_ASSERT_TEXT(g_InternalOSContextInitialized == K15_TRUE, "No os context has been created yet.");
	
	return &g_InternalOSContext;
}
/*********************************************************************************/
void K15_InternalSetOSLayerContext(K15_OSContext p_OSLayerContext)
{
	K15_ASSERT_TEXT(g_InternalOSContextInitialized == K15_FALSE, "OS Context has already been set.");

 	g_InternalOSContext = p_OSLayerContext;
 	g_InternalOSContextInitialized = K15_TRUE;
}
/*********************************************************************************/