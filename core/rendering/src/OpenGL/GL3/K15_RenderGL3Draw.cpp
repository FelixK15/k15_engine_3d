/*********************************************************************************/
intern uint8 K15_GLDrawFullscreenQuad(K15_RenderContext* p_RenderContext, K15_RenderProgramHandle* p_RenderProgramHandle)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	K15_GLProgram* glProgram = &glContext->glObjects.programs[*p_RenderProgramHandle];

	//Get rasterizer state & enable counter clockwise vertex order to draw quad properly
	K15_RenderRasterizerStateDesc* rasterizerState = p_RenderContext->renderState.rasterizerStateDesc;
	
	GLuint glPreviousProgramPipeline = glContext->gl3.programPipelineHandle;
	GLuint glFragmentShaderHandle = glProgram->program;

	GLuint glQuadVBO = glContext->gl3.fullscreenQuadVBO;
	GLuint glQuadIBO = glContext->gl3.fullscreenQuadIBO;
	GLuint glQuadVAO = glContext->gl3.fullscreenQuadVAO;
	GLuint glQuadPPL = glContext->gl3.fullscreenQuadPPL;

	GLuint glPreviousArrayBuffer = 0;
	GLuint glPreviousElementArrayBuffer = 0;
	GLuint glPreviousVAO = glContext->gl3.vertexArrayHandle;

	GLenum previousVertexOrder = K15_GLConvertVertexOrdering(rasterizerState->vertexOrder);

#ifdef K15_OPENGL_ENABLE_QUAD_BUFFER_BACKUP
	K15_OPENGL_CALL(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&glPreviousArrayBuffer));
	K15_OPENGL_CALL(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)&glPreviousElementArrayBuffer));
#endif //K15_OPENGL_ENABLE_QUAD_BUFFER_BACKUP

	if (previousVertexOrder != GL_CW)
	{
		K15_OPENGL_CALL(kglFrontFace(GL_CW));
	}

	//Set state for quad
	K15_OPENGL_CALL(kglBindVertexArray(glQuadVAO));
	K15_OPENGL_CALL(kglBindProgramPipeline(glQuadPPL));
	K15_OPENGL_CALL(kglUseProgramStages(glQuadPPL, GL_FRAGMENT_SHADER_BIT, glFragmentShaderHandle));

	//Bind Vertex & Index buffer
	K15_OPENGL_CALL(kglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glQuadIBO));
	K15_OPENGL_CALL(kglBindBuffer(GL_ARRAY_BUFFER, glQuadVBO));
	K15_OPENGL_CALL(kglDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0));
	K15_OPENGL_CALL(kglBindVertexArray(glPreviousVAO));
	K15_OPENGL_CALL(kglBindProgramPipeline(glPreviousProgramPipeline));

	if (previousVertexOrder != GL_CW)
	{
		K15_OPENGL_CALL(kglFrontFace(previousVertexOrder));
	}

#ifdef K15_OPENGL_ENABLE_QUAD_BUFFER_BACKUP
	K15_OPENGL_CALL(kglBindBuffer(GL_ARRAY_BUFFER, glPreviousArrayBuffer));
	K15_OPENGL_CALL(kglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glPreviousElementArrayBuffer));
#endif //K15_OPENGL_ENABLE_QUAD_BUFFER_BACKUP

	return K15_SUCCESS;
}
/*********************************************************************************/
intern inline uint8 K15_GLDrawIndexed(K15_RenderContext* p_RenderContext, uint32 p_IndexCount, uint32 p_IndexOffset)
{
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderContext->userData;

	//get buffer access for vertex and index buffer
	//K15_GLBufferAccessData* vertexAccessData = glContext->glBoundObjects.currentBufferAccess[K15_RENDER_BUFFER_TYPE_VERTEX];
	//K15_GLBufferAccessData* indexAccessData = glContext->glBoundObjects.currentBufferAccess[K15_RENDER_BUFFER_TYPE_INDEX];

	K15_GLBuffer* vertexBuffer = glContext->glBoundObjects.boundBuffers[K15_RENDER_BUFFER_TYPE_VERTEX];
	K15_GLBuffer* indexBuffer = glContext->glBoundObjects.boundBuffers[K15_RENDER_BUFFER_TYPE_INDEX];

	K15_GLProgram* boundVertexProgram = glContext->glBoundObjects.boundPrograms[K15_RENDER_PROGRAM_TYPE_VERTEX];

	GLint vsPositionRegister = 0;
	GLuint glProgramHandle = boundVertexProgram->program;

	K15_OPENGL_CALL(vsPositionRegister = kglGetAttribLocation(glProgramHandle, "position"));
	K15_OPENGL_CALL(kglEnableVertexAttribArray(vsPositionRegister));
	K15_OPENGL_CALL(kglVertexAttribPointer(vsPositionRegister, 3, GL_FLOAT, GL_FALSE, vertexBuffer->singleElementSizeInBytes, 0));
	K15_OPENGL_CALL(kglDrawElements(GL_TRIANGLES, p_IndexCount, GL_UNSIGNED_SHORT, 0));

	return K15_SUCCESS;
}
/*********************************************************************************/
intern uint8 K15_GLDrawMeshDesc(K15_RenderContext* p_RenderContext, K15_RenderMeshDesc* p_RenderMeshDesc)
{

}
/*********************************************************************************/