/*********************************************************************************/
intern void K15_InternalGLCreateGUIProjectionMatrix(K15_RenderBackEnd* p_RenderBackEnd, float p_Width, float p_Height)
{
	//set the GL orthographic projection matrix
	K15_Matrix4 guiProj = 
	{ 2.f / p_Width, 0.f,			 0.f,  0.f,
	  0.f,			-2.f / p_Height, 0.f,  0.f,
      0.f,		     0.f,			 1.f,  0.f,
	 -1.f,		     1.f,			 0.f,  1.f };

	K15_UpdateUniformCacheEntry(&p_RenderBackEnd->uniformCache,
		K15_UNIFORM_SEMANTIC_GUI_PROJECTION_MATRIX, (byte*)&guiProj);
}
/*********************************************************************************/
inline result8 K15_GLClearScreen(K15_RenderBackEnd* p_RenderBackEnd)
{
	K15_OPENGL_CALL(kglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	return K15_SUCCESS;
}
/*********************************************************************************/
inline result8 K15_GLSwapFrameBuffer(K15_RenderBackEnd* p_RenderBackEnd)
{
	K15_OPENGL_CALL(kglSwapBuffers((K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform));

	return K15_SUCCESS;
}
/*********************************************************************************/
inline result8 K15_GLResizeViewport(K15_RenderBackEnd* p_RenderBackEnd, uint32 p_Width, uint32 p_Height)
{
	K15_OPENGL_CALL(kglViewport(0, 0, p_Width, p_Height));
	K15_InternalGLCreateGUIProjectionMatrix(p_RenderBackEnd, p_Width, p_Height);

	return K15_SUCCESS;
}
/*********************************************************************************/