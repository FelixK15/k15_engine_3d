 /*********************************************************************************/
inline uint8 K15_GLClearScreen(K15_RenderContext* p_RenderContext)
{
	kglSwapBuffers((K15_GLRenderContext*)p_RenderContext->userData);
	kglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	return K15_SUCCESS;
}
/*********************************************************************************/