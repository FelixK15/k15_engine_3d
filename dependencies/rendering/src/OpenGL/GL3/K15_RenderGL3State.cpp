/*********************************************************************************/
intern inline uint8 K15_GLSetDepthStateDesc(K15_RenderContext* p_RenderContext, K15_RenderDepthStateDesc* p_DepthStateDesc)
{
	assert(p_DepthStateDesc);

	GLenum depthFunc = K15_GLConvertCompareFunction(p_DepthStateDesc->compareFunction);

	if (p_DepthStateDesc->enabled == TRUE)
	{
		K15_OPENGL_CALL(glEnable(GL_DEPTH_TEST));
	}
	else
	{
		K15_OPENGL_CALL(glDisable(GL_DEPTH_TEST));
	}

	K15_OPENGL_CALL(glDepthFunc(depthFunc));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLSetBlendStateDesc(K15_RenderContext* p_RenderContext, K15_RenderBlendStateDesc* p_BlendStateDesc)
{
	assert(p_BlendStateDesc);

	GLenum srcBlendAlpha = K15_GLConvertBlend(p_BlendStateDesc->sourceBlendAlpha);
	GLenum srcBlendRGB = K15_GLConvertBlend(p_BlendStateDesc->sourceBlendRGB);
	GLenum dstBlendRGB = K15_GLConvertBlend(p_BlendStateDesc->destinationBlendRGB);
	GLenum dstBlendAlpha = K15_GLConvertBlend(p_BlendStateDesc->destinationBlendAlpha);
	GLenum blendOPAlpha = K15_GLConvertBlendOperation(p_BlendStateDesc->blendOperationAlpha);
	GLenum blendOPRGB = K15_GLConvertBlendOperation(p_BlendStateDesc->blendOperationRGB);

	if (p_BlendStateDesc->enabled == TRUE)
	{
		K15_OPENGL_CALL(glEnable(GL_BLEND));
	}
	else
	{
		K15_OPENGL_CALL(glDisable(GL_BLEND));
	}

	K15_OPENGL_CALL(kglBlendEquationSeparate(blendOPRGB, blendOPAlpha));
	K15_OPENGL_CALL(kglBlendFuncSeparate(srcBlendRGB, dstBlendRGB, srcBlendAlpha, dstBlendAlpha));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLSetStencilStateDesc(K15_RenderContext* p_RenderContext, K15_RenderStencilStateDesc* p_StencilStateDesc)
{
	assert(p_StencilStateDesc);

	if (p_StencilStateDesc->enabled == TRUE)
	{
		K15_OPENGL_CALL(glEnable(GL_STENCIL_TEST));
	}
	else
	{
		K15_OPENGL_CALL(glDisable(GL_STENCIL_TEST));
	}

// 	GLuint stencilMask = 
// 	glStencilMask(p_StencilStateDesc->)

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLSetRasterizerStateDesc(K15_RenderContext* p_RenderContext, K15_RenderRasterizerStateDesc* p_RasterizerStateDesc)
{
	assert(p_RasterizerStateDesc);

	GLenum cullingMode = K15_GLConvertCullingMode(p_RasterizerStateDesc->cullingMode);
	GLenum fillMode = K15_GLConvertFillMode(p_RasterizerStateDesc->fillMode);
	GLenum ordering = K15_GLConvertVertexOrdering(p_RasterizerStateDesc->vertexOrder);

	if (p_RasterizerStateDesc->scissoringEnabled == TRUE)
	{
		K15_OPENGL_CALL(glEnable(GL_SCISSOR_TEST));
	}
	else
	{
		K15_OPENGL_CALL(glDisable(GL_SCISSOR_TEST));
	}

	K15_OPENGL_CALL(glCullFace(cullingMode));
	K15_OPENGL_CALL(glPolygonMode(GL_FRONT_AND_BACK, fillMode));
	K15_OPENGL_CALL(glFrontFace(ordering));

	GLclampf clearColor[] = {
		p_RasterizerStateDesc->clearColor.r,
		p_RasterizerStateDesc->clearColor.g,
		p_RasterizerStateDesc->clearColor.b
	};

	//clamp color values
	for (uint32 colorChannelIndex = 0;
		 colorChannelIndex < K15_ARRAY_COUNT(clearColor);
		 ++colorChannelIndex)
	{
		if (clearColor[colorChannelIndex] < 0.f)
		{
			clearColor[colorChannelIndex] = 0.0f;
		}
		else if(clearColor[colorChannelIndex] > 1.0f)
		{
			clearColor[colorChannelIndex] = 1.0f;
		}
	}

	K15_OPENGL_CALL(glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.f));
	
	return K15_SUCCESS;
}
/*********************************************************************************/