#include "K15_BlockAllocator.h"
#include "K15_MemoryBuffer.h"

/*********************************************************************************/
struct K15_BlockHeaderInfo
{
	K15_BlockHeaderInfo* prevBlock;
	K15_BlockHeaderInfo* nextBlock;
	uint32 memorySize;
	bool8 free;
};
/*********************************************************************************/
struct K15_BlockAllocatorInfo
{
	K15_BlockHeaderInfo* firstBlock;
	uint32 freeMemoryInBytes;
};
/*********************************************************************************/
intern void K15_InternalTryToMergeWithNeighbours(K15_BlockHeaderInfo* p_BlockHeader)
{
	if (p_BlockHeader->free)
	{
		K15_BlockHeaderInfo* previousBlock = p_BlockHeader->prevBlock;
		K15_BlockHeaderInfo* nextBlock = p_BlockHeader->nextBlock;

		if (previousBlock && previousBlock->free)
		{
			uint32 sizeCurrentBlock = p_BlockHeader->memorySize + sizeof(K15_BlockHeaderInfo);
			previousBlock->memorySize += sizeCurrentBlock;

			if (nextBlock)
			{
				nextBlock->prevBlock = previousBlock;
				previousBlock->nextBlock = nextBlock;
			}
			else
			{
				previousBlock->nextBlock = 0;
			}

			p_BlockHeader = previousBlock;
		}
		else if (nextBlock && nextBlock->free)
		{
			uint32 sizeNextBlock = nextBlock->memorySize + sizeof(K15_BlockHeaderInfo);
			p_BlockHeader->memorySize += sizeNextBlock;

			if (nextBlock->nextBlock)
			{
				nextBlock->nextBlock = p_BlockHeader;
				p_BlockHeader->nextBlock = nextBlock->nextBlock;
			}
			else
			{
				p_BlockHeader->nextBlock = 0;
			}
		}
	}
}
/*********************************************************************************/
intern void* K15_InternalFindFreeBlock_r(K15_BlockHeaderInfo* p_BlockHeader, uint32 p_SizeInBytes)
{
	if (!p_BlockHeader)
	{
		return 0;
	}

	K15_BlockHeaderInfo* freeBlockHeader = 0;
	byte* blockMemory = 0;

	if (p_BlockHeader->free)
	{
		if (p_BlockHeader->memorySize == p_SizeInBytes)
		{
			freeBlockHeader = p_BlockHeader;
		}
		else if (p_BlockHeader->memorySize > p_SizeInBytes)
		{
			byte* blockHeaderAsByte = (byte*)p_BlockHeader;
			blockMemory = blockHeaderAsByte + sizeof(K15_BlockHeaderInfo);

			uint32 blockSizeInBytes = p_BlockHeader->memorySize;
			uint32 newBlockSizeInBytes = blockSizeInBytes - (p_SizeInBytes + sizeof(K15_BlockHeaderInfo));

			p_BlockHeader->memorySize = newBlockSizeInBytes;

			K15_BlockHeaderInfo* newBlockHeader = (K15_BlockHeaderInfo*)(blockMemory + newBlockSizeInBytes);
			newBlockHeader->prevBlock = p_BlockHeader;
			newBlockHeader->nextBlock = 0;
			newBlockHeader->free = K15_FALSE;

			freeBlockHeader = newBlockHeader;
			p_BlockHeader->nextBlock = newBlockHeader;
		}
	}

	if (freeBlockHeader)
	{
		byte* blockHeaderAsByte = (byte*)freeBlockHeader;
		blockMemory = blockHeaderAsByte + sizeof(K15_BlockHeaderInfo);
	}

	return blockMemory ? blockMemory : K15_InternalFindFreeBlock_r(p_BlockHeader->nextBlock, p_SizeInBytes);
}
/*********************************************************************************/
intern void K15_InternalDefragmentMemoryBlocks_r(K15_BlockHeaderInfo* p_BlockHeader)
{
	if (p_BlockHeader->nextBlock)
	{
		K15_InternalDefragmentMemoryBlocks_r(p_BlockHeader->nextBlock);
		K15_InternalTryToMergeWithNeighbours(p_BlockHeader->nextBlock);
	}
}
/*********************************************************************************/
intern void* K15_InternalBlockAlloc(size_t p_SizeInBytes, void* p_UserData)
{
	K15_BlockAllocatorInfo* allocatorInfo = (K15_BlockAllocatorInfo*)p_UserData;
	uint32 freeAllocatorSizeInBytes = allocatorInfo->freeMemoryInBytes;

	//find next free block
	K15_BlockHeaderInfo* blockHeader = allocatorInfo->firstBlock;
	uint32 memoryBlockSize = allocatorInfo->freeMemoryInBytes;

	//add book keeping size
	//uint32 sizeInBytes = (uint32)(p_SizeInBytes + sizeof(K15_BlockHeaderInfo));
	void* memory = K15_InternalFindFreeBlock_r(blockHeader, (uint32)p_SizeInBytes);

	if (!memory)
	{
		K15_InternalDefragmentMemoryBlocks_r(blockHeader);
	}

	return memory ? memory : K15_InternalFindFreeBlock_r(blockHeader, (uint32)p_SizeInBytes);
}
/*********************************************************************************/
intern void K15_InternalBlockFree(void* p_Pointer, void* p_UserData)
{
	K15_BlockAllocatorInfo* allocatorInfo = (K15_BlockAllocatorInfo*)p_UserData;
	byte* bytePointer = (byte*)p_Pointer;

	K15_BlockHeaderInfo* blockHeader = (K15_BlockHeaderInfo*)(bytePointer - sizeof(K15_BlockHeaderInfo));
	blockHeader->free = K15_TRUE;

	K15_InternalTryToMergeWithNeighbours(blockHeader);
}
/*********************************************************************************/
intern void K15_InternalBlockClear(void* p_UserData)
{
	K15_BlockAllocatorInfo* allocatorInfo = (K15_BlockAllocatorInfo*)p_UserData;
	
	uint32 memorySize = allocatorInfo->freeMemoryInBytes;
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
	firstBlock->nextBlock = 0;
	firstBlock->prevBlock = 0;

	allocatorInfo->firstBlock = firstBlock;
	allocatorInfo->freeMemoryInBytes = p_MemoryBuffer.sizeInBytes;

	return K15_CreateCustomMemoryAllocator(K15_InternalBlockAlloc, K15_InternalBlockFree, K15_InternalBlockClear, allocatorInfo, p_AllocatorName);
}
/*********************************************************************************/
