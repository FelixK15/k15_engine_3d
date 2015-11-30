/*********************************************************************************/
intern inline uint32 K15_InternalPush2DScreenspaceRect(K15_RenderBackEnd* p_RenderBackEnd, 
													float* p_VertexBuffer, uint32 p_VertexBufferOffsetIndex, 
													float p_PosLeft, float p_PosRight, float p_PosTop, float p_PosBottom,
													float p_UCoordLeft, float p_UCoordRight, float p_VCoordTop, float p_VCoordBottom)
{
	uint32 vertexIndex = p_VertexBufferOffsetIndex;

	p_VertexBuffer[vertexIndex++] = p_PosLeft;
	p_VertexBuffer[vertexIndex++] = p_PosTop;
	p_VertexBuffer[vertexIndex++] = p_UCoordLeft;
	p_VertexBuffer[vertexIndex++] = p_VCoordTop;
	
	p_VertexBuffer[vertexIndex++] = p_PosLeft;
	p_VertexBuffer[vertexIndex++] = p_PosBottom;
	p_VertexBuffer[vertexIndex++] = p_UCoordLeft;
	p_VertexBuffer[vertexIndex++] = p_VCoordBottom;
	
	p_VertexBuffer[vertexIndex++] = p_PosRight;
	p_VertexBuffer[vertexIndex++] = p_PosTop;
	p_VertexBuffer[vertexIndex++] = p_UCoordRight;
	p_VertexBuffer[vertexIndex++] = p_VCoordTop;
	
	p_VertexBuffer[vertexIndex++] = p_PosLeft;
	p_VertexBuffer[vertexIndex++] = p_PosBottom;
	p_VertexBuffer[vertexIndex++] = p_UCoordLeft;
	p_VertexBuffer[vertexIndex++] = p_VCoordBottom;
	
	p_VertexBuffer[vertexIndex++] = p_PosRight;
	p_VertexBuffer[vertexIndex++] = p_PosBottom;
	p_VertexBuffer[vertexIndex++] = p_UCoordRight;
	p_VertexBuffer[vertexIndex++] = p_VCoordBottom;
	
	p_VertexBuffer[vertexIndex++] = p_PosRight;
	p_VertexBuffer[vertexIndex++] = p_PosTop;
	p_VertexBuffer[vertexIndex++] = p_UCoordRight;
	p_VertexBuffer[vertexIndex++] = p_VCoordTop;

	return vertexIndex;
}
/*********************************************************************************/
intern result8 K15_InternalDrawGeometry(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderGeometryDesc* p_RenderGeometryDesc)
{
	return p_RenderBackEnd->renderInterface.drawGeometry(p_RenderBackEnd, p_RenderGeometryDesc);
}
/*********************************************************************************/
intern void K15_InternalDrawDefault2DTexture(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderResourceHandle* p_TextureHandle,
											 float p_PosLeft, float p_PosRight, float p_PosTop, float p_PosBottom,
											 float p_UCoordLeft, float p_UCoordRight, float p_VCoordTop, float p_VCoordBottom)
{
	K15_RenderContext* renderContext = p_RenderBackEnd->renderContext;
	K15_CustomMemoryAllocator* renderAllocator = &renderContext->memoryAllocator;

	K15_RenderVertexFormatDesc vertexFormatDesc = K15_CreateRenderVertexFormatDesc(p_RenderBackEnd->renderContext, 2, 
		K15_SEMANTIC_POSITION_ID, K15_TYPE_FLOAT_VECTOR2_ID,
		K15_SEMANTIC_TEXCOORD1_ID, K15_TYPE_FLOAT_VECTOR2_ID);

	uint32 numVertices = 6;
	uint32 sizeVerticesInBytes = vertexFormatDesc.stride * numVertices;

	float* vertexMemory = (float*)K15_AllocateFromMemoryAllocator(renderAllocator, sizeVerticesInBytes);

	K15_InternalPush2DScreenspaceRect(p_RenderBackEnd, vertexMemory, 0, 
		p_PosLeft, p_PosRight, p_PosTop, p_PosBottom, 
		p_UCoordLeft, p_UCoordRight, p_VCoordTop, p_VCoordBottom);

	K15_RenderVertexData* vertexData = p_RenderBackEnd->renderInterface.updateVertexData(p_RenderBackEnd, vertexMemory, numVertices, &vertexFormatDesc);
	K15_RenderGeometryDesc renderGeometry = {};

	K15_SetRenderMaterialRenderResourceDataByName(&p_RenderBackEnd->resources.materials.default2DMaterial.materialPasses[0], "tex", p_TextureHandle);

	renderGeometry.vertexData = vertexData;
	renderGeometry.topology = K15_RENDER_TOPOLOGY_TRIANGLE_STRIP;
	renderGeometry.worldMatrix = K15_GetIdentityMatrix4();
	renderGeometry.material = &p_RenderBackEnd->resources.materials.default2DMaterial;

	K15_InternalDrawGeometry(p_RenderBackEnd, &renderGeometry);

	p_RenderBackEnd->renderInterface.freeVertexData(p_RenderBackEnd, vertexData);
	K15_FreeFromMemoryAllocator(renderAllocator, vertexMemory);
}
/*********************************************************************************/