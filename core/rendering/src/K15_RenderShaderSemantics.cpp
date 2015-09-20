#include "K15_RenderShaderSemantics.h"
#include "K15_ShaderCompiler.h"

/*********************************************************************************/
 K15_SemanticTable K15_CreateSemanticTable()
{
	K15_SemanticTable table = {};

	uint32 counter = 0;

	table.entries[counter].semanticID = K15_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX_ID;
	table.entries[counter].semanticGroupID = K15_SEMANTIC_GROUP_ID_PER_OBJECT;
	table.entries[counter].variableName = K15_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX_VARIABLE_NAME;
	table.entries[counter++].semanticName = K15_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_MODEL_MATRIX_ID;
	table.entries[counter].semanticGroupID = K15_SEMANTIC_GROUP_ID_PER_OBJECT;
	table.entries[counter].variableName = K15_SEMANTIC_MODEL_MATRIX_VARIABLE_NAME;
	table.entries[counter++].semanticName = K15_SEMANTIC_MODEL_MATRIX_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_DATA_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_DATA_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_POSITION_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_POSITION_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_NORMAL_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_NORMAL_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_TEXCOORD1_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_TEXCOORD1_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_TEXCOORD2_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_TEXCOORD2_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_TEXCOORD3_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_TEXCOORD3_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_TEXCOORD4_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_TEXCOORD4_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_TEXCOORD5_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_TEXCOORD5_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_TEXCOORD6_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_TEXCOORD6_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_TEXCOORD7_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_TEXCOORD7_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_TEXCOORD8_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_TEXCOORD8_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_COLOR1_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_COLOR1_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_COLOR2_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_COLOR2_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_COLOR3_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_COLOR3_SEMANTIC_NAME;

	table.entries[counter].semanticID = K15_SEMANTIC_COLOR4_ID;
	table.entries[counter++].semanticName = K15_SEMANTIC_COLOR4_SEMANTIC_NAME;

	table.numEntries = counter;

	K15_ASSERT_TEXT(counter < K15_MAX_SHADER_SEMANTICS, "Shader semantic table exceeds '%d' entries (%d entries provided)", K15_MAX_SHADER_SEMANTICS, counter);

	return table;
}
/*********************************************************************************/
K15_TypeTable K15_CreateTypeTable()
{
	K15_TypeTable table = {};

	uint32 counter = 0;

	table.entries[counter].typeID = K15_TYPE_INT_ID;
	table.entries[counter].typeName = K15_TYPE_INT_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(int);

	table.entries[counter].typeID = K15_TYPE_FLOAT_ID;
	table.entries[counter].typeName = K15_TYPE_FLOAT_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float);

	table.entries[counter].typeID = K15_TYPE_FLOAT_VECTOR2_ID;
	table.entries[counter].typeName = K15_TYPE_FLOAT_VECTOR2_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float) * 2;

	table.entries[counter].typeID = K15_TYPE_FLOAT_VECTOR3_ID;
	table.entries[counter].typeName = K15_TYPE_FLOAT_VECTOR3_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float) * 3;

	table.entries[counter].typeID = K15_TYPE_FLOAT_VECTOR4_ID;
	table.entries[counter].typeName = K15_TYPE_FLOAT_VECTOR4_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float) * 4;

	table.entries[counter].typeID = K15_TYPE_FLOAT_MATRIX2_ID;
	table.entries[counter].typeName = K15_TYPE_FLOAT_MATRIX2_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float) * 4;

	table.entries[counter].typeID = K15_TYPE_FLOAT_MATRIX3_ID;
	table.entries[counter].typeName = K15_TYPE_FLOAT_MATRIX3_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float) * 9;

	table.entries[counter].typeID = K15_TYPE_FLOAT_MATRIX4_ID;
	table.entries[counter].typeName = K15_TYPE_FLOAT_MATRIX4_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float) * 16;

	table.entries[counter].typeID = K15_TYPE_SAMPLER_2D_ID;
	table.entries[counter].typeName = K15_TYPE_SAMPLER_2D_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(int);

	table.entries[counter].typeID = K15_TYPE_TEXTURE_2D_ID;
	table.entries[counter].typeName = K15_TYPE_TEXTURE_2D_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(int);

	table.numEntries = counter;

	K15_ASSERT_TEXT(counter < K15_MAX_SHADER_TYPES, "Shader type table exceeds '%d' entries (%d entries provided)", K15_MAX_SHADER_TYPES, counter);

	return table;
}
/*********************************************************************************/