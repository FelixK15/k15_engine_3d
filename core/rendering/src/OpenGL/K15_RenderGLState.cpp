/*********************************************************************************/
intern inline uint8 K15_GLSetDepthStateDesc(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderDepthStateDesc* p_DepthStateDesc)
{
	assert(p_DepthStateDesc);

	GLenum depthFunc = K15_GLConvertCompareFunction(p_DepthStateDesc->compareFunction);

	if (p_DepthStateDesc->enabled == TRUE)
	{
		K15_OPENGL_CALL(kglEnable(GL_DEPTH_TEST));
	}
	else
	{
		K15_OPENGL_CALL(kglDisable(GL_DEPTH_TEST));
	}

	K15_OPENGL_CALL(kglDepthFunc(depthFunc));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLSetBlendStateDesc(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderBlendStateDesc* p_BlendStateDesc)
{
	assert(p_BlendStateDesc);

	GLenum srcBlendAlpha = K15_GLConvertBlend(p_BlendStateDesc->sourceBlendFactorAlpha);
	GLenum srcBlendRGB = K15_GLConvertBlend(p_BlendStateDesc->sourceBlendFactorRGB);
	GLenum dstBlendRGB = K15_GLConvertBlend(p_BlendStateDesc->destinationBlendFactorRGB);
	GLenum dstBlendAlpha = K15_GLConvertBlend(p_BlendStateDesc->destinationBlendFactorAlpha);
	GLenum blendOPAlpha = K15_GLConvertBlendOperation(p_BlendStateDesc->blendOperationAlpha);
	GLenum blendOPRGB = K15_GLConvertBlendOperation(p_BlendStateDesc->blendOperationRGB);

	if (p_BlendStateDesc->enabled == TRUE)
	{
		K15_OPENGL_CALL(kglEnable(GL_BLEND));
	}
	else
	{
		K15_OPENGL_CALL(kglDisable(GL_BLEND));
	}

	K15_OPENGL_CALL(kglBlendEquationSeparate(blendOPRGB, blendOPAlpha));
	K15_OPENGL_CALL(kglBlendFuncSeparate(srcBlendRGB, dstBlendRGB, srcBlendAlpha, dstBlendAlpha));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLSetStencilStateDesc(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderStencilStateDesc* p_StencilStateDesc)
{
	assert(p_StencilStateDesc);

	if (p_StencilStateDesc->enabled == TRUE)
	{
		K15_OPENGL_CALL(kglEnable(GL_STENCIL_TEST));
	}
	else
	{
		K15_OPENGL_CALL(kglDisable(GL_STENCIL_TEST));
	}

// 	GLuint stencilMask = 
// 	glStencilMask(p_StencilStateDesc->)

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLSetRasterizerStateDesc(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderRasterizerStateDesc* p_RasterizerStateDesc)
{
	assert(p_RasterizerStateDesc);

	GLenum cullingMode = K15_GLConvertCullingMode(p_RasterizerStateDesc->cullingMode);
	GLenum fillMode = K15_GLConvertFillMode(p_RasterizerStateDesc->fillMode);
	GLenum ordering = K15_GLConvertVertexOrdering(p_RasterizerStateDesc->vertexOrder);

	if (p_RasterizerStateDesc->scissoringEnabled == TRUE)
	{
		K15_OPENGL_CALL(kglEnable(GL_SCISSOR_TEST));
	}
	else
	{
		K15_OPENGL_CALL(kglDisable(GL_SCISSOR_TEST));
	}

	K15_OPENGL_CALL(kglCullFace(cullingMode));
	K15_OPENGL_CALL(kglPolygonMode(GL_FRONT_AND_BACK, fillMode));
	K15_OPENGL_CALL(kglFrontFace(ordering));
	
	return K15_SUCCESS;
}
/*********************************************************************************/