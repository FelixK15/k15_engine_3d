/*********************************************************************************/
intern inline uint8 K15_GLCreateRenderTarget(K15_RenderContext* p_RenderContext, K15_RenderTargetDesc* p_RenderTargetDesc, K15_RenderTargetHandle* p_RenderTargetHandle)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	GLuint glFramebuffer = 0;
	GLuint glRenderTexture = 0;
	GLuint glDepthRenderbuffer = 0;
	GLenum glFormat = K15_GLConvertRenderFormat(p_RenderTargetDesc->format);
	GLenum glType = K15_GLConvertRenderFormatType(p_RenderTargetDesc->format);
	uint32 activeViewport = p_RenderContext->activeViewportIndex;

	uint32 width = p_RenderContext->viewports[activeViewport].width;
	uint32 height = p_RenderContext->viewports[activeViewport].height;

	K15_GLRenderTarget* glRenderTargetStruct = &glContext->gl3.renderTargets[*p_RenderTargetHandle];

	glRenderTargetStruct->textureCount = 0;
	glRenderTargetStruct->glFramebuffer = GL_INVALID_VALUE;
	glRenderTargetStruct->glRenderbuffer = GL_INVALID_VALUE;

	float renderTargetFactor = 1.0f;

	if (p_RenderTargetDesc->dimension == K15_RENDER_TARGET_DIMENSION_HALF_RESOLUTION)
	{
		renderTargetFactor = 0.5f;
	}
	else if (p_RenderTargetDesc->dimension == K15_RENDER_TARGET_DIMENSION_QUARTER_RESOLUTION)
	{
		renderTargetFactor = 0.25f;
	}

	uint32 renderTargetWidth = (uint32)((float)width * renderTargetFactor);
	uint32 renderTargetHeight = (uint32)((float)height * renderTargetFactor);

	uint32 maxColorOutput = p_RenderContext->capabilities.maxRenderTargets;
	uint32 actualColorOutputCount = 0;

	GLenum* glDrawBuffers = (GLenum*)alloca(sizeof(GLenum) * maxColorOutput); //+2 for depth and stencil buffer
	
	//create & bind framebuffer
	K15_OPENGL_CALL(kglGenFramebuffers(1, &glFramebuffer));
	K15_OPENGL_CALL(kglBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer));

	GLenum attachment = GL_COLOR_ATTACHMENT0;

	//Check each individual bit for attachment
	for (uint32 renderOutputIndex = 0;
		renderOutputIndex < maxColorOutput;
		++renderOutputIndex, ++attachment)
	{
		uint32 outputBit = (1 << renderOutputIndex);

		if ((p_RenderTargetDesc->outputFlags & outputBit) > 0)
		{
			GLuint glCurrentTextureBinding = 0;

			K15_OPENGL_CALL(glGenTextures(1, &glCurrentTextureBinding));
			K15_OPENGL_CALL(kglTextureImage2DEXT(glCurrentTextureBinding, GL_TEXTURE_2D, 0, glFormat, renderTargetWidth, renderTargetHeight, 0, glFormat, glType, 0));
		
			K15_OPENGL_CALL(kglFramebufferTexture(GL_FRAMEBUFFER, attachment, glCurrentTextureBinding, 0));

			glRenderTargetStruct->glTextures[actualColorOutputCount] = glCurrentTextureBinding;
			glDrawBuffers[actualColorOutputCount++] = attachment;
		}
	}

	if ((p_RenderTargetDesc->outputFlags & K15_RENDER_TARGET_OUTPUT_DEPTH_BIT) > 0
		&& (p_RenderTargetDesc->outputFlags & K15_RENDER_TARGET_OUTPUT_STENCIL_BIT) > 0)
	{
		GLuint glDepthStencilRenderbuffer = 0;

		K15_OPENGL_CALL(kglGenRenderbuffers(1, &glDepthStencilRenderbuffer));
		K15_OPENGL_CALL(kglBindRenderbuffer(GL_RENDERBUFFER, glDepthStencilRenderbuffer));
		K15_OPENGL_CALL(kglRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, renderTargetWidth, renderTargetHeight));
		K15_OPENGL_CALL(kglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, glDepthStencilRenderbuffer));
	
		glRenderTargetStruct->glRenderbuffer = glDepthStencilRenderbuffer;
	}
	else if ((p_RenderTargetDesc->outputFlags & K15_RENDER_TARGET_OUTPUT_DEPTH_BIT) > 0)
	{ 
		GLuint glDepthRenderbuffer = 0;

		K15_OPENGL_CALL(kglGenRenderbuffers(1, &glDepthRenderbuffer));
		K15_OPENGL_CALL(kglBindRenderbuffer(GL_RENDERBUFFER, glDepthRenderbuffer));
		K15_OPENGL_CALL(kglRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, renderTargetWidth, renderTargetHeight));
		K15_OPENGL_CALL(kglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, glDepthRenderbuffer));
	
		glRenderTargetStruct->glRenderbuffer = glDepthRenderbuffer;
	}
	else if ((p_RenderTargetDesc->outputFlags & K15_RENDER_TARGET_OUTPUT_STENCIL_BIT) > 0)
	{
		GLuint glStencilRenderbuffer = 0;

		K15_OPENGL_CALL(kglGenRenderbuffers(1, &glStencilRenderbuffer));
		K15_OPENGL_CALL(kglBindRenderbuffer(GL_RENDERBUFFER, glStencilRenderbuffer));
		K15_OPENGL_CALL(kglRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, renderTargetWidth, renderTargetHeight));
		K15_OPENGL_CALL(kglFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, glStencilRenderbuffer));

		glRenderTargetStruct->glRenderbuffer = glStencilRenderbuffer;
	}

	GLenum frameBufferStatus = GL_INVALID_ENUM;

	K15_OPENGL_CALL(frameBufferStatus = kglCheckFramebufferStatus(GL_FRAMEBUFFER));
	
	if (frameBufferStatus == GL_FRAMEBUFFER_COMPLETE)
	{
		K15_OPENGL_CALL(kglDrawBuffers(actualColorOutputCount, glDrawBuffers));
	}

	glRenderTargetStruct->glFramebuffer = glFramebuffer;
	glRenderTargetStruct->textureCount = actualColorOutputCount;
	//bind backbuffer
	K15_OPENGL_CALL(kglBindFramebuffer(GL_FRAMEBUFFER, 0));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLDeleteRenderTarget(K15_RenderContext* p_RenderContext, K15_RenderTargetHandle* p_RenderTargetHandle)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	K15_GLRenderTarget* glRenderTargetStruct = &glContext->gl3.renderTargets[*p_RenderTargetHandle];

	K15_OPENGL_CALL(glDeleteTextures(glRenderTargetStruct->textureCount, glRenderTargetStruct->glTextures));
	
	if(glRenderTargetStruct->glRenderbuffer != GL_INVALID_VALUE)
	{
		K15_OPENGL_CALL(kglDeleteRenderbuffers(1, &glRenderTargetStruct->glRenderbuffer));
	}

	K15_OPENGL_CALL(kglDeleteFramebuffers(1, &glRenderTargetStruct->glFramebuffer));

	return K15_SUCCESS;
}
/*********************************************************************************/