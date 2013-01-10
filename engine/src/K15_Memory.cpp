/**
 * @file K15_Memory.cpp
 * @author Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/12
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "K15_Memory.h"
#include "K15_CStdIncludes.h"
#include "K15_System.h"

using namespace K15_EngineV2;

bool Memory::ms_bMemoryLogging = false;
bool Memory::ms_bMemorySerialization = false;

unsigned int Memory::ms_iSystemMemory = 0;
unsigned int Memory::ms_iApplicationMemory = 0;
unsigned int Memory::ms_iAllocatedMemory = 0;
unsigned int Memory::ms_iAmountAllocations = 0;

MemoryBlock *Memory::ms_pHeadMemoryBlock = NULL;

void *operator new[](unsigned int iSize,const char* sFile,unsigned int sLineNumber)
{
	return Memory::Allocate(iSize,sFile,sLineNumber,true);
}

void *operator new(unsigned int iSize,const char* sFile,unsigned int sLineNumber)
{
	return Memory::Allocate(iSize,sFile,sLineNumber,false);
}

void *operator new(unsigned int iSize,void *pWhere)
{
	return pWhere;
}

void *operator new[](unsigned int iSize,void *pWhere)
{
	return pWhere;
}

void *operator new(unsigned int iSize)
{
	return Memory::Allocate(iSize,"",0,false);
}

void *operator new[](unsigned int iSize)
{
	return Memory::Allocate(iSize,"",0,true);
}

void operator delete[](void* pPointer)
{
	Memory::Free(pPointer,true);
}

void operator delete(void* pPointer)
{
	Memory::Free(pPointer,false);
}

void *Memory::Allocate(unsigned int iSize,const char* sFile,unsigned int sLineNumber,bool bArray)
{
//	if(ms_bMemoryLogging){
		iSize += sizeof(MemoryBlock);
//	}

	void *pPointer = malloc(iSize);

	//If ptr is pointing to NULL, the engine might use up all of the system memory.
	if(pPointer == NULL){
		/*
		 *@TODO
		 *Protocol: Error not enough memory;
		 */
		//assert(true);
	}

//	if(ms_bMemoryLogging){
		ProtocolMemoryAllocation(pPointer,iSize,sFile,sLineNumber,bArray);
		iSize -= sizeof(MemoryBlock);
//	}

	//Make everything in the new memory block is set to 0.
	memset(pPointer,0,iSize);

	return pPointer;
}

void Memory::Free( void *pPointer,bool bArray )
{
//	if(ms_bMemoryLogging){
		ProtocolMemoryDeallocation(pPointer,bArray);
//	}

	free(pPointer);
}

void Memory::ProtocolMemoryAllocation( void*& pPointer,unsigned int iSize,const char* sFile,unsigned int iLineNumber,bool bArray )
{
	MemoryBlock* pBlock = (MemoryBlock*)pPointer;

	pBlock->IsArray = bArray;
	pBlock->Size = iSize;
	pBlock->File = sFile;
	pBlock->Line = iLineNumber;
	pPointer = (void*)(pBlock + 1);

	_AddMemoryBlock(pBlock);
	ms_iAllocatedMemory += iSize;
	++ms_iAmountAllocations;
}

void Memory::ProtocolMemoryDeallocation( void*& pPointer,bool bArray )
{
 	MemoryBlock *pBlock = (MemoryBlock*)pPointer;
 	--pBlock;
 	
// 	assert(bArray == pBlock->IsArray);
 	
 	ms_iAllocatedMemory -= pBlock->Size;
 	--ms_iAmountAllocations;
 
 	pPointer = (void*)pBlock;
 
 	_RemoveMemoryBlock(pBlock);
}

void Memory::CheckForMemoryLeak()
{
	if(ms_pHeadMemoryBlock){
		char arrString[SHRT_MAX/2];
		char arrBuffer[UCHAR_MAX];
		
		void* pPointer = ms_pHeadMemoryBlock - 1;

		arrString[0] = 0;

		for(MemoryBlock *pBlock = ms_pHeadMemoryBlock;pBlock;pBlock = pBlock->Next,pPointer = pBlock - 1){
			arrBuffer[0] = 0;

			if(pBlock->IsArray){
				strcat(arrString,"Array-allocation: ");
			}else{
				strcat(arrString,"Non-array-allocation: ");
			}

			sprintf(arrBuffer,"%s<%d> | Pointer: 0x%p | Size: %d bytes.\n",pBlock->File,pBlock->Line,pPointer,pBlock->Size);
			strcat(arrString,arrBuffer);
		}

		g_pSystem->ShowMessageBox(arrString);
	}

	ms_bMemoryLogging = false;
}

void Memory::MemCopy( void* pDestination,const void* pSource,unsigned int iSize )
{
	/*byte *pDestinationMemory = (byte*)pDestination;
	byte *pSourceMemory = (byte*)pSource;

	while(iSize--){
		*pDestinationMemory++ = *pSourceMemory++;
	}*/

	memcpy(pDestination,pSource,iSize);
}

void Memory::MemMove( void* pDestination,const void* pSource,unsigned int iSize )
{
	/*byte *pBuffer = new byte[iSize];

	MemCopy(pBuffer,pSource,iSize);
	MemCopy(pDestination,pBuffer,iSize);

	delete[] pBuffer;*/

	memmove(pDestination,pSource,iSize);
}

void Memory::MemSet(void* pDestination,char iValue,unsigned int iSize)
{
	/*byte *pDestinationMemory = (byte*)pDestination;

	while(iSize--){
		*pDestinationMemory++ = iValue;
	}*/

	memset(pDestination,iValue,iSize);
}

void Memory::CleanUp()
{

}

void Memory::_AddMemoryBlock(MemoryBlock *pBlock)
{
	if(!ms_pHeadMemoryBlock){
		pBlock->Previous = NULL;
		pBlock->Next = NULL;
	}else{
		pBlock->Next = ms_pHeadMemoryBlock;
		ms_pHeadMemoryBlock->Previous = pBlock;
		pBlock->Previous = NULL;
	}

	ms_pHeadMemoryBlock = pBlock;
}

void Memory::_RemoveMemoryBlock(MemoryBlock *pBlock)
{
	if(pBlock->Next){
		pBlock->Next->Previous = pBlock->Previous;
	}

	if(pBlock->Previous){
		pBlock->Previous->Next = pBlock->Next;
	}

	if(pBlock == ms_pHeadMemoryBlock){
		ms_pHeadMemoryBlock = ms_pHeadMemoryBlock->Next;
	}
}
