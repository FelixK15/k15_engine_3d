/**
 * @file K15_BaseAllocationPattern.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2013/09/10
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
 */

#ifndef _K15Engine_System_Memory_BaseAllocationPattern_h_
#define _K15Engine_System_Memory_BaseAllocationPattern_h_

namespace K15_Engine { namespace System { namespace Memory {

  class BaseAllocationPattern
  {
  public:
    BaseAllocationPattern(size_t iMemorySize);
    virtual ~BaseAllocationPattern();

    virtual void* allocate(size_t iSize);
    virtual void* allocateDebug(size_t iSize,const char* pFile,int iLineNumber,bool bArray,const char* pFunctionName);

    virtual void* deallocate(void* pPointer);
    virtual void* deallocateDebug(void* pPointer,const char* pFile,int iLineNumber,bool bArray,const char* pFunctionName);
  protected:
    byte* m_pMemory;

    size_t m_iMemorySize;
    size_t m_iUsedMemory;
  };
}}}

#endif //_K15Engine_System_Memory_BaseAllocationPattern_h_