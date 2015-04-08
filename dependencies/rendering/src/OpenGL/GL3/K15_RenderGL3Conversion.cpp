/*********************************************************************************/
inline GLenum K15_GLConvertBufferType(K15_RenderBufferType p_BufferType)
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
inline GLenum K15_GLConvertProgramType(K15_RenderProgramType p_ProgramType)
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
inline GLenum K15_GLConvertBufferUsage(K15_RenderBufferUsage p_BufferUsage)
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
GLenum K15_GLConvertCullingMode(K15_CullingMode p_CullingMode)
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
GLenum K15_GLConvertFillMode(K15_FillMode p_FillMode)
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
GLenum K15_GLConvertVertexOrdering(K15_VertexOrder p_VertexOrder)
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
GLenum K15_GLConvertBlendOperation(K15_BlendOperation p_BlendOperation)
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
GLenum K15_GLConvertBlend(K15_Blend p_Blend)
{
	GLenum nativeBlend = GL_INVALID_ENUM;

	switch(p_Blend)
	{
		case K15_BLEND_ZERO:
		{
			nativeBlend = GL_ZERO;
			break;
		}

		case K15_BLEND_ONE:
		{
			nativeBlend = GL_ONE;
			break;
		}

		case K15_BLEND_SRC_COLOR:
		{
			nativeBlend = GL_SRC_COLOR;
			break;
		}

		case K15_BLEND_INVERSE_SRC_COLOR:
		{
			nativeBlend = GL_ONE_MINUS_SRC_COLOR;
			break;
		}

		case K15_BLEND_DESC_COLOR:
		{
			nativeBlend = GL_DST_COLOR;
			break;
		}

		case K15_BLEND_INVERSE_DESC_COLOR:
		{
			nativeBlend = GL_ONE_MINUS_DST_COLOR;
			break;
		}

		case K15_BLEND_SRC_ALPHA:
		{
			nativeBlend = GL_SRC_ALPHA;
			break;
		}

		case K15_BLEND_INVERSE_SRC_ALPHA:
		{
			nativeBlend = GL_ONE_MINUS_SRC_ALPHA;
			break;
		}

		case K15_BLEND_DEST_ALPHA:
		{
			nativeBlend = GL_DST_ALPHA;
			break;
		}

		case K15_BLEND_INVERSE_DEST_ALPHA:
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
GLenum K15_GLConvertCompareFunction(K15_Comparison p_Comparison)
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
GLenum K15_GLConvertUniformTypeToGLType(K15_UniformType p_UniformType)
{
	GLenum nativeUniformType = GL_INVALID_ENUM;

	switch(p_UniformType)
	{
		case K15_UNIFORM_TYPE_INT8:
		case K15_UNIFORM_TYPE_INT16:
		case K15_UNIFORM_TYPE_INT32:
		{
			nativeUniformType = GL_INT;
			break;
		}

		case K15_UNIFORM_TYPE_UINT8:
		case K15_UNIFORM_TYPE_UINT16:
		case K15_UNIFORM_TYPE_UINT32:
		{
			nativeUniformType = GL_UNSIGNED_INT;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT:
		{
			nativeUniformType = GL_FLOAT;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT2:
		{
			nativeUniformType = GL_FLOAT_VEC2;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT3:
		{
			nativeUniformType = GL_FLOAT_VEC3;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT4:
		{
			nativeUniformType = GL_FLOAT_VEC4;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT2x2:
		{
			nativeUniformType = GL_FLOAT_MAT2;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT3x3:
		{
			nativeUniformType = GL_FLOAT_MAT3;
			break;
		}

		case K15_UNIFORM_TYPE_FLOAT4x4:
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
GLenum K15_GLConvertTextureType(K15_RenderTextureType p_TextureType)
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
GLenum K15_GLConvertRenderFormatType(K15_RenderFormat p_RenderFormat)
{
	GLenum nativeRenderFormatType = GL_INVALID_ENUM;

	switch(p_RenderFormat)
	{
		case K15_RENDER_FORMAT_R8G8B8A8_INT:
		case K15_RENDER_FORMAT_R8G8B8_INT:
		{
			nativeRenderFormatType = GL_BYTE;
			break;
		}

		case K15_RENDER_FORMAT_R16G16B16A16_INT:
		case K15_RENDER_FORMAT_R16G16B16_INT:
		{
			nativeRenderFormatType = GL_SHORT;
			break;
		}

		case K15_RENDER_FORMAT_R32G32B32_INT:
		case K15_RENDER_FORMAT_R32G32B32A32_INT:
		{
			nativeRenderFormatType = GL_INT;
			break;
		}	

		case K15_RENDER_FORMAT_R8G8B8_UINT:
		case K15_RENDER_FORMAT_R8G8B8A8_UINT:
		{
			nativeRenderFormatType = GL_UNSIGNED_BYTE;
			break;
		}

		case K15_RENDER_FORMAT_R16G16B16_UINT:
		case K15_RENDER_FORMAT_R16G16B16A16_UINT:
		{
			nativeRenderFormatType = GL_UNSIGNED_SHORT;
			break;
		}

		case K15_RENDER_FORMAT_R32G32B32_UINT:
		case K15_RENDER_FORMAT_R32G32B32A32_UINT:
		{
			nativeRenderFormatType = GL_UNSIGNED_INT;
			break;
		}

		case K15_RENDER_FORMAT_R16G16B16_FLOAT:
		case K15_RENDER_FORMAT_R16G16B16A16_FLOAT:
		{
			nativeRenderFormatType = GL_HALF_FLOAT;
		}

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
GLenum K15_GLConvertRenderFormat(K15_RenderFormat p_RenderFormat)
{
	GLenum nativeRenderFormat = GL_INVALID_ENUM;

	switch(p_RenderFormat)
	{
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
GLenum K15_GLConvertInternalRenderFormat(K15_RenderFormat p_RenderFormat)
{
	GLenum nativeRenderFormat = GL_INVALID_ENUM;

	switch(p_RenderFormat)
	{
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
K15_UniformType K15_GLConvertGLTypeToUniformType(GLenum p_GLType, GLint p_TypeSizeInBytes)
{
	K15_UniformType uniformType = K15_UNIFORM_TYPE_INVALID;

	switch(p_GLType)
	{
		case GL_INT:
		{
			if (p_TypeSizeInBytes == sizeof(int8))
			{
				uniformType = K15_UNIFORM_TYPE_INT8;
			}
			else if(p_TypeSizeInBytes == sizeof(int16))
			{
				uniformType = K15_UNIFORM_TYPE_INT16;
			}
			else if(p_TypeSizeInBytes == sizeof(int32))
			{
				uniformType = K15_UNIFORM_TYPE_INT32;
			}
			else
			{
				assert(false);
			}

			break;
		}

		case GL_UNSIGNED_INT:
		{
			if (p_TypeSizeInBytes == sizeof(uint8))
			{
				uniformType = K15_UNIFORM_TYPE_UINT8;
			}
			else if(p_TypeSizeInBytes == sizeof(uint16))
			{
				uniformType = K15_UNIFORM_TYPE_UINT16;
			}
			else if(p_TypeSizeInBytes == sizeof(uint32))
			{
				uniformType = K15_UNIFORM_TYPE_UINT32;
			}
			else
			{
				assert(false);
			}

			break;
		}

		case GL_FLOAT:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT;
			break;
		}

		case GL_FLOAT_VEC2:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT2;
			break;
		}

		case GL_FLOAT_VEC3:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT3;
			break;
		}

		case GL_FLOAT_VEC4:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT4;
			break;
		}

		case GL_FLOAT_MAT2:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT2x2;
			break;
		}

		case GL_FLOAT_MAT3:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT3x3;
			break;
		}

		case GL_FLOAT_MAT4:
		{
			uniformType = K15_UNIFORM_TYPE_FLOAT4x4;
			break;
		}

		default:
		{
			assert(false);
		}
	}

	return uniformType;
}
/*********************************************************************************/