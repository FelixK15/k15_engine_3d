/*********************************************************************************/
K15_AutoUniformType K15_ConvertToAutoType(char* p_UniformName)
{
	K15_AutoUniformType autoType = K15_AUTO_UNIFORM_TYPE_UNKNOWN;

	if (strcmp(p_UniformName, "projMatrix") == 0)
	{
		autoType = K15_AUTO_UNIFORM_TYPE_PROJECTION_MATRIX;
	}
	else if (strcmp(p_UniformName, "worldMatrix") == 0)
	{
		autoType = K15_AUTO_UNIFORM_TYPE_WORLD_MATRIX;
	}
	else if (strcmp(p_UniformName, "viewMatrix") == 0)
	{
		autoType = K15_AUTO_UNIFORM_TYPE_VIEW_MATRIX;
	}

	return autoType;
}
/*********************************************************************************/
K15_AutoUniformTypeGroupFlag K15_ConvertToAutoTypeGroup(K15_AutoUniformType p_UniformAutoType)
{
	K15_AutoUniformTypeGroupFlag autoTypeGroup = K15_AUTO_UNIFORM_TYPE_GROUP_PER_MATERIAL;

	switch (p_UniformAutoType)
	{
		case K15_AUTO_UNIFORM_TYPE_WORLD_MATRIX:
		{
			autoTypeGroup = K15_AUTO_UNIFORM_TYPE_GROUP_PER_MESH;
			break;
		}

		case K15_AUTO_UNIFORM_TYPE_PROJECTION_MATRIX:
		case K15_AUTO_UNIFORM_TYPE_VIEW_MATRIX:
		{
			autoTypeGroup = K15_AUTO_UNIFORM_TYPE_GROUP_PER_CAMERA;	
			break;
		}

		default:
		{
			autoTypeGroup = K15_AUTO_UNIFORM_TYPE_GROUP_PER_MATERIAL;
			break;
		}
	}

	return autoTypeGroup;
}
/*********************************************************************************/
uint32 K15_ConvertToUniformByteSize(K15_UniformType p_UniformType)
{
	uint32 sizeInBytes = 0;

	switch(p_UniformType)
	{
		case K15_UNIFORM_TYPE_UINT8:
		case K15_UNIFORM_TYPE_INT8:
		{
			sizeInBytes = 1;
			break;
		}

		case K15_UNIFORM_TYPE_UINT16:
		case K15_UNIFORM_TYPE_INT16:
		{
			sizeInBytes = 2;
			break;
		}

		case K15_UNIFORM_TYPE_UINT32:
		case K15_UNIFORM_TYPE_INT32:
		case K15_UNIFORM_TYPE_FLOAT:
		{
			sizeInBytes = 4;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT2:
		{
			sizeInBytes = 8;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT2x2:
		case K15_UNIFORM_TYPE_FLOAT4:
		{
			sizeInBytes = 16;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT3:
		{
			sizeInBytes = 12;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT3x3:
		{
			sizeInBytes = 36;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT4x4:
		{
			sizeInBytes = 64;
			break;
		}

		default:
		{
			assert(false);
			break;
		}


	}

	return sizeInBytes;
}
/*********************************************************************************/