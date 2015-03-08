 /*********************************************************************************/
inline uint8 K15_GLClearScreen(K15_RenderContext* p_RenderContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	kglSwapBuffers((K15_GLRenderContext*)p_RenderContext->userData);
	return K15_SUCCESS;
}
/*********************************************************************************/