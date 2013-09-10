/**
 * @file K15_Memory.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/07/23
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
 *
 * @section DESCRIPTION
 *
 * The Memory class is responsible for allocating and freeing memory, 
 * provide the user with memory information (e.g. how much memory already has been allocated)
 * and keeping track of memory leaks (optional)
 *
 * You can activate memory leak detection by passing System::SF_PROTOCOL_MEMORY_USAGE as one of the flags
 * you can pass by calling System::Initialize().
 */
#pragma once

#ifndef __K15_MEMORY__
#define __K15_MEMORY__

#include "K15_UsefulMacros.h"
#include "K15_MemoryBlock.h"

#define K15_NEW new(__FILE__,__LINE__)
#define K15_DELETE delete

#define SAFE_DELETE(pPointer) if(pPointer){K15_DELETE pPointer; pPointer = NULL;}

/**
  * Custom new, new[], delete and delete[] operators.
  * For easier use, use the macro K15_NEW instead of just new.
  */
void *operator new[](unsigned int iSize,const char* sFile,unsigned int iLineNumber);
void *operator new(unsigned int iSize,const char* sFile,unsigned int iLineNumber);
void *operator new(unsigned int iSize);
void *operator new[](unsigned int iSize);
void *operator new(unsigned int iSize,void *pWhere);
void *operator new[](unsigned int iSize,void *pWhere);

void operator delete[](void* pPointer);
void operator delete(void* pPointer);

namespace K15_EngineV2
{
	class K15ENGINE2_API Memory
	{
		//System has to call some private functions during System::Initialize().
		friend class System;

	public:

		/**
		* Performs a cleanup of all allocated objects used internally.
		* @note This function will get called internally. There's no reason for you to
		* call this function by yourself.
		*/
		static void CleanUp();

		/**
		* This function allocates a new block of memory via malloc().
		* @note This function is called via the macro K15_NEW. You just need to
		*	   replace new with K15_NEW to make use of this function.
		*
		* Example:
		* @code{.cpp}
		* //Object *myObject = new Object();
		* Object *myObject = K15_NEW Object();
		* @endcode
		*
		* @param  iSize - Amount of bytes you want to allocate.
		* @param  sFile - file in which the call to Memory::Allocate() occured.
		* @param  iLineNumber - line in which the call to Memory::Allocate() occured.
		*
		* @return void * - Pointer to newly allocated memory, or NULL if allocation failed.
		*/
		static void *Allocate(unsigned int iSize,const char* sFile,unsigned int iLineNumber,bool bArray);

		/**
		* This function deallocates previously allocated memory via free().
		* @note This function is called via the normal delete operator.
		*
		* @param  pPointer - pointer to previously allocated memory.
		*/
		static void Free(void *pPointer,bool bArray);

		/**
		* This function is called during allocation. It will create a new MemoryAlloc Object and fill it
		* with the given parameters to keep track of allocated memory.
		*
		* @note This function is only called when Memory Logging has been activated.
		*
		* @param  pPointer - pointer to newly allocated memory.
		* @param  iSize - size of allocated memory in byte.
		* @param  sFile - file in which the call to Memory::Allocate() occured.
		* @param  iLineNumber - line in which the call to Memory::Allocate() occured.
		*/
		static void ProtocolMemoryAllocation(void*& pPointer,unsigned int iSize,const char* sFile,unsigned int iLineNumber,bool bArray);

		/**
		* This Function is called during deallocation. It will search for a MemoryAlloc object corresponding to the
		* allocated memory pointed to by ptr.
		*
		* @note This function is only called when Memory Logging has been activated.
		*
		* @param  pPointer - pointer to memory.
		*/
		static void ProtocolMemoryDeallocation(void*& pPointer,bool bArray);

		/**
		* If you want to know how much memory (in byte) the OS reserved for the Engine you should call this function.
		*
		* @return unsigned int - Amount of memory (in byte) the OS reserved for the Engine.
		*/
		static unsigned int GetApplicationMemory();

		/**
		* This function will tell you how much memory (in byte) the system has in general.
		*
		* @return unsigned int - Amount of memory this system has.
		*/
		static unsigned int GetSystemMemory();

		/**
		* This function will return the amount of memory (in byte) that is currently allocated by the engine.
		*
		* @return unsigned int - Amount of memory the engine has allocated.
		*/
		static unsigned int GetAllocatedMemory();

		/**
		* This function will return the amount of missing delete calls. (Part of the memory leak detection system)
		*
		* @note This function will get called during CheckForMemoryLeak(). (not implemented yet)
		*
		* @return unsigned int - amount of missing delete calls.
		*/
		static unsigned int GetAmountOfMissingDeleteStatements();

		/**
		*	@warning not implemented yet.
		*/
		static void CheckForMemoryLeak();

		/**
		* Will copy a block of memory from destination to source.
		*
		* @note Unlike Memory::MemMove() the memory can not overlap (source is not within range of destination + size or vice versa)
		*
		* @param  pDestination - pointer to destination memory address.
		* @param  pSource - pointer to source memory address.
		* @param  iSize - amount of memory (in byte) you want to copy from source to destination
		*
		* @return void* - pointer to pDestination.
		*/
		static void MemCopy(void* pDestination,const void* pSource,unsigned int iSize);

		/**
		* Will copy a block of memory from destination to source.
		*
		* @note Unlike Memory::MemCopy() the memory can overlap (source is in range within destination + size or vice versa)
		*
		* @param  pDestination - pointer to destination memory address.
		* @param  pSource - pointer to source memory address.
		* @param  iSize - amount of memory (in byte) you want to copy from source to destination
		*
		* @return void* - pointer to pDestination.
		*/
		static void MemMove(void* pDestination,const void* pSource,unsigned int iSize);

		/**
		* Will fill memory with val from destination to destination + size
		*
		* @param  pDestination - start address of memory that shall get filled.
		* @param  iValue - value the memory will get filled with.
		* @param  iSize - amount of memory that shall get filled.
		*
		* @return void* - pointer to pDestination.
		*/
		static void MemSet(void* pDestination,char iValue,unsigned int iSize);

	private:
		static void SetApplicationMemory(size_t iMemory);
		static void SetSystemMemory(size_t iMemory);

		static void EnableMemorySerialization();
		static void EnableEventsOnMemoryCommunication();

		static void _AddMemoryBlock(MemoryHeader *pBlock);
		static void _RemoveMemoryBlock(MemoryHeader *pBlock);

	private:
		static bool ms_bMemoryLogging;
		static bool ms_bMemorySerialization;

		static unsigned int ms_iAllocatedMemory;
		static unsigned int ms_iSystemMemory;
		static unsigned int ms_iApplicationMemory;
		static unsigned int ms_iAmountAllocations;

		static MemoryHeader *ms_pHeadMemoryBlock;
	};

	#include "../src/K15_Memory.inl"
}

#endif //__K15_MEMORY__