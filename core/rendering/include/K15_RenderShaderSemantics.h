#ifndef _K15_Rendering_ShaderSemantics_h_
#define _K15_Rendering_ShaderSemantics_h_

#include "K15_RenderPrerequisites.h"

//semantic groups
enum K15_ShaderUniformGroups : uint8
{
	K15_UNIFORM_GROUP_PER_OBJECT = 0,
	K15_UNIFORM_GROUP_PER_MATERIAL,
	K15_UNIFORM_GROUP_PER_CAMERA,
	K15_UNIFORM_GROUP_COUNT
};

//uniforms
enum K15_ShaderUniformSemantics : uint8
{
	K15_UNIFORM_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX = 0,
	K15_UNIFORM_SEMANTIC_PROJECTION_MATRIX,
	K15_UNIFORM_SEMANTIC_VIEW_MATRIX,
	K15_UNIFORM_SEMANTIC_NORMAL_MATRIX,
	K15_UNIFORM_SEMANTIC_MODEL_MATRIX,
	K15_UNIFORM_SEMANTIC_GUI_PROJECTION_MATRIX,
	K15_UNIFORM_SEMANTIC_DATA,
	K15_UNIFORM_SEMANTIC_TEX1_DIMENSION,
	K15_UNIFORM_SEMANTIC_TEX2_DIMENSION,
	K15_UNIFORM_SEMANTIC_TEX3_DIMENSION,
	K15_UNIFORM_SEMANTIC_TEX4_DIMENSION,
	K15_UNIFORM_SEMANTIC_TEX5_DIMENSION,
	K15_UNIFORM_SEMANTIC_TEX6_DIMENSION,
	K15_UNIFORM_SEMANTIC_TEX7_DIMENSION,
	K15_UNIFORM_SEMANTIC_TEX8_DIMENSION,
	
	K15_UNIFORM_SEMANTIC_COUNT
};

//attributes
enum K15_ShaderAttributeSemantics : uint8
{
	K15_ATTRIBUTE_SEMANTIC_POSITION = K15_UNIFORM_SEMANTIC_COUNT,
	K15_ATTRIBUTE_SEMANTIC_NORMAL,	
	K15_ATTRIBUTE_SEMANTIC_TEXCOORD1,
	K15_ATTRIBUTE_SEMANTIC_TEXCOORD2,
	K15_ATTRIBUTE_SEMANTIC_TEXCOORD3,
	K15_ATTRIBUTE_SEMANTIC_TEXCOORD4,
	K15_ATTRIBUTE_SEMANTIC_TEXCOORD5,
	K15_ATTRIBUTE_SEMANTIC_TEXCOORD6,
	K15_ATTRIBUTE_SEMANTIC_TEXCOORD7,
	K15_ATTRIBUTE_SEMANTIC_TEXCOORD8,
	K15_ATTRIBUTE_SEMANTIC_COLOR1,
	K15_ATTRIBUTE_SEMANTIC_COLOR2,
	K15_ATTRIBUTE_SEMANTIC_COLOR3,
	K15_ATTRIBUTE_SEMANTIC_COLOR4,

	K15_ATTRIBUTE_SEMANTIC_COUNT
};

//types
enum K15_ShaderType : uint8
{
	K15_TYPE_INT = 0,
	K15_TYPE_FLOAT,
	K15_TYPE_FLOAT_VECTOR2,
	K15_TYPE_FLOAT_VECTOR3,
	K15_TYPE_FLOAT_VECTOR4,
	K15_TYPE_FLOAT_MATRIX2,
	K15_TYPE_FLOAT_MATRIX3,
	K15_TYPE_FLOAT_MATRIX4,
	K15_TYPE_INT_VECTOR2,
	K15_TYPE_INT_VECTOR3,
	K15_TYPE_INT_VECTOR4,
	K15_TYPE_SAMPLER_2D,
	K15_TYPE_TEXTURE_2D,

	K15_TYPE_COUNT
};

#define K15_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX_VARIABLE_NAME "g_MVPMatrix"
#define K15_SEMANTIC_PROJECTION_MATRIX_VARIABLE_NAME "g_ProjMatrix"
#define K15_SEMANTIC_VIEW_MATRIX_VARIABLE_NAME "g_ViewMatrix"
#define K15_SEMANTIC_NORMAL_MATRIX_VARIABLE_NAME "g_NormalMatrix"
#define K15_SEMANTIC_MODEL_MATRIX_VARIABLE_NAME "g_ModelMatrix"
#define K15_SEMANTIC_GUI_PROJECTION_MATRIX_VARIABLE_NAME "g_GUIProjMatrix"
#define K15_SEMANTIC_TEX1_DIMENSION_NAME "g_Tex1Dimension"
#define K15_SEMANTIC_TEX2_DIMENSION_NAME "g_Tex2Dimension"
#define K15_SEMANTIC_TEX3_DIMENSION_NAME "g_Tex3Dimension"
#define K15_SEMANTIC_TEX4_DIMENSION_NAME "g_Tex4Dimension"
#define K15_SEMANTIC_TEX5_DIMENSION_NAME "g_Tex5Dimension"
#define K15_SEMANTIC_TEX6_DIMENSION_NAME "g_Tex6Dimension"
#define K15_SEMANTIC_TEX7_DIMENSION_NAME "g_Tex7Dimension"
#define K15_SEMANTIC_TEX8_DIMENSION_NAME "g_Tex8Dimension"

#define K15_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX_SEMANTIC_NAME "MVPMATRIX"
#define K15_SEMANTIC_PROEJCTION_MATRIX_SEMANTIC_NAME "PROJECTIONMATRIX"
#define K15_SEMANTIC_VIEW_MATRIX_SEMANTIC_NAME "VIEWMATRIX"
#define K15_SEMANTIC_NORMAL_MATRIX_SEMANTIC_NAME "NORMALMATRIX"
#define K15_SEMANTIC_MODEL_MATRIX_SEMANTIC_NAME "MODELMATRIX"
#define K15_SEMANTIC_GUI_PROJECTION_MATRIX_SEMANTIC_NAME "GUIPROJECTIONMATRIX"
#define K15_SEMANTIC_DATA_SEMANTIC_NAME "DATA"

#define K15_SEMANTIC_POSITION_SEMANTIC_NAME "POSITION"
#define K15_SEMANTIC_NORMAL_SEMANTIC_NAME "NORMAL"
#define K15_SEMANTIC_TEXCOORD1_SEMANTIC_NAME "TEXCOORD1"
#define K15_SEMANTIC_TEXCOORD2_SEMANTIC_NAME "TEXCOORD2"
#define K15_SEMANTIC_TEXCOORD3_SEMANTIC_NAME "TEXCOORD3"
#define K15_SEMANTIC_TEXCOORD4_SEMANTIC_NAME "TEXCOORD4"
#define K15_SEMANTIC_TEXCOORD5_SEMANTIC_NAME "TEXCOORD5"
#define K15_SEMANTIC_TEXCOORD6_SEMANTIC_NAME "TEXCOORD6"
#define K15_SEMANTIC_TEXCOORD7_SEMANTIC_NAME "TEXCOORD7"
#define K15_SEMANTIC_TEXCOORD8_SEMANTIC_NAME "TEXCOORD8"
#define K15_SEMANTIC_TEX1_DIMENSION_SEMANTIC_NAME "TEX_DIMENSION1"
#define K15_SEMANTIC_TEX2_DIMENSION_SEMANTIC_NAME "TEX_DIMENSION2"
#define K15_SEMANTIC_TEX3_DIMENSION_SEMANTIC_NAME "TEX_DIMENSION3"
#define K15_SEMANTIC_TEX4_DIMENSION_SEMANTIC_NAME "TEX_DIMENSION4"
#define K15_SEMANTIC_TEX5_DIMENSION_SEMANTIC_NAME "TEX_DIMENSION5"
#define K15_SEMANTIC_TEX6_DIMENSION_SEMANTIC_NAME "TEX_DIMENSION6"
#define K15_SEMANTIC_TEX7_DIMENSION_SEMANTIC_NAME "TEX_DIMENSION7"
#define K15_SEMANTIC_TEX8_DIMENSION_SEMANTIC_NAME "TEX_DIMENSION8"
#define K15_SEMANTIC_COLOR1_SEMANTIC_NAME "COLOR1"
#define K15_SEMANTIC_COLOR2_SEMANTIC_NAME "COLOR2"
#define K15_SEMANTIC_COLOR3_SEMANTIC_NAME "COLOR3"
#define K15_SEMANTIC_COLOR4_SEMANTIC_NAME "COLOR4"

#define K15_TYPE_INT_NAME "int"
#define K15_TYPE_FLOAT_NAME "float"
#define K15_TYPE_FLOAT_VECTOR2_NAME "float2"
#define K15_TYPE_FLOAT_VECTOR3_NAME "float3"
#define K15_TYPE_FLOAT_VECTOR4_NAME "float4"
#define K15_TYPE_FLOAT_MATRIX2_NAME "float2x2"
#define K15_TYPE_FLOAT_MATRIX3_NAME "float3x3"
#define K15_TYPE_FLOAT_MATRIX4_NAME "float4x4"
#define K15_TYPE_INT_VECTOR2_NAME "int2"
#define K15_TYPE_INT_VECTOR3_NAME "int3"
#define K15_TYPE_INT_VECTOR4_NAME "int4"
#define K15_TYPE_SAMPLER_2D_NAME "sampler2d"
#define K15_TYPE_TEXTURE_2D_NAME "texture2d"

#endif //_K15_Rendering_ShaderSemantics_h_