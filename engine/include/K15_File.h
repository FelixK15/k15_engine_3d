/**
 * @file K15_File.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/10/16
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
 * 
 */
#pragma once

#ifndef __K15_FILE__
#define __K15_FILE__

#include "K15_StdInclude.h"

namespace K15_EngineV2
{
	class K15ENGINE2_API File
	{
	public:
		enum OpenMode {
			OM_READ = 0,
			OM_WRITE = 1,
			OM_APPEND = 2,
			OM_BINARY = 4
		};

		/**
		* Constructor
		* Tries to open a specific file using the given flags
		*
		* @param  pFileName path to the file you want to open.
		* @param  iOpenFlags flags you want to use. (See File::OpenMode - multiple options can be OR'd together)
		*/
		File(const char *pFile,U32 iOpenFlags);
		~File();

		/**
		* This function will read a block of data from the file. (The size of the block is iElementSize * iSize).
		* Once the block is read it will put a buffer at the position of pBuffer. You will be responsible for freeing 
		* that block once you're finished with it.
		* Example:
		* @code
		* int iSizeElement = sizeof(char);
		* int iSize = 50;
		* byte *pBuffer = NULL;
		*
		* File fFile("Test.txt",OM_READ);
		* if(fFile.Read(iSizeElement,iSize,pBuffer)){
		*	//...Do things with pBuffer
		*	delete[] pBuffer;
		* }
		* @endcode
		*
		* @param  iElementSize the size of one element to read (in bytes).
		* @param  iSize the number of elements you want to read.
		* @param  pBuffer pointer to where the buffer will get set.
		*
		* @return bool true if data has been read successful and false if not (e.g. file is not open)
		*/
		bool Read(U32 iElementSize,U32 iSize,byte *&pBuffer);

		/**
		* Reads a single line from the file. (moves the internal position)
		* Once the block is read it will put a buffer at the position of pBuffer. You will be responsible for freeing 
		* that block once you're finished with it.
		* Example:
		* @code
		* byte *pBuffer = NULL;
		*
		* File fFile("Test.txt",OM_READ);
		* if(fFile.ReadLine(pBuffer)){
		*	//...Do things with pBuffer
		*	delete[] pBuffer;
		* }
		* @endcode
		*
		* @param  pBuffer pointer to where the buffer will get set.
		*
		* @return bool - true if data has been read successful and false if not (e.g. file is not open)
		*/
		bool ReadLine(byte *&pBuffer);

		/**
		* This function will write a block of data to a file. (the size of the block must be iElementSize * iSize).
		* If the block is written to the file, the function will delete the memory block if bDeleteBuffer is true.
		*
		* @param  iElementSize the size of one element to write (in bytes).
		* @param  iSize the number of elements you want to read.
		* @param  pBuffer pointer to the buffer that holds the data you want to write.
		* @param  bDeleteBuffer boolean that defines wether or not the buffer will get deleted after all data has been written.
		*
		* @return bool true if data has been written successful and false if not (e.g. file is not open)
		*/
		bool Write(U32 iElementSize,U32 iSize,byte *pBuffer,bool bDeleteBuffer = false);

		/**
		* Use this function to check wether or not the file is open.
		*
		* @return bool true if file is open and false if not.
		*/
		inline bool IsOpen();

		/**
		* This function will close the file.
		* @note Close() will get called during the destructor call.
		*/
		void Close();

		/**
		* Tries to open a specific file using the given flags
		* @note Calls Close() on the old file if there was one.
		*
		* @param  pFileName path to the file you want to open.
		* @param  iOpenFlags flags you want to use. (See File::OpenMode - multiple options can be OR'd together)
		*
		* @return bool true if the file could be opened and false if not.
		*/
		bool Open(const char *pFileName,U32 iOpenFlags);

		/**
		* Check whether a specific file exists or not.
		*
		* @param  pFileName path to the file you want to check
		*
		* @return bool true if the file exists and false if not.
		*/
		static bool Exists(const char *pFileName);

		/**
		* If you want to erase a specific file, use this function.
		*
		* @param  pFileName path to the file you want to delete.
		*
		* @return bool true if file was deleted successfully and false if not.
		*/
		static bool Erase(const char *pFileName);

		/**
		* Tries to rename a specific file.
		*
		* @param  pFileName path to the file you want to rename
		* @param  pNewFileName path to the new filename.
		*
		* @return bool true if file was renamed successfully and false if not.
		*/
		static bool Rename(const char *pFileName,const char *pNewFileName);

	private:
		FILE *m_pFile;
		const char *m_pFileName;
		U32 m_iReferences;
	};
	#include "..\src\K15_File.inl"
}

#endif //__K15_FILE__