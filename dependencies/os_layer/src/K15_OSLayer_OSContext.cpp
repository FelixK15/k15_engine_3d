#include "K15_OSLayer_OSContext.h"

internal K15_OSLayerContext g_InternalOSContext = {};
internal uint8 g_InternalOSContextInitialized = FALSE;

/*********************************************************************************/
K15_OSLayerContext* K15_GetOSLayerContext()
{
	assert(g_InternalOSContextInitialized == TRUE);
	
	return &g_InternalOSContext;
}
/*********************************************************************************/
void K15_InternalSetOSLayerContext(K15_OSLayerContext p_OSLayerContext)
{
	assert(g_InternalOSContextInitialized == FALSE);

 	g_InternalOSContext = p_OSLayerContext;
 	g_InternalOSContextInitialized = TRUE;
}
/*********************************************************************************/