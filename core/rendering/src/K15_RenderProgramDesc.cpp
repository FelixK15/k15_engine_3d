/*********************************************************************************/
// uint32 K15_ConvertToUniformByteSize(K15_UniformType p_UniformType)
// {
// 	uint32 sizeInBytes = 0;
// 
// 	switch(p_UniformType)
// 	{
// 		case K15_UNIFORM_TYPE_UINT8:
// 		case K15_UNIFORM_TYPE_INT8:
// 		{
// 			sizeInBytes = 1;
// 			break;
// 		}
// 
// 		case K15_UNIFORM_TYPE_UINT16:
// 		case K15_UNIFORM_TYPE_INT16:
// 		{
// 			sizeInBytes = 2;
// 			break;
// 		}
// 
// 		case K15_UNIFORM_TYPE_UINT32:
// 		case K15_UNIFORM_TYPE_INT32:
// 		case K15_UNIFORM_TYPE_FLOAT:
// 		{
// 			sizeInBytes = 4;
// 			break;
// 		}
// 
// 		case K15_UNIFORM_TYPE_FLOAT2:
// 		{
// 			sizeInBytes = 8;
// 			break;
// 		}
// 
// 		case K15_UNIFORM_TYPE_FLOAT2x2:
// 		case K15_UNIFORM_TYPE_FLOAT4:
// 		{
// 			sizeInBytes = 16;
// 			break;
// 		}
// 
// 		case K15_UNIFORM_TYPE_FLOAT3:
// 		{
// 			sizeInBytes = 12;
// 			break;
// 		}
// 
// 		case K15_UNIFORM_TYPE_FLOAT3x3:
// 		{
// 			sizeInBytes = 36;
// 			break;
// 		}
// 
// 		case K15_UNIFORM_TYPE_FLOAT4x4:
// 		{
// 			sizeInBytes = 64;
// 			break;
// 		}
// 
// 		default:
// 		{
// 			assert(false);
// 			break;
// 		}
// 
// 
// 	}
// 
// 	return sizeInBytes;
//}
/*********************************************************************************/
const char* K15_GetMainFunctionIdentifierForProgramType(K15_RenderProgramType p_ProgramType)
{
	const char* mainIdentifier = 0;

	switch(p_ProgramType)
	{
		case K15_RENDER_PROGRAM_TYPE_FRAGMENT:
		{
			mainIdentifier = "ShadeFragment";
			break;
		}

		case K15_RENDER_PROGRAM_TYPE_VERTEX:
		{
			mainIdentifier = "TransformVertex";
			break;
		}
	}

	return mainIdentifier;
}
/*********************************************************************************/