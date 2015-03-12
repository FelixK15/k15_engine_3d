#ifndef _K15_Rendering_RenderProgramDesc_h_
#define _K15_Rendering_RenderProgramDesc_h_

#include "K15_RenderPrerequisites.h"

enum K15_RenderProgramType
{
	K15_RENDER_PROGRAM_TYPE_VERTEX = 0,
	K15_RENDER_PROGRAM_TYPE_FRAGMENT,
	K15_RENDER_PROGRAM_TYPE_GEOMETRY
};

enum K15_RenderProgramSource
{
	K15_RENDER_PROGRAM_SOURCE_FILE = 0,
	K15_RENDER_PROGRAM_SOURCE_CODE
};

struct K15_RenderProgramDesc
{
	const char* file;
	const char* code;
	K15_RenderProgramType type;
	K15_RenderProgramSource source;
};

#endif //_K15_Rendering_RenderProgramDesc_h_