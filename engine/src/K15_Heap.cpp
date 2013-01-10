/**
 * @file K15_Heap.cpp
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/08/06
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

#include "K15_Heap.h"

#include "K15_List.h"

using namespace K15_EngineV2;

Heap::Heap(size_t iSize)
{
	m_pHeap = K15_NEW byte[iSize];
	m_iSize = iSize;
	m_iUsedMemory = 0;
	/*m_lMemoryBlocks = K15_NEW List<MemoryBlock>();
	m_lMemoryBlocks->PushFront(MemoryBlock(m_pHeap,iSize));*/
}

Heap::Heap(const Heap& hpSource)
{
	m_pHeap = K15_NEW byte[hpSource.m_iSize];
	m_lMemoryBlocks = K15_NEW List<MemoryHeapNode>();

	Memory::MemCopy(m_pHeap,hpSource.m_pHeap,hpSource.m_iSize);

	m_iSize = hpSource.m_iSize;
	m_iUsedMemory = hpSource.m_iUsedMemory;
	
	size_t iOtherHeapAddress = (size_t)hpSource.m_pHeap;
	size_t iThisHeapAddress = (size_t)m_pHeap;

	for(ListNode<MemoryHeapNode> *pNode = hpSource.m_lMemoryBlocks->GetFirstElement();pNode;pNode = pNode->GetNext()){
		MemoryHeapNode &mbCurBlock = pNode->GetElement();

		size_t iAddress = (size_t)mbCurBlock.Pointer;
		iAddress -= iOtherHeapAddress;
		iAddress += iThisHeapAddress;

		//m_lMemoryBlocks->PushBack(MemoryBlock((void*)iAddress,mbCurBlock.Size,mbCurBlock.IsFree));
	}
}

Heap::~Heap()
{
	if(m_pHeap){
		K15_DELETE[] m_pHeap;
		m_pHeap = NULL;
	}

	K15_DELETE m_lMemoryBlocks;
}

void *Heap::Allocate( size_t iSize )
{
	void *pPointerNewBlock = NULL;

	if(GetFreeMemory() < iSize){
		return NULL;
	}

	/*The function will check again if there's enough memory 
	(In case _OutOfMemory managed to allocate new memory)*/
	if(GetFreeMemory() >= iSize){
		size_t iStartAddress = (size_t)m_pHeap;
		size_t iEndAddress = m_iSize + iStartAddress;
		
		/*The function will start to look at each MemoryBlock. If the block is free and has enough size,
		the block will be marked as used and the memory of the block will get returned. (A new MemoryBlock will get created
		which holds the rest of the memory)*/
		for(ListNode<MemoryHeapNode> *pNode = m_lMemoryBlocks->GetFirstElement();pNode;pNode = pNode->GetNext()){
			MemoryHeapNode &mbCurrentBlock = pNode->GetElement();

			if(mbCurrentBlock.IsFree){
				if(mbCurrentBlock.Size >= iSize){
					mbCurrentBlock.Size -= iSize;

					size_t iAddressCurBlock = (size_t)mbCurrentBlock.Pointer;
					pPointerNewBlock = mbCurrentBlock.Pointer;

					iAddressCurBlock += iSize;
					mbCurrentBlock.Pointer = (void*)iAddressCurBlock;

					m_iUsedMemory += iSize;

					//m_lMemoryBlocks->PushBack(MemoryBlock(pPointerNewBlock,iSize,false));
					break;
				}
			}
		}
	}
	
	return pPointerNewBlock;
}

bool Heap::Free( void *pPointer, size_t &iSizeOut )
{
	size_t iStartAddress = (size_t)m_pHeap;
	size_t iPointerAddress = (size_t)pPointer;

	//Is the pointer within the range of the memory block?
	if(iPointerAddress >= iStartAddress && iPointerAddress < (iStartAddress + m_iSize)){
		for(ListNode<MemoryHeapNode> *pNode = m_lMemoryBlocks->GetFirstElement();pNode;pNode = pNode->GetNext()){
			MemoryHeapNode &mbCurrentBlock = pNode->GetElement();

			//Is the current block the block we are looking for?
			if(mbCurrentBlock.Pointer == pPointer){
				m_iUsedMemory -= mbCurrentBlock.Size;
				iSizeOut = mbCurrentBlock.Size;

				//Try to merge the current block with its successor.
				if(_MergeBlocks(mbCurrentBlock)){
					//If the merge was successful, delete the current block.
					m_lMemoryBlocks->Erase(pNode);
					return true;
				}

				break;
			}
		} 
	}

	return false;
}

bool Heap::_MergeBlocks( MemoryHeapNode &mbBlock )
{
	size_t iAddressNextBlock = (size_t)mbBlock.Pointer + m_iSize;
	size_t iStartAddress = (size_t)m_pHeap;
	bool bReturn = false;

	//Check if the block has a successor
	if(!(iAddressNextBlock > iStartAddress + m_iSize)){
		//if the block has a successor, try to find it
		for(ListNode<MemoryHeapNode> *pNode = m_lMemoryBlocks->GetLastElement();pNode;pNode = pNode->GetPrev()){
			MemoryHeapNode &mbCurrentBlock = pNode->GetElement();
			//if the successor has been found (and is marked as free), the size of the current block
			//will be added to the size of the predecessor.
			if((size_t)mbCurrentBlock.Pointer == iAddressNextBlock && mbCurrentBlock.IsFree){
				mbCurrentBlock.Size += mbBlock.Size;
				mbCurrentBlock.Pointer = mbBlock.Pointer;
				bReturn = true;
			}
		}
	}

	//If the merge was unsuccessful, just mark the block as free.
	if(!bReturn){
		mbBlock.IsFree = true;
	}

	return bReturn;
}