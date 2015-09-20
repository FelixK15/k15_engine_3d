/*********************************************************************************/
intern inline GLenum K15_GLConvertBufferType(K15_RenderBufferType p_BufferType)
{
	GLenum nativeBufferType = GL_INVALID_ENUM;

	switch(p_BufferType)
	{
		case K15_RENDER_BUFFER_TYPE_VERTEX:
		{
			nativeBufferType = GL_ARRAY_BUFFER;
			break;
		}
		case K15_RENDER_BUFFER_TYPE_INDEX:
		{
			nativeBufferType = GL_ELEMENT_ARRAY_BUFFER;	
			break;
		}
		case K15_RENDER_BUFFER_TYPE_UNIFORM:
		{
			nativeBufferType = GL_UNIFORM_BUFFER;	
			break;
		}
		
		default:
		{
			assert(false);
		}
	}

	return nativeBufferType;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertProgramType(K15_RenderProgramType p_ProgramType)
{
	GLenum glProgramType = GL_INVALID_ENUM;

	switch(p_ProgramType)
	{
		case K15_RENDER_PROGRAM_TYPE_VERTEX:
		{
			glProgramType = GL_VERTEX_SHADER;
			break;
		}

		case K15_RENDER_PROGRAM_TYPE_FRAGMENT:
		{
			glProgramType = GL_FRAGMENT_SHADER;
			break;
		}

		case K15_RENDER_PROGRAM_TYPE_GEOMETRY:
		{
			glProgramType = GL_GEOMETRY_SHADER;
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return glProgramType;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertBufferUsage(K15_RenderBufferUsage p_BufferUsage)
{
	GLenum nativeBufferUsage = GL_INVALID_ENUM;

	switch(p_BufferUsage)
	{
		case K15_RENDER_BUFFER_USAGE_STATIC_DRAW:
		{
			nativeBufferUsage = GL_STATIC_DRAW;
			break;
		}

		case K15_RENDER_BUFFER_USAGE_STREAM_DRAW:
		{
			nativeBufferUsage = GL_STREAM_DRAW;	
			break;
		}

		case K15_RENDER_BUFFER_USAGE_DYNAMIC_DRAW:
		{
			nativeBufferUsage = GL_DYNAMIC_DRAW;	
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return nativeBufferUsage;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertCullingMode(K15_CullingMode p_CullingMode)
{
	GLenum nativeCullingMode = GL_INVALID_ENUM;

	switch(p_CullingMode)
	{
		case K15_CULLING_MODE_FRONT:
		{
			nativeCullingMode = GL_FRONT;
			break;
		}

		case K15_CULLING_MODE_BACK:
		{
			nativeCullingMode = GL_BACK;
			break;
		}
	}

	return nativeCullingMode;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertFillMode(K15_FillMode p_FillMode)
{
	GLenum nativeFillMode = GL_INVALID_ENUM;

	switch (p_FillMode)
	{
		case K15_FILLMODE_SOLID:
		{
			nativeFillMode = GL_FILL;
			break;
		}

		case K15_FILLMODE_WIREFRAME:
		{
			nativeFillMode = GL_LINE;
			break;
		}
	}

	return nativeFillMode;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertVertexOrdering(K15_VertexOrder p_VertexOrder)
{
	GLenum nativeVertexOrder = GL_INVALID_ENUM;

	switch(p_VertexOrder)
	{
		case K15_VERTEX_ORDER_CLOCKWISE:
		{
			nativeVertexOrder = GL_CW;
			break;
		}

		case K15_VERTEX_ORDER_COUNTER_CLOCKWISE:
		{
			nativeVertexOrder = GL_CCW;
			break;
		}
	}

	return nativeVertexOrder;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertBlendOperation(K15_BlendOperation p_BlendOperation)
{
	GLenum nativeBlendOperator = GL_INVALID_ENUM;

	switch(p_BlendOperation)
	{
		case K15_BLEND_OPERATION_ADD:
		{
			nativeBlendOperator = GL_FUNC_ADD;
			break;
		}
		
		case K15_BLEND_OPERATION_SUBTRACT:
		{
			nativeBlendOperator = GL_FUNC_SUBTRACT;
			break;
		}

		case K15_BLEND_OPERATION_REVERSE_SUBTRACT:
		{
			nativeBlendOperator = GL_FUNC_REVERSE_SUBTRACT;
			break;
		}

		case K15_BLEND_OPERATION_MAX:
		{
			nativeBlendOperator = GL_MAX;
			break;
		}

		case K15_BLEND_OPERATION_MIN:
		{
			nativeBlendOperator = GL_MIN;
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return nativeBlendOperator;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertBlend(K15_BlendFactor p_Blend)
{
	GLenum nativeBlend = GL_INVALID_ENUM;

	switch(p_Blend)
	{
		case K15_BLEND_FACTOR_ZERO:
		{
			nativeBlend = GL_ZERO;
			break;
		}

		case K15_BLEND_FACTOR_ONE:
		{
			nativeBlend = GL_ONE;
			break;
		}

		case K15_BLEND_FACTOR_SRC_COLOR:
		{
			nativeBlend = GL_SRC_COLOR;
			break;
		}

		case K15_BLEND_FACTOR_INVERSE_SRC_COLOR:
		{
			nativeBlend = GL_ONE_MINUS_SRC_COLOR;
			break;
		}

		case K15_BLEND_FACTOR_DESC_COLOR:
		{
			nativeBlend = GL_DST_COLOR;
			break;
		}

		case K15_BLEND_FACTOR_INVERSE_DESC_COLOR:
		{
			nativeBlend = GL_ONE_MINUS_DST_COLOR;
			break;
		}

		case K15_BLEND_FACTOR_SRC_ALPHA:
		{
			nativeBlend = GL_SRC_ALPHA;
			break;
		}

		case K15_BLEND_FACTOR_INVERSE_SRC_ALPHA:
		{
			nativeBlend = GL_ONE_MINUS_SRC_ALPHA;
			break;
		}

		case K15_BLEND_FACTOR_DEST_ALPHA:
		{
			nativeBlend = GL_DST_ALPHA;
			break;
		}

		case K15_BLEND_FACTOR_INVERSE_DEST_ALPHA:
		{
			nativeBlend = GL_ONE_MINUS_DST_ALPHA;
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return nativeBlend;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertCompareFunction(K15_Comparison p_Comparison)
{
	GLenum nativeCompareFunc = GL_INVALID_ENUM;

	switch(p_Comparison)
	{
		case K15_COMPARISON_ALWAYS:
		{
			nativeCompareFunc = GL_ALWAYS;
			break;
		}

		case K15_COMPARISON_EQUAL:
		{
			nativeCompareFunc = GL_EQUAL;
			break;
		}

		case K15_COMPARISON_GREATER:
		{
			nativeCompareFunc = GL_GREATER;
			break;
		}

		case K15_COMPARISON_GREATER_EQUAL:
		{
			nativeCompareFunc = GL_GEQUAL;
			break;
		}

		case K15_COMPARISON_LESS:
		{
			nativeCompareFunc = GL_LESS;
			break;
		}

		case K15_COMPARISON_LESS_EQUAL:
		{
			nativeCompareFunc = GL_LEQUAL;
			break;
		}

		case K15_COMPARISON_NEVER:
		{
			nativeCompareFunc = GL_NEVER;
			break;
		}

		default:
		{
			assert(false);		
		}
	}

	return nativeCompareFunc;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertUniformTypeToGLType(uint32 p_TypeID)
{
	GLenum nativeUniformType = GL_INVALID_ENUM;

	switch(p_TypeID)
	{
		case K15_TYPE_INT_ID:
// 		case K15_UNIFORM_TYPE_INT16:
// 		case K15_UNIFORM_TYPE_INT32:
 		{
			nativeUniformType = GL_INT;
			break;
		}

// 		case K15_UNIFORM_TYPE_UINT8:
// 		case K15_UNIFORM_TYPE_UINT16:
// 		case K15_UNIFORM_TYPE_UINT32:
// 		{
// 			nativeUniformType = GL_UNSIGNED_INT;
// 			break;
// 		}

		case K15_TYPE_FLOAT_ID:
		{
			nativeUniformType = GL_FLOAT;
			break;
		}

		case K15_TYPE_FLOAT_VECTOR2_ID:
		{
			nativeUniformType = GL_FLOAT_VEC2;
			break;
		}

		case K15_TYPE_FLOAT_VECTOR3_ID:
		{
			nativeUniformType = GL_FLOAT_VEC3;
			break;
		}

		case K15_TYPE_FLOAT_VECTOR4_ID:
		{
			nativeUniformType = GL_FLOAT_VEC4;
			break;
		}

		case K15_TYPE_FLOAT_MATRIX2_ID:
		{
			nativeUniformType = GL_FLOAT_MAT2;
			break;
		}

		case K15_TYPE_FLOAT_MATRIX3_ID:
		{
			nativeUniformType = GL_FLOAT_MAT3;
			break;
		}

		case K15_TYPE_FLOAT_MATRIX4_ID:
		{
			nativeUniformType = GL_FLOAT_MAT4;
			break;
		}

		default:
		{
			assert(false);
		}
	}


	return nativeUniformType;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertTextureType(K15_RenderTextureType p_TextureType)
{
	GLenum nativeTextureFormat = GL_INVALID_ENUM;

	switch(p_TextureType)
	{
		case K15_RENDER_TEXTURE_TYPE_1D:
		{
			nativeTextureFormat = GL_TEXTURE_1D;
			break;
		}

		case K15_RENDER_TEXTURE_TYPE_2D:
		{
			nativeTextureFormat = GL_TEXTURE_2D;
			break;
		}

		case K15_RENDER_TEXTURE_TYPE_3D:
		{
			nativeTextureFormat = GL_TEXTURE_3D;
			break;
		}

		case K15_RENDER_TEXTURE_TYPE_CUBE:
		{
			nativeTextureFormat = GL_TEXTURE_CUBE_MAP;
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return nativeTextureFormat;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertRenderFormatType(K15_RenderFormat p_RenderFormat)
{
	GLenum nativeRenderFormatType = GL_INVALID_ENUM;

	switch(p_RenderFormat)
	{
		case K15_RENDER_FORMAT_R8_INT:
		case K15_RENDER_FORMAT_R8G8_INT:
		case K15_RENDER_FORMAT_R8G8B8_INT:
		case K15_RENDER_FORMAT_R8G8B8A8_INT:
		{
			nativeRenderFormatType = GL_BYTE;
			break;
		}

		case K15_RENDER_FORMAT_R16_INT:
		case K15_RENDER_FORMAT_R16G16_INT:
		case K15_RENDER_FORMAT_R16G16B16_INT:
		case K15_RENDER_FORMAT_R16G16B16A16_INT:
		{
			nativeRenderFormatType = GL_SHORT;
			break;
		}

		case K15_RENDER_FORMAT_R32_INT:
		case K15_RENDER_FORMAT_R32G32_INT:
		case K15_RENDER_FORMAT_R32G32B32_INT:
		case K15_RENDER_FORMAT_R32G32B32A32_INT:
		{
			nativeRenderFormatType = GL_INT;
			break;
		}	

		case K15_RENDER_FORMAT_R8_UINT:
		case K15_RENDER_FORMAT_R8G8_UINT:
		case K15_RENDER_FORMAT_R8G8B8_UINT:
		case K15_RENDER_FORMAT_R8G8B8A8_UINT:
		{
			nativeRenderFormatType = GL_UNSIGNED_BYTE;
			break;
		}

		case K15_RENDER_FORMAT_R16_UINT:
		case K15_RENDER_FORMAT_R16G16_UINT:
		case K15_RENDER_FORMAT_R16G16B16_UINT:
		case K15_RENDER_FORMAT_R16G16B16A16_UINT:
		{
			nativeRenderFormatType = GL_UNSIGNED_SHORT;
			break;
		}

		case K15_RENDER_FORMAT_R32_UINT:
		case K15_RENDER_FORMAT_R32G32_UINT:
		case K15_RENDER_FORMAT_R32G32B32_UINT:
		case K15_RENDER_FORMAT_R32G32B32A32_UINT:
		{
			nativeRenderFormatType = GL_UNSIGNED_INT;
			break;
		}

		case K15_RENDER_FORMAT_R16_FLOAT:
		case K15_RENDER_FORMAT_R16G16_FLOAT:
		case K15_RENDER_FORMAT_R16G16B16_FLOAT:
		case K15_RENDER_FORMAT_R16G16B16A16_FLOAT:
		{
			nativeRenderFormatType = GL_HALF_FLOAT;
		}

		case K15_RENDER_FORMAT_R32_FLOAT:
		case K15_RENDER_FORMAT_R32G32_FLOAT:
		case K15_RENDER_FORMAT_R32G32B32_FLOAT:
		case K15_RENDER_FORMAT_R32G32B32A32_FLOAT:
		{
			nativeRenderFormatType = GL_FLOAT;
			break;
		}
	}

	return nativeRenderFormatType;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertRenderFormat(K15_RenderFormat p_RenderFormat)
{
	GLenum nativeRenderFormat = GL_INVALID_ENUM;

	switch(p_RenderFormat)
	{
		case K15_RENDER_FORMAT_R8_INT:
		case K15_RENDER_FORMAT_R8_UINT:
		case K15_RENDER_FORMAT_R16_INT:
		case K15_RENDER_FORMAT_R16_UINT:
		case K15_RENDER_FORMAT_R16_FLOAT:
		case K15_RENDER_FORMAT_R32_INT:
		case K15_RENDER_FORMAT_R32_UINT:
		case K15_RENDER_FORMAT_R32_FLOAT:
		{
			nativeRenderFormat = GL_RED;
			break;
		}

		case K15_RENDER_FORMAT_R8G8_INT:
		case K15_RENDER_FORMAT_R8G8_UINT:
		case K15_RENDER_FORMAT_R16G16_INT:
		case K15_RENDER_FORMAT_R16G16_UINT:
		case K15_RENDER_FORMAT_R16G16_FLOAT:
		case K15_RENDER_FORMAT_R32G32_INT:
		case K15_RENDER_FORMAT_R32G32_UINT:
		case K15_RENDER_FORMAT_R32G32_FLOAT:
		{
			nativeRenderFormat = GL_RG;
			break;
		}

		case K15_RENDER_FORMAT_R8G8B8_INT:
		case K15_RENDER_FORMAT_R8G8B8_UINT:
		case K15_RENDER_FORMAT_R16G16B16_INT:
		case K15_RENDER_FORMAT_R16G16B16_UINT:
		case K15_RENDER_FORMAT_R16G16B16_FLOAT:
		case K15_RENDER_FORMAT_R32G32B32_INT:
		case K15_RENDER_FORMAT_R32G32B32_UINT:
		case K15_RENDER_FORMAT_R32G32B32_FLOAT:
		case K15_RENDER_FORMAT_RGB_DXT_1:
		{
			nativeRenderFormat = GL_RGB;
			break;
		}

		case K15_RENDER_FORMAT_R8G8B8A8_INT:
		case K15_RENDER_FORMAT_R8G8B8A8_UINT:
		case K15_RENDER_FORMAT_R16G16B16A16_INT:
		case K15_RENDER_FORMAT_R16G16B16A16_UINT:
		case K15_RENDER_FORMAT_R16G16B16A16_FLOAT:
		case K15_RENDER_FORMAT_R32G32B32A32_INT:
		case K15_RENDER_FORMAT_R32G32B32A32_UINT:
		case K15_RENDER_FORMAT_R32G32B32A32_FLOAT:
		case K15_RENDER_FORMAT_RGBA_DXT_1:
		case K15_RENDER_FORMAT_RGBA_DXT_3:
		case K15_RENDER_FORMAT_RGBA_DXT_5:
		{
			nativeRenderFormat = GL_RGBA;
			break;
		}
		
		default:
		{
			assert(false);
		}
	}

	return nativeRenderFormat;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertInternalRenderFormat(K15_RenderFormat p_RenderFormat)
{
	GLenum nativeRenderFormat = GL_INVALID_ENUM;

	switch(p_RenderFormat)
	{
		case K15_RENDER_FORMAT_R8_UINT:
		case K15_RENDER_FORMAT_R8_INT:
		{
			nativeRenderFormat = GL_R8;
			break;
		}

		case K15_RENDER_FORMAT_R16_UINT:
		case K15_RENDER_FORMAT_R16_INT:
		case K15_RENDER_FORMAT_R16_FLOAT:
		{
			nativeRenderFormat = GL_R16;
			break;
		}
		
		case K15_RENDER_FORMAT_R8G8_UINT:
		case K15_RENDER_FORMAT_R8G8_INT:
		{
			nativeRenderFormat = GL_RG8;
			break;
		}

		case K15_RENDER_FORMAT_R16G16_UINT:
		case K15_RENDER_FORMAT_R16G16_INT:
		case K15_RENDER_FORMAT_R16G16_FLOAT:
		{
			nativeRenderFormat = GL_RG16;
			break;
		}

		case K15_RENDER_FORMAT_R8G8B8_INT:
		case K15_RENDER_FORMAT_R8G8B8_UINT:
		{
			nativeRenderFormat = GL_RGB8;
			break;
		}

		case K15_RENDER_FORMAT_R16G16B16_UINT:
		case K15_RENDER_FORMAT_R16G16B16_INT:
		case K15_RENDER_FORMAT_R16G16B16_FLOAT:
		{
			nativeRenderFormat = GL_RGB16;
			break;
		}

		case K15_RENDER_FORMAT_R8G8B8A8_INT:
		case K15_RENDER_FORMAT_R8G8B8A8_UINT:
		{
			nativeRenderFormat = GL_RGBA8;
			break;
		}

		case K15_RENDER_FORMAT_R16G16B16A16_INT:
		case K15_RENDER_FORMAT_R16G16B16A16_UINT:
		case K15_RENDER_FORMAT_R16G16B16A16_FLOAT:
		{
			nativeRenderFormat = GL_RGBA16;
			break;
		}

		case K15_RENDER_FORMAT_RGB_DXT_1:
		{
			nativeRenderFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			break;
		}

		case K15_RENDER_FORMAT_RGBA_DXT_1:
		{
			nativeRenderFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		}

		case K15_RENDER_FORMAT_RGBA_DXT_3:
		{
			nativeRenderFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		}

		case K15_RENDER_FORMAT_RGBA_DXT_5:
		{
			nativeRenderFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		}

		default:
		{
			assert(false);
			break;
		}
	}

	return nativeRenderFormat;
}
/*********************************************************************************/
// intern inline K15_RenderVertexAttributeValueType K15_GLConvertGLTypeToVertexAttributeValueType(GLenum p_GLType, GLint p_TypeSizeInBytes)
// {
// 	K15_RenderVertexAttributeValueType attributeValueType = K15_VERTEX_ATTRIBUTE_VALUE_TYPE_INVALID;
// 
// 	switch(p_GLType)
// 	{
// 	case GL_INT:
// 		{
// 			if (p_TypeSizeInBytes == sizeof(int8))
// 			{
// 				//uniformType = K15_UNIFORM_TYPE_INT8;
// 			}
// 			else if(p_TypeSizeInBytes == sizeof(int16))
// 			{
// 				//uniformType = K15_UNIFORM_TYPE_INT16;
// 			}
// 			else if(p_TypeSizeInBytes == sizeof(int32))
// 			{
// 				//uniformType = K15_UNIFORM_TYPE_INT32;
// 			}
// 			else
// 			{
// 				assert(false);
// 			}
// 
// 			break;
// 		}
// 
// 	case GL_UNSIGNED_INT:
// 		{
// 			if (p_TypeSizeInBytes == sizeof(uint8))
// 			{
// 				//uniformType = K15_UNIFORM_TYPE_UINT8;
// 			}
// 			else if(p_TypeSizeInBytes == sizeof(uint16))
// 			{
// 				//uniformType = K15_UNIFORM_TYPE_UINT16;
// 			}
// 			else if(p_TypeSizeInBytes == sizeof(uint32))
// 			{
// 				//uniformType = K15_UNIFORM_TYPE_UINT32;
// 			}
// 			else
// 			{
// 				assert(false);
// 			}
// 
// 			break;
// 		}
// 
// 	case GL_FLOAT:
// 		{
// 			attributeValueType = K15_VALUE_TYPE_FLOAT;
// 			break;
// 		}
// 
// 	case GL_FLOAT_VEC2:
// 		{
// 			attributeValueType = K15_VALUE_TYPE_FLOAT_VECTOR_2;
// 			break;
// 		}
// 
// 	case GL_FLOAT_VEC3:
// 		{
// 			attributeValueType = K15_VALUE_TYPE_FLOAT_VECTOR_3;
// 			break;
// 		}
// 
// 	case GL_FLOAT_VEC4:
// 		{
// 			attributeValueType = K15_VALUE_TYPE_FLOAT_VECTOR_4;
// 			break;
// 		}
// 
// 	case GL_FLOAT_MAT2:
// 		{
// 			attributeValueType = K15_VALUE_TYPE_FLOAT_MATRIX_2x2;
// 			break;
// 		}
// 
// 	case GL_FLOAT_MAT3:
// 		{
// 			attributeValueType = K15_VALUE_TYPE_FLOAT_MATRIX_3x3;
// 			break;
// 		}
// 
// 	case GL_FLOAT_MAT4:
// 		{
// 			attributeValueType = K15_VALUE_TYPE_FLOAT_MATRIX_4x4;
// 			break;
// 		}
// 
// 	default:
// 		{
// 			assert(false);
// 		}
// 	}
// 
// 	return attributeValueType;
// }
// /*********************************************************************************/
// intern inline K15_UniformType K15_GLConvertGLTypeToUniformType(GLenum p_GLType, GLint p_TypeSizeInBytes)
// {
// 	K15_UniformType uniformType = K15_UNIFORM_TYPE_INVALID;
// 
// 	switch(p_GLType)
// 	{
// 		case GL_INT:
// 		{
// 			if (p_TypeSizeInBytes == sizeof(int8))
// 			{
// 				uniformType = K15_UNIFORM_TYPE_INT8;
// 			}
// 			else if(p_TypeSizeInBytes == sizeof(int16))
// 			{
// 				uniformType = K15_UNIFORM_TYPE_INT16;
// 			}
// 			else if(p_TypeSizeInBytes == sizeof(int32))
// 			{
// 				uniformType = K15_UNIFORM_TYPE_INT32;
// 			}
// 			else
// 			{
// 				assert(false);
// 			}
// 
// 			break;
// 		}
// 
// 		case GL_UNSIGNED_INT:
// 		{
// 			if (p_TypeSizeInBytes == sizeof(uint8))
// 			{
// 				uniformType = K15_UNIFORM_TYPE_UINT8;
// 			}
// 			else if(p_TypeSizeInBytes == sizeof(uint16))
// 			{
// 				uniformType = K15_UNIFORM_TYPE_UINT16;
// 			}
// 			else if(p_TypeSizeInBytes == sizeof(uint32))
// 			{
// 				uniformType = K15_UNIFORM_TYPE_UINT32;
// 			}
// 			else
// 			{
// 				assert(false);
// 			}
// 
// 			break;
// 		}
// 
// 		case GL_FLOAT:
// 		{
// 			uniformType = K15_UNIFORM_TYPE_FLOAT;
// 			break;
// 		}
// 
// 		case GL_FLOAT_VEC2:
// 		{
// 			uniformType = K15_UNIFORM_TYPE_FLOAT2;
// 			break;
// 		}
// 
// 		case GL_FLOAT_VEC3:
// 		{
// 			uniformType = K15_UNIFORM_TYPE_FLOAT3;
// 			break;
// 		}
// 
// 		case GL_FLOAT_VEC4:
// 		{
// 			uniformType = K15_UNIFORM_TYPE_FLOAT4;
// 			break;
// 		}
// 
// 		case GL_FLOAT_MAT2:
// 		{
// 			uniformType = K15_UNIFORM_TYPE_FLOAT2x2;
// 			break;
// 		}
// 
// 		case GL_FLOAT_MAT3:
// 		{
// 			uniformType = K15_UNIFORM_TYPE_FLOAT3x3;
// 			break;
// 		}
// 
// 		case GL_FLOAT_MAT4:
// 		{
// 			uniformType = K15_UNIFORM_TYPE_FLOAT4x4;
// 			break;
// 		}
// 
// 		case GL_SAMPLER_2D:
// 			{
// 				uniformType = K15_UNIFORM_TYPE_SAMPLER_2D;
// 				break;
// 			}
// 
// 		default:
// 		{
// 			assert(false);
// 		}
// 	}
// 
// 	return uniformType;
// }
/*********************************************************************************/
intern inline GLenum K15_GLConvertRenderFilterMode(K15_RenderFilterMode p_FilterMode)
{
	GLenum nativeRenderFilterMode = GL_INVALID_ENUM;
	
	switch(p_FilterMode)
	{
		case K15_RENDER_FILTER_MODE_LINEAR:
		{
			nativeRenderFilterMode = GL_LINEAR;
			break;
		}

		case K15_RENDER_FILTER_MODE_NEAREST:
		{
			nativeRenderFilterMode = GL_NEAREST;
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return nativeRenderFilterMode;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertRenderMinificationFilterMode(K15_RenderFilterMode p_FilterMode)
{
	GLenum nativeRenderFilterMode = GL_INVALID_ENUM;

	switch(p_FilterMode)
	{
	case K15_RENDER_FILTER_MODE_LINEAR:
		{
			nativeRenderFilterMode = GL_LINEAR_MIPMAP_LINEAR;
			break;
		}

	case K15_RENDER_FILTER_MODE_NEAREST:
		{
			nativeRenderFilterMode = GL_NEAREST_MIPMAP_NEAREST;
			break;
		}

	default:
		{
			assert(false);
		}
	}

	return nativeRenderFilterMode;
}
/*********************************************************************************/
intern inline GLenum K15_GLConvertFilterAddressMode(K15_RenderFilterAddressMode p_FilterAddressMode)
{
	GLenum nativeFilterAddressMode = GL_INVALID_ENUM;

	switch(p_FilterAddressMode)
	{
		case K15_RENDER_FILTER_ADDRESS_MODE_CLAMP:
		{
			nativeFilterAddressMode = GL_CLAMP_TO_EDGE;
			break;
		}

		case K15_RENDER_FILTER_ADDRESS_MODE_MIRROR:
		{
			nativeFilterAddressMode = GL_MIRRORED_REPEAT;
			break;
		}

		case K15_RENDER_FILTER_ADDRESS_MODE_REPEAT:
		{
			nativeFilterAddressMode = GL_REPEAT;
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return nativeFilterAddressMode;
}
/*********************************************************************************/
K15_RenderProgramType K15_ConvertGLProgramType(GLenum p_GLProgramType)
{
	K15_RenderProgramType programType = K15_RENDER_PROGRAM_TYPE_COUNT;

	switch (p_GLProgramType)
	{
		case GL_VERTEX_SHADER:
		{
			programType = K15_RENDER_PROGRAM_TYPE_VERTEX;
			break;
		}

		case GL_FRAGMENT_SHADER:
		{
			programType = K15_RENDER_PROGRAM_TYPE_FRAGMENT;
			break;
		}

		case GL_GEOMETRY_SHADER:
		{
			programType = K15_RENDER_PROGRAM_TYPE_GEOMETRY;
			break;
		}

		default:
		{
			assert(false);
			break;
		}
	}

	return programType;
}
/*********************************************************************************/
GLenum K15_GLConvertTopology(K15_RenderTopology p_Topology)
{
	GLenum nativeTopology = GL_INVALID_ENUM;

	switch (p_Topology)
	{
		case K15_RENDER_TOPOLOGY_POINTS:
		{
			nativeTopology = GL_POINTS;
			break;
		}

		case K15_RENDER_TOPOLOGY_LINES:
		{
			nativeTopology = GL_LINES;
			break;
		}

		case K15_RENDER_TOPOLOGY_TRIANGLES:
		{
			nativeTopology = GL_TRIANGLES;
			break;
		}

		case K15_RENDER_TOPOLOGY_TRIANGLE_FAN:
		{
			nativeTopology = GL_TRIANGLE_FAN;
			break;
		}

		case K15_RENDER_TOPOLOGY_TRIANGLE_STRIP:
		{
			nativeTopology = GL_TRIANGLE_STRIP;
			break;
		}
	}

	return nativeTopology;
}
/*********************************************************************************/
GLbitfield K15_GLConvertProgramTypeBit(GLenum p_GLProgramType)
{
	GLbitfield nativeShaderBitField = GL_INVALID_ENUM;

	switch(p_GLProgramType)
	{
		case GL_VERTEX_SHADER:
		{
			nativeShaderBitField = GL_VERTEX_SHADER_BIT;
			break;
		}

		case GL_FRAGMENT_SHADER:
		{
			nativeShaderBitField = GL_FRAGMENT_SHADER_BIT;
			break;
		}

		case GL_GEOMETRY_SHADER:
		{
			nativeShaderBitField = GL_GEOMETRY_SHADER_BIT;
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return nativeShaderBitField;
}
/*********************************************************************************/