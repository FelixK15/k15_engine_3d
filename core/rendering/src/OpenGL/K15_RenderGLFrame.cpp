 /*********************************************************************************/
inline uint8 K15_GLClearScreen(K15_RenderBackEnd* p_RenderBackEnd)
{
	kglSwapBuffers((K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform);
	kglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	return K15_SUCCESS;
}
/*********************************************************************************/