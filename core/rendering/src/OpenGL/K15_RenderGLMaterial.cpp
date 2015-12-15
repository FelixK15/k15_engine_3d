/*********************************************************************************/
result8 K15_GLBindMaterialPass(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderMaterialPassDesc* p_MaterialPassDesc)
{
	result8 result = K15_SUCCESS;
	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;
	K15_ShaderProcessorContext* shaderProcessorContext = p_RenderBackEnd->shaderProcessorContext;

#ifdef K15_TOLERATE_INVALID_GPU_HANDLES
	if (*p_MaterialPassDesc->vertexShaderHandle == K15_INVALID_GPU_RESOURCE_HANDLE ||
		*p_MaterialPassDesc->fragmentShaderHandle == K15_INVALID_GPU_RESOURCE_HANDLE)
	{
		return K15_SUCCESS;
	}
#endif //K15_TOLERATE_INVALID_GPU_HANDLES

	K15_GLBindProgram(p_RenderBackEnd, p_MaterialPassDesc->vertexShaderHandle);
	K15_GLBindProgram(p_RenderBackEnd, p_MaterialPassDesc->fragmentShaderHandle);
	K15_GLUpdateDirtyUniforms(p_RenderBackEnd);

	K15_GLProgram* vertexProgram = (K15_GLProgram*)K15_InternalGetGLObjectData(glContext, *p_MaterialPassDesc->vertexShaderHandle, K15_GL_TYPE_PROGRAM);
	K15_GLProgram* fragmentProgram = (K15_GLProgram*)K15_InternalGetGLObjectData(glContext, *p_MaterialPassDesc->fragmentShaderHandle, K15_GL_TYPE_PROGRAM);

	result = K15_GLUpdateProgramDataUniforms(p_RenderBackEnd, vertexProgram, p_MaterialPassDesc);

	if (result == K15_SUCCESS)
	{
		result = K15_GLUpdateProgramDataUniforms(p_RenderBackEnd, fragmentProgram, p_MaterialPassDesc);
	}

	return result;
}
/*********************************************************************************/