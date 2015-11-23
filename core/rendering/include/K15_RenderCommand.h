#ifndef _K15_RenderCommand_h_
#define _K15_RenderCommand_h_

enum K15_RenderCommandType : uint32
{
	K15_RENDER_COMMAND_RENDER_2D_TEXTURE = 0,
	K15_RENDER_COMMAND_RENDER_2D_TEXTURE_EX,
	K15_RENDER_COMMAND_RENDER_2D_TEXT,
	K15_RENDER_COMMAND_CREATE_TEXTURE_FROM_TEXTURE_FORMAT,
	K15_RENDER_COMMAND_CREATE_SAMPLER_FROM_SAMPLER_FORMAT,
	K15_RENDER_COMMAND_CREATE_TEXTURE,
	K15_RENDER_COMMAND_CREATE_PROGRAM,
	K15_RENDER_COMMAND_WINDOW_RESIZED,
	K15_RENDER_COMMAND_DELETE_TEXTURE,
	K15_RENDER_COMMAND_DELETE_SAMPLER,
	K15_RENDER_COMMAND_DELETE_PROGRAM,
	K15_RENDER_COMMAND_COUNT
};

struct K15_RenderCommand
{
	K15_RenderCommandType type;
	uint32 parameterSize;
};

#endif //_K15_RenderCommand_h_