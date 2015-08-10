#ifndef _K15_RenderCommand_h_
#define _K15_RenderCommand_h_

enum K15_RenderCommandType : uint32
{
	K15_RENDER_COMMAND_RENDER_2D_TEXTURE = 0,
	K15_RENDER_COMMAND_CREATE_TEXTURE_FROM_TEXTURE_FORMAT,
	K15_RENDER_COMMAND_CREATE_SAMPLER_FROM_SAMPLER_FORMAT,
	
	K15_RENDER_COMMAND_COUNT
};

struct K15_RenderCommand
{
	K15_RenderCommandType type;
	uint32 parameterSize;
};

#endif //_K15_RenderCommand_h_