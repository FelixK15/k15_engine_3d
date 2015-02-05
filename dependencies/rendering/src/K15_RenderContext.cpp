#include "K15_RenderContext.h"

#include <K15_OSLayer_OSContext.h>
#include <K15_OSLayer_ErrorCodes.h>

#include "GL/K15_RenderGLContext.h"

/*********************************************************************************/
uint8 K15_CreateRenderContext(K15_RenderContext* p_RenderContext, K15_OSLayerContext* p_OSContext)
{
	K15_RenderContext* renderContext = (K15_RenderContext*)malloc(sizeof(K15_RenderContext));

	if (!renderContext)
	{
		return K15_ERROR_OUT_OF_MEMORY;
	}

	uint8 createGLRenderContext = TRUE;

	//TODO: DX 11/10/9

	if (createGLRenderContext == TRUE)
	{
		uint8 result = K15_CreateGLRenderContext(renderContext, p_OSContext);
	
		if (result != K15_SUCCESS)
		{
			return result;
		}
	}
	return K15_SUCCESS;
}
/*********************************************************************************/