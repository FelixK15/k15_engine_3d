#ifndef _K15_Rendering_RenderStateDesc_h_
#define _K15_Rendering_RenderStateDesc_h_

#include "K15_RenderPrerequisites.h"

enum K15_BlendOperation : uint32
{
	K15_BLEND_OPERATION_ADD = 0,
	K15_BLEND_OPERATION_SUBTRACT,
	K15_BLEND_OPERATION_REVERSE_SUBTRACT,
	K15_BLEND_OPERATION_MIN,
	K15_BLEND_OPERATION_MAX
};

enum K15_BlendFactor : uint32
{
	K15_BLEND_FACTOR_ZERO = 0,
	K15_BLEND_FACTOR_ONE,
	K15_BLEND_FACTOR_SRC_COLOR,
	K15_BLEND_FACTOR_INVERSE_SRC_COLOR,
	K15_BLEND_FACTOR_SRC_ALPHA,
	K15_BLEND_FACTOR_INVERSE_SRC_ALPHA,
	K15_BLEND_FACTOR_DEST_ALPHA,
	K15_BLEND_FACTOR_INVERSE_DEST_ALPHA,
	K15_BLEND_FACTOR_DESC_COLOR,
	K15_BLEND_FACTOR_INVERSE_DESC_COLOR
};

enum K15_Comparison : uint32
{
	K15_COMPARISON_NEVER = 0,
	K15_COMPARISON_LESS,
	K15_COMPARISON_EQUAL,
	K15_COMPARISON_LESS_EQUAL,
	K15_COMPARISON_GREATER,
	K15_COMPARISON_NOT_EQUAL,
	K15_COMPARISON_GREATER_EQUAL,
	K15_COMPARISON_ALWAYS
};

enum K15_VertexOrder : uint32
{
	K15_VERTEX_ORDER_CLOCKWISE = 0,
	K15_VERTEX_ORDER_COUNTER_CLOCKWISE
};

enum K15_FillMode : uint32
{
	K15_FILLMODE_WIREFRAME = 0,
	K15_FILLMODE_SOLID
};

enum K15_CullingMode : uint32
{
	K15_CULLING_MODE_NONE = 0,
	K15_CULLING_MODE_FRONT,
	K15_CULLING_MODE_BACK
};

struct K15_RenderBlendStateDesc
{
	//Render Target ID?

	K15_BlendFactor sourceBlendFactorRGB;
	K15_BlendFactor destinationBlendFactorRGB;

	K15_BlendOperation blendOperationRGB;

	K15_BlendFactor sourceBlendFactorAlpha;
	K15_BlendFactor destinationBlendFactorAlpha;

	K15_BlendOperation blendOperationAlpha;

	uint8 enabled;
};

struct K15_RenderDepthStateDesc
{
	K15_Comparison compareFunction;
	uint8 enabled;
};

struct K15_RenderStencilStateDesc
{
	uint8 writeMask;
	uint8 readMask;
	uint8 enabled;
};

struct K15_RenderRasterizerStateDesc
{
	K15_FillMode fillMode;
	K15_CullingMode cullingMode;
	K15_VertexOrder vertexOrder;

	int32 depthBias;
	float depthBiasClamp;

	uint8 scissoringEnabled;
};

#endif //_K15_Rendering_RenderStateDesc_h_