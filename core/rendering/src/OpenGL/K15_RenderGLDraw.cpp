/*********************************************************************************/
inline result8 K15_GLDrawGeometry(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderGeometryDesc* p_RenderGeometry)
{
	result8 result = K15_SUCCESS;

	K15_GLRenderContext* glContext = (K15_GLRenderContext*)p_RenderBackEnd->specificRenderPlatform;

	K15_RenderResourceHandle vertexBuffer = p_RenderGeometry->vertexData->vertexBufferHandle;
	K15_RenderTopology topology = p_RenderGeometry->topology;

	K15_GLBuffer* glBuffer = (K15_GLBuffer*)K15_InternalGetGLObjectData(glContext, vertexBuffer, K15_GL_TYPE_BUFFER);

	K15_GLBindBuffer(glContext, glBuffer);
	K15_GLResolvePendingBufferUpdates(glContext, glBuffer);

	GLenum glTopology = K15_GLConvertTopology(topology);
	GLint startIndex = p_RenderGeometry->vertexData->startVertexIndex;
	GLsizei numVertices = p_RenderGeometry->vertexData->numVertices;

	K15_OPENGL_CALL(kglDrawArrays(glTopology, startIndex, numVertices));

	return result;
}
/*********************************************************************************/