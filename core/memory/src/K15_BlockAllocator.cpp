#include "K15_BlockAllocator.h"
#include "K15_MemoryBuffer.h"

/*********************************************************************************/
struct K15_BlockHeaderInfo
{
	uint32 size;
	uint32 offset;
	bool8 free;
};

struct K15_BlockAllocatorInfo
{
	K15_BlockHeaderInfo* firstBlock;
	uint32 memoryBlockSize;
};
/*********************************************************************************/
intern void* K15_InternalFindFreeBlock_r(K15_BlockHeaderInfo* p_BlockHeader, uint32 p_MemoryBlockSize, uint32 p_SizeInBytes)
{
	byte* memory = (byte*)p_BlockHeader;
	memory += sizeof(K15_BlockHeaderInfo);

	if (p_BlockHeader->free && 
		p_BlockHeader->size >= p_SizeInBytes)
	{
		//calculate new size for the current block
		p_BlockHeader->size -= p_SizeInBytes;
		
		//create new block
		K15_BlockHeaderInfo* newBlock = (K15_BlockHeaderInfo*)(memory + p_BlockHeader->size);
		newBlock->offset = p_BlockHeader->offset + p_BlockHeader->size;
		newBlock->size = p_SizeInBytes;
		newBlock->free = K15_FALSE;

		memory = (byte*)newBlock;
		//get memory after the block
		return memory + sizeof(K15_BlockHeaderInfo);
	}

	if (p_BlockHeader->offset + p_SizeInBytes >= p_MemoryBlockSize)
	{
		return 0;
	}

	K15_BlockHeaderInfo* nextBlock = (K15_BlockHeaderInfo*)(memory + p_BlockHeader->offset + p_BlockHeader->size);

	return K15_InternalFindFreeBlock_r(nextBlock, p_MemoryBlockSize, p_SizeInBytes);
}
/*********************************************************************************/
intern void K15_InternalDefragmentMemoryBlocks_r(K15_BlockHeaderInfo* p_BlockHeader)
{

}
/*********************************************************************************/
intern void* K15_InternalBlockAlloc(size_t p_SizeInBytes, void* p_UserData)
{
	K15_BlockAllocatorInfo* allocatorInfo = (K15_BlockAllocatorInfo*)p_UserData;

	//find next free block
	K15_BlockHeaderInfo* blockHeader = allocatorInfo->firstBlock;
	uint32 memoryBlockSize = allocatorInfo->memoryBlockSize;

	//add book keeping size
	uint32 sizeInBytes = (uint32)(p_SizeInBytes + sizeof(K15_BlockHeaderInfo));
	void* memory = K15_InternalFindFreeBlock_r(blockHeader, memoryBlockSize, sizeInBytes);

	if (!memory)
	{
		K15_InternalDefragmentMemoryBlocks_r(blockHeader);
	}

	return K15_InternalFindFreeBlock_r(blockHeader, memoryBlockSize, sizeInBytes);
}
/*********************************************************************************/
intern void K15_InternalBlockFree(void* p_Pointer, void* p_UserData)
{
	K15_BlockAllocatorInfo* allocatorInfo = (K15_BlockAllocatorInfo*)p_UserData;
	byte* bytePointer = (byte*)p_Pointer;

	K15_BlockHeaderInfo* blockHeader = (K15_BlockHeaderInfo*)(bytePointer - sizeof(K15_BlockHeaderInfo));
	blockHeader->free = K15_TRUE;
}
/*********************************************************************************/
K15_CustomMemoryAllocator K15_CreateBlockAllocator(K15_MemoryBuffer* p_MemoryBuffer, char* p_AllocatorName)
{
	K15_BlockAllocatorInfo* allocatorInfo = (K15_BlockAllocatorInfo*)K15_GetMemoryFromMemoryBuffer(p_MemoryBuffer, sizeof(K15_BlockAllocatorInfo));

	//create first block header
	byte* bufferMemory = p_MemoryBuffer->buffer + p_MemoryBuffer->usedBytesOffset;
	K15_BlockHeaderInfo* firstBlock = (K15_BlockHeaderInfo*)bufferMemory;

	firstBlock->free = K15_TRUE;
	firstBlock->size = p_MemoryBuffer->sizeInBytes - sizeof(K15_BlockAllocatorInfo);
	firstBlock->offset = sizeof(K15_BlockAllocatorInfo);

	allocatorInfo->firstBlock = firstBlock;
	allocatorInfo->memoryBlockSize = p_MemoryBuffer->sizeInBytes;

	return K15_CreateCustomMemoryAllocator(K15_InternalBlockAlloc, K15_InternalBlockFree, allocatorInfo, p_AllocatorName);
}
/*********************************************************************************/
