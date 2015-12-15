#include "K15_RenderShaderSemantics.h"
#include "K15_ShaderCompiler.h"

/*********************************************************************************/
intern uint32 K15_InternalAddPerObjectUniformEntries(K15_SemanticTable* p_SemanticTable, uint32 p_Counter)
{
	//model view projection matrix
	p_SemanticTable->entries[p_Counter].semanticID = K15_UNIFORM_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX;
	p_SemanticTable->entries[p_Counter].semanticGroupID = K15_UNIFORM_GROUP_PER_OBJECT;
	p_SemanticTable->entries[p_Counter].variableName = K15_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX_VARIABLE_NAME;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_MODEL_VIEW_PROJECTION_MATRIX_SEMANTIC_NAME;

	//model matrix
	p_SemanticTable->entries[p_Counter].semanticID = K15_UNIFORM_SEMANTIC_MODEL_MATRIX;
	p_SemanticTable->entries[p_Counter].semanticGroupID = K15_UNIFORM_GROUP_PER_OBJECT;
	p_SemanticTable->entries[p_Counter].variableName = K15_SEMANTIC_MODEL_MATRIX_VARIABLE_NAME;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_MODEL_MATRIX_SEMANTIC_NAME;

	return p_Counter;
}
/*********************************************************************************/
intern uint32 K15_InternalAddPerMaterialUniformEntries(K15_SemanticTable* p_SemanticTable, uint32 p_Counter)
{
	p_SemanticTable->entries[p_Counter].semanticID = K15_UNIFORM_SEMANTIC_TEXCOORD1_DIMENSION;
	p_SemanticTable->entries[p_Counter].semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	p_SemanticTable->entries[p_Counter].variableName = K15_SEMANTIC_TEXCOORD1_DIMENSION_NAME;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD1_DIMENSION_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_UNIFORM_SEMANTIC_TEXCOORD2_DIMENSION;
	p_SemanticTable->entries[p_Counter].semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	p_SemanticTable->entries[p_Counter].variableName = K15_SEMANTIC_TEXCOORD2_DIMENSION_NAME;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD2_DIMENSION_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_UNIFORM_SEMANTIC_TEXCOORD3_DIMENSION;
	p_SemanticTable->entries[p_Counter].semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	p_SemanticTable->entries[p_Counter].variableName = K15_SEMANTIC_TEXCOORD3_DIMENSION_NAME;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD3_DIMENSION_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_UNIFORM_SEMANTIC_TEXCOORD4_DIMENSION;
	p_SemanticTable->entries[p_Counter].semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	p_SemanticTable->entries[p_Counter].variableName = K15_SEMANTIC_TEXCOORD4_DIMENSION_NAME;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD4_DIMENSION_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_UNIFORM_SEMANTIC_TEXCOORD5_DIMENSION;
	p_SemanticTable->entries[p_Counter].semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	p_SemanticTable->entries[p_Counter].variableName = K15_SEMANTIC_TEXCOORD5_DIMENSION_NAME;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD5_DIMENSION_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_UNIFORM_SEMANTIC_TEXCOORD6_DIMENSION;
	p_SemanticTable->entries[p_Counter].semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	p_SemanticTable->entries[p_Counter].variableName = K15_SEMANTIC_TEXCOORD6_DIMENSION_NAME;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD6_DIMENSION_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_UNIFORM_SEMANTIC_TEXCOORD7_DIMENSION;
	p_SemanticTable->entries[p_Counter].semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	p_SemanticTable->entries[p_Counter].variableName = K15_SEMANTIC_TEXCOORD7_DIMENSION_NAME;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD7_DIMENSION_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_UNIFORM_SEMANTIC_TEXCOORD8_DIMENSION;
	p_SemanticTable->entries[p_Counter].semanticGroupID = K15_UNIFORM_GROUP_PER_MATERIAL;
	p_SemanticTable->entries[p_Counter].variableName = K15_SEMANTIC_TEXCOORD8_DIMENSION_NAME;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD8_DIMENSION_SEMANTIC_NAME;

	return p_Counter;
}
/*********************************************************************************/
intern uint32 K15_InternalAddAttributeSemanticEntries(K15_SemanticTable* p_SemanticTable, uint32 p_Counter)
{
	p_SemanticTable->entries[p_Counter].semanticID = K15_UNIFORM_SEMANTIC_DATA;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_DATA_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_POSITION;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_POSITION_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_NORMAL;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_NORMAL_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_TEXCOORD1;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD1_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_TEXCOORD2;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD2_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_TEXCOORD3;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD3_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_TEXCOORD4;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD4_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_TEXCOORD5;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD5_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_TEXCOORD6;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD6_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_TEXCOORD7;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD7_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_TEXCOORD8;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_TEXCOORD8_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_COLOR1;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_COLOR1_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_COLOR2;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_COLOR2_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_COLOR3;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_COLOR3_SEMANTIC_NAME;

	p_SemanticTable->entries[p_Counter].semanticID = K15_ATTRIBUTE_SEMANTIC_COLOR4;
	p_SemanticTable->entries[p_Counter++].semanticName = K15_SEMANTIC_COLOR4_SEMANTIC_NAME;

	return p_Counter;
}
/*********************************************************************************/
 K15_SemanticTable K15_CreateSemanticTable()
{
	K15_SemanticTable table = {};

	uint32 counter = 0;

	counter = K15_InternalAddPerObjectUniformEntries(&table, counter);
	counter = K15_InternalAddPerMaterialUniformEntries(&table, counter);
	counter = K15_InternalAddAttributeSemanticEntries(&table, counter);
	table.numEntries = counter;

	K15_ASSERT_TEXT(counter < K15_MAX_SHADER_SEMANTICS, "Shader semantic table exceeds '%d' entries (%d entries provided)", K15_MAX_SHADER_SEMANTICS, counter);

	return table;
}
/*********************************************************************************/
K15_TypeTable K15_CreateTypeTable()
{
	K15_TypeTable table = {};

	uint32 counter = 0;

	table.entries[counter].typeID = K15_TYPE_INT;
	table.entries[counter].typeName = K15_TYPE_INT_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(int);

	table.entries[counter].typeID = K15_TYPE_FLOAT;
	table.entries[counter].typeName = K15_TYPE_FLOAT_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float);

	table.entries[counter].typeID = K15_TYPE_FLOAT_VECTOR2;
	table.entries[counter].typeName = K15_TYPE_FLOAT_VECTOR2_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float) * 2;

	table.entries[counter].typeID = K15_TYPE_FLOAT_VECTOR3;
	table.entries[counter].typeName = K15_TYPE_FLOAT_VECTOR3_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float) * 3;

	table.entries[counter].typeID = K15_TYPE_FLOAT_VECTOR4;
	table.entries[counter].typeName = K15_TYPE_FLOAT_VECTOR4_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float) * 4;

	table.entries[counter].typeID = K15_TYPE_FLOAT_MATRIX2;
	table.entries[counter].typeName = K15_TYPE_FLOAT_MATRIX2_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float) * 4;

	table.entries[counter].typeID = K15_TYPE_FLOAT_MATRIX3;
	table.entries[counter].typeName = K15_TYPE_FLOAT_MATRIX3_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float) * 9;

	table.entries[counter].typeID = K15_TYPE_FLOAT_MATRIX4;
	table.entries[counter].typeName = K15_TYPE_FLOAT_MATRIX4_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(float) * 16;

	table.entries[counter].typeID = K15_TYPE_SAMPLER_2D;
	table.entries[counter].typeName = K15_TYPE_SAMPLER_2D_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(int);

	table.entries[counter].typeID = K15_TYPE_TEXTURE_2D;
	table.entries[counter].typeName = K15_TYPE_TEXTURE_2D_NAME;
	table.entries[counter++].typeSizeInBytes = sizeof(int);

	table.numEntries = counter;

	K15_ASSERT_TEXT(counter < K15_MAX_SHADER_TYPES, "Shader type table exceeds '%d' entries (%d entries provided)", K15_MAX_SHADER_TYPES, counter);

	return table;
}
/*********************************************************************************/