#include "K15_RenderFormats.h"

/*********************************************************************************/
K15_RenderFormat K15_ConvertToRenderFormat(uint8 p_PixelFormatFromTextureFormat)
{
	K15_RenderFormat renderFormat = K15_RENDER_FORMAT_R8G8B8A8_UINT;

	switch(p_PixelFormatFromTextureFormat)
	{
		case K15_TEXTURE_DXT1_COMPRESSION:
		{
			renderFormat = K15_RENDER_FORMAT_RGB_DXT_1;
			break;
		}
		
		case K15_TEXTURE_DXT3_COMPRESSION:
		{
			renderFormat = K15_RENDER_FORMAT_RGBA_DXT_3;
			break;
		}

		case K15_TEXTURE_DXT5_COMPRESSION:
		{
			renderFormat = K15_RENDER_FORMAT_RGBA_DXT_5;
			break;
		}

		case K15_TEXTURE_R8G8B8_UBYTE:
		{
			renderFormat = K15_RENDER_FORMAT_R8G8B8_UINT;
			break;
		}

		case K15_TEXTURE_R8G8B8A8_UBYTE:
		{
			renderFormat = K15_RENDER_FORMAT_R8G8B8A8_UINT;
			break;
		}
	}

	return renderFormat;
}
/*********************************************************************************/