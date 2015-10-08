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
K15_RenderVertexFormatDesc K15_CreateRenderVertexFormatDesc(K15_RenderContext* p_RenderContext, uint32 p_NumAttributes, ...)
{
	K15_ASSERT_TEXT(p_RenderContext, "Render Context is NULL.");
	K15_ASSERT_TEXT(p_NumAttributes, "Number of vertex attributes is 0.");

	K15_ShaderProcessorContext* shaderProcessorContext = p_RenderContext->backEnd.shaderProcessorContext;
	K15_RenderVertexFormatDesc desc = {};
	
	va_list vlist;
	va_start(vlist, p_NumAttributes);
	
	K15_CustomMemoryAllocator* memoryAllocator = &p_RenderContext->memoryAllocator;
	desc.elements = (K15_RenderVertexFormatElementDesc*)K15_AllocateFromMemoryAllocator(memoryAllocator, p_NumAttributes * sizeof(K15_RenderVertexFormatElementDesc));

	uint32 offsetInBytes = 0;

	for (uint32 attributeIndex = 0;
		attributeIndex < p_NumAttributes;
		++attributeIndex)
	{
		int semanticID = va_arg(vlist, int);
		int typeID = va_arg(vlist, int);

		K15_RenderVertexFormatElementDesc currentElement = K15_InternalCreateVertexFormatElement(p_RenderContext, semanticID, typeID);
		currentElement.offsetInBytes = offsetInBytes;
		desc.elements[desc.numAttributes++] = currentElement;

		offsetInBytes += K15_GetTypeSizeInBytesByTypeID(shaderProcessorContext, typeID);
	}

	desc.hash = K15_GenerateDataHash((byte*)desc.elements, sizeof(desc.elements));
	desc.stride = K15_InternalCalculateVertexSizeInBytes(shaderProcessorContext, &desc);

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