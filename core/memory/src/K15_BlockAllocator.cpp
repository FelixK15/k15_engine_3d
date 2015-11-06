#include "K15_BlockAllocator.h"
#include "K15_MemoryBuffer.h"

/*********************************************************************************/
struct K15_BlockHeaderInfo
{
	uint32 memorySize;
	uint32 memoryOffsetFromBuffer;
	bool8 free;
};
/*********************************************************************************/
struct K15_BlockAllocatorInfo
{
	K15_BlockHeaderInfo* firstBlock;
	uint32 memoryBlockSize;
};
/*********************************************************************************/
intern void* K15_InternalFindFreeBlock_r(K15_BlockHeaderInfo* p_BlockHeader, uint32 p_MemoryBlockSize, uint32 p_SizeInBytes)
{
	//p_SizeInBytes = size to allocate without the size of the block header info struct
	uint32 sizeInBytesInclHeaderInfo = p_SizeInBytes + sizeof(K15_BlockHeaderInfo);
	byte* memoryFromBlock = (byte*)p_BlockHeader;
	memoryFromBlock += sizeof(K15_BlockHeaderInfo);

	if (p_BlockHeader->free && 
		p_BlockHeader->memorySize >= sizeInBytesInclHeaderInfo)
	{
		//calculate new size for the current block
		p_BlockHeader->memorySize -= sizeInBytesInclHeaderInfo;
		
		//create new block
		K15_BlockHeaderInfo* newBlock = (K15_BlockHeaderInfo*)(memoryFromBlock + p_BlockHeader->memorySize);
		newBlock->memoryOffsetFromBuffer = p_BlockHeader->memoryOffsetFromBuffer + p_BlockHeader->memorySize + sizeof(K15_BlockHeaderInfo);
		newBlock->memorySize = p_SizeInBytes;
		newBlock->free = K15_FALSE;

		memoryFromBlock = (byte*)newBlock;
		//get memory after the block
		return memoryFromBlock + sizeof(K15_BlockHeaderInfo);
	}

	//are we at the end of the buffer?
	if (p_BlockHeader->memoryOffsetFromBuffer + p_BlockHeader->memorySize + sizeof(K15_BlockHeaderInfo) >= p_MemoryBlockSize)
	{
		return 0;
	}

	K15_BlockHeaderInfo* nextBlock = (K15_BlockHeaderInfo*)(memoryFromBlock + p_BlockHeader->memorySize);

	return K15_InternalFindFreeBlock_r(nextBlock, p_MemoryBlockSize, p_SizeInBytes);
}
/*********************************************************************************/
intern void K15_InternalDefragmentMemoryBlocks_r(K15_BlockHeaderInfo* p_BlockHeader, uint32 p_MemoryBlockSize)
{
	K15_BlockHeaderInfo* currentBlock = p_BlockHeader;
	K15_BlockHeaderInfo* nextBlock = 0;

	if (currentBlock->memoryOffsetFromBuffer + currentBlock->memorySize + sizeof(K15_BlockHeaderInfo) >= p_MemoryBlockSize)
	{
		return;
	}

	byte* blockMemory = (byte*)currentBlock;
	blockMemory += sizeof(K15_BlockHeaderInfo);

	nextBlock = (K15_BlockHeaderInfo*)(blockMemory + currentBlock->memorySize);

	if (currentBlock->free &&
		nextBlock->free)
	{
		currentBlock->memorySize += nextBlock->memorySize + sizeof(K15_BlockHeaderInfo);
		K15_InternalDefragmentMemoryBlocks_r(currentBlock, p_MemoryBlockSize);
	}
	else
	{
		K15_InternalDefragmentMemoryBlocks_r(nextBlock, p_MemoryBlockSize);
	}
}
/*********************************************************************************/
intern void* K15_InternalBlockAlloc(size_t p_SizeInBytes, void* p_UserData)
{
	K15_BlockAllocatorInfo* allocatorInfo = (K15_BlockAllocatorInfo*)p_UserData;

	//find next free block
	K15_BlockHeaderInfo* blockHeader = allocatorInfo->firstBlock;
	uint32 memoryBlockSize = allocatorInfo->memoryBlockSize;

	//add book keeping size
	//uint32 sizeInBytes = (uint32)(p_SizeInBytes + sizeof(K15_BlockHeaderInfo));
	void* memory = K15_InternalFindFreeBlock_r(blockHeader, memoryBlockSize, (uint32)p_SizeInBytes);

	if (!memory)
	{
		K15_InternalDefragmentMemoryBlocks_r(blockHeader, memoryBlockSize);
	}

	return memory ? memory : K15_InternalFindFreeBlock_r(blockHeader, memoryBlockSize, (uint32)p_SizeInBytes);
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
intern void K15_InternalBlockClear(void* p_UserData)
{
	K15_BlockAllocatorInfo* allocatorInfo = (K15_BlockAllocatorInfo*)p_UserData;
	
	uint32 memorySize = allocatorInfo->memoryBlockSize;
	allocatorInfo->firstBlock->memorySize = memorySize - sizeof(K15_BlockAllocatorInfo);
	allocatorInfo->firstBlock->free = K15_TRUE;
}
/*********************************************************************************/
K15_CustomMemoryAllocator K15_CreateBlockAllocator(K15_MemoryBuffer p_MemoryBuffer, char* p_AllocatorName)
{
	K15_ASSERT(p_MemoryBuffer.sizeInBytes >= sizeof(K15_BlockAllocatorInfo) + sizeof(K15_BlockHeaderInfo));
	K15_BlockAllocatorInfo* allocatorInfo = (K15_BlockAllocatorInfo*)p_MemoryBuffer.buffer;

	p_MemoryBuffer.buffer += sizeof(K15_BlockAllocatorInfo);
	p_MemoryBuffer.sizeInBytes -= sizeof(K15_BlockAllocatorInfo);

	//create first block header
	byte* bufferMemory = p_MemoryBuffer.buffer;
	K15_BlockHeaderInfo* firstBlock = (K15_BlockHeaderInfo*)bufferMemory;

	firstBlock->free = K15_TRUE;
	firstBlock->memorySize = p_MemoryBuffer.sizeInBytes;
	firstBlock->memoryOffsetFromBuffer = sizeof(K15_BlockHeaderInfo);

	allocatorInfo->firstBlock = firstBlock;
	allocatorInfo->memoryBlockSize = p_MemoryBuffer.sizeInBytes;

	return K15_CreateCustomMemoryAllocator(K15_InternalBlockAlloc, K15_InternalBlockFree, K15_InternalBlockClear, allocatorInfo, p_AllocatorName);
}
/*********************************************************************************/
