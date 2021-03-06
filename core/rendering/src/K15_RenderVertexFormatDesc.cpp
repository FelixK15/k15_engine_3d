#include "K15_RenderVertexFormatDesc.h"
#include "K15_ShaderCompiler.h"
#include "K15_String.h"

/*********************************************************************************/
intern uint32 K15_InternalCalculateVertexSizeInBytes(K15_ShaderProcessorContext* p_ShaderProcessorContext, K15_RenderVertexFormatDesc* p_VertexFormatDesc)
{
	uint32 sizeInBytes = 0;

	for (uint32 attributeIndex = 0;
		attributeIndex < p_VertexFormatDesc->numAttributes;
		++attributeIndex)
	{
		int typeID = p_VertexFormatDesc->elements[attributeIndex].typeID;
		uint32 numValues = p_VertexFormatDesc->elements[attributeIndex].numValues;
		uint32 elementSizeInBytes = K15_GetTypeSizeInBytesByTypeID(p_ShaderProcessorContext, typeID);

		sizeInBytes += elementSizeInBytes;
	}

	return sizeInBytes;
}
/*********************************************************************************/
intern K15_RenderVertexFormatElementDesc K15_InternalCreateVertexFormatElement(K15_RenderContext* p_RenderContext, int p_SemanticID, int p_TypeID)
{
	K15_RenderVertexFormatElementDesc element = {};

	element.typeID = p_TypeID;
	element.semanticID = p_SemanticID;
	
	return element;
}
/*********************************************************************************/
intern bool8 K15_InternalFindVertexFormatInCache(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderVertexFormatDesc* p_VertexFormat, uint32 p_NumAttributes, va_list p_VList)
{
	uint32 numVertexFormats = p_RenderBackEnd->vertexFormatCache.numVertexFormats;
	K15_RenderVertexFormatDesc* cachedVertexFormats = p_RenderBackEnd->vertexFormatCache.vertexFormats;

	bool8 foundVertexCacheFormat = K15_FALSE;

	for (uint32 vertexFormatCacheIndex = 0;
		vertexFormatCacheIndex < numVertexFormats;
		++vertexFormatCacheIndex)
	{
		K15_RenderVertexFormatDesc* cachedVertexFormat = &cachedVertexFormats[vertexFormatCacheIndex];

		if (cachedVertexFormat->numAttributes == p_NumAttributes)
		{
			bool8 sameVertexFormat = K15_TRUE;

			for (uint32 attributeIndex = 0;
				attributeIndex < p_NumAttributes;
				++attributeIndex)
			{
				int semanticID = va_arg(p_VList, int);
				int typeID = va_arg(p_VList, int);

				if (cachedVertexFormat->elements[attributeIndex].semanticID != semanticID ||
					cachedVertexFormat->elements[attributeIndex].typeID != typeID)
				{
					sameVertexFormat = K15_FALSE;
					break;
				}
			}

			if (sameVertexFormat)
			{
				foundVertexCacheFormat = K15_TRUE;
				*p_VertexFormat = *cachedVertexFormat;
				break;
			}
		}
	}
		
	return foundVertexCacheFormat;
}
/*********************************************************************************/
intern void K15_InternalCreateVertexFormat(K15_RenderBackEnd* p_RenderBackEnd, K15_RenderVertexFormatDesc* p_VertexFormatDesc, uint32 p_NumAttributes, va_list p_VList)
{
	K15_RenderContext* renderContext = p_RenderBackEnd->renderContext;
	K15_CustomMemoryAllocator* memoryAllocator = &renderContext->memoryAllocator;
	K15_ShaderProcessorContext* shaderProcessorContext = p_RenderBackEnd->shaderProcessorContext;
	uint32 numCachedVertexFormats = p_RenderBackEnd->vertexFormatCache.numVertexFormats;

	K15_ASSERT(numCachedVertexFormats+1 < K15_RENDER_MAX_VERTEX_FORMATS);

	p_VertexFormatDesc->elements = (K15_RenderVertexFormatElementDesc*)K15_AllocateFromMemoryAllocator(memoryAllocator, p_NumAttributes * sizeof(K15_RenderVertexFormatElementDesc));

	uint32 offsetInBytes = 0;

	for (uint32 attributeIndex = 0;
		attributeIndex < p_NumAttributes;
		++attributeIndex)
	{
		int semanticID = va_arg(p_VList, int);
		int typeID = va_arg(p_VList, int);

		K15_RenderVertexFormatElementDesc currentElement = K15_InternalCreateVertexFormatElement(renderContext, semanticID, typeID);
		currentElement.offsetInBytes = offsetInBytes;
		p_VertexFormatDesc->elements[p_VertexFormatDesc->numAttributes++] = currentElement;

		offsetInBytes += K15_GetTypeSizeInBytesByTypeID(shaderProcessorContext, typeID);
	}

	p_VertexFormatDesc->hash = K15_GenerateDataHash((byte*)p_VertexFormatDesc->elements, sizeof(p_VertexFormatDesc->elements));
	p_VertexFormatDesc->stride = K15_InternalCalculateVertexSizeInBytes(shaderProcessorContext, p_VertexFormatDesc);

	p_RenderBackEnd->vertexFormatCache.vertexFormats[numCachedVertexFormats] = *p_VertexFormatDesc;

	p_RenderBackEnd->vertexFormatCache.numVertexFormats++;
}
/*********************************************************************************/
K15_RenderVertexFormatDesc K15_CreateRenderVertexFormatDesc(K15_RenderContext* p_RenderContext, uint32 p_NumAttributes, ...)
{
	K15_ASSERT_TEXT(p_RenderContext, "Render Context is NULL.");
	K15_ASSERT_TEXT(p_NumAttributes, "Number of vertex attributes is 0.");
	
	K15_RenderBackEnd* backEnd = &p_RenderContext->backEnd;
	K15_RenderVertexFormatDesc desc = {};
	
	va_list vlist;
	va_start(vlist, p_NumAttributes);
	
	bool8 foundVertexFormat = K15_InternalFindVertexFormatInCache(backEnd, &desc, p_NumAttributes, vlist);

	if (!foundVertexFormat)
	{
		va_start(vlist, p_NumAttributes);
		K15_InternalCreateVertexFormat(backEnd, &desc, p_NumAttributes, vlist);
	}

	va_end(vlist);

	return desc;
}
/*********************************************************************************/
void K15_FreeRenderVertexFormatDesc(K15_RenderContext* p_RenderContext, K15_RenderVertexFormatDesc* p_RenderFormatDesc)
{
	K15_ASSERT_TEXT(p_RenderContext, "Render Context is NULL.");
	K15_ASSERT_TEXT(p_RenderFormatDesc, "Render Vertex Format is NULL.");

	K15_CustomMemoryAllocator* memoryAllocator = &p_RenderContext->memoryAllocator;
	K15_FreeFromMemoryAllocator(memoryAllocator, p_RenderFormatDesc->elements);
}
/*********************************************************************************/