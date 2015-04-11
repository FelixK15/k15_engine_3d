#define K15_RENDER_GL_MAX_SAMPLER_COUNT 32

intern GLuint internOpenGLSampler[K15_RENDER_GL_MAX_SAMPLER_COUNT] = {0};

/*********************************************************************************/
intern uint8 K15_GLCreateSampler(K15_RenderContext* p_RenderContext, K15_RenderSamplerDesc* p_RenderSamplerDesc, K15_RenderSamplerHandle* p_RenderSamplerHandle)
{
	GLuint glSamplerHandle;

	GLenum minifactionFilter = K15_GLConvertRenderFilterMode(p_RenderSamplerDesc->filtering.minification);
	GLenum magnificationFilter = K15_GLConvertRenderFilterMode(p_RenderSamplerDesc->filtering.magnification);

	GLenum UAdressing = K15_GLConvertFilterAddressMode(p_RenderSamplerDesc->address.u);
	GLenum VAdressing = K15_GLConvertFilterAddressMode(p_RenderSamplerDesc->address.v);
	GLenum WAdressing = K15_GLConvertFilterAddressMode(p_RenderSamplerDesc->address.w);

	K15_OPENGL_CALL(kglGenSamplers(1, &glSamplerHandle));

	GLfloat maxAnisotropy = (GLfloat)p_RenderContext->capabilities.maxAnisotropy;

	//special case: anisotropic filtering
	if (p_RenderSamplerDesc->filtering.magnification == K15_RENDER_FILTER_MODE_ANISOTROPIC 
		|| p_RenderSamplerDesc->filtering.minification == K15_RENDER_FILTER_MODE_ANISOTROPIC)
	{
		K15_OPENGL_CALL(kglSamplerParameterf(glSamplerHandle, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy));
	}
	else
	{
		K15_OPENGL_CALL(kglSamplerParameteri(glSamplerHandle, GL_TEXTURE_MIN_FILTER, minifactionFilter));
		K15_OPENGL_CALL(kglSamplerParameteri(glSamplerHandle, GL_TEXTURE_MAG_FILTER, magnificationFilter));
	}

	K15_OPENGL_CALL(kglSamplerParameteri(glSamplerHandle, GL_TEXTURE_WRAP_S, UAdressing));
	K15_OPENGL_CALL(kglSamplerParameteri(glSamplerHandle, GL_TEXTURE_WRAP_T, VAdressing));
	K15_OPENGL_CALL(kglSamplerParameteri(glSamplerHandle, GL_TEXTURE_WRAP_R, WAdressing));

	internOpenGLSampler[*p_RenderSamplerHandle] = glSamplerHandle;

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_GLDeleteSampler(K15_RenderContext* p_RenderContext, K15_RenderSamplerHandle* p_RenderSamplerHandle)
{
	GLuint glSamplerHandle = internOpenGLSampler[*p_RenderSamplerHandle];

	K15_OPENGL_CALL(kglDeleteSamplers(1, &glSamplerHandle));

	return K15_SUCCESS;
}
/*********************************************************************************/