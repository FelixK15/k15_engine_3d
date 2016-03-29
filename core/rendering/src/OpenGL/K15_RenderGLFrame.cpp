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

	return K15_SUCCESS;
}
/*********************************************************************************/