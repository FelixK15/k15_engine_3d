/**
 * @file K15_AllocatedObject.inl
 * @author  Felix Klinge <f.klinge15@gmail.com>
 * @version 1.0
 * @date 2012/07/11
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

/*********************************************************************************/
template<class Allocator> Allocator* AllocatedObject<Allocator>::MemoryAllocator = Allocator::getInstance();
/*********************************************************************************/
template<class Allocator>
AllocatedObject<Allocator>::AllocatedObject()
{
	if(!MemoryAllocator)
	{
		MemoryAllocator = Allocator::getInstance();
	}
}
/*********************************************************************************/
template<class Allocator>
AllocatedObject<Allocator>::~AllocatedObject()
{

}
/*********************************************************************************/
#if defined (K15_DEBUG)
/*********************************************************************************/
template<class Allocator>
void* AllocatedObject<Allocator>::alloc(size_t p_Size,const char* p_File,uint32 p_Line,bool p_Array,const char* p_Function)
{
	return Allocator::getInstance()->allocateDebug(p_Size,p_File,p_Line,p_Array,p_Function);
}
/*********************************************************************************/
template<class Allocator>
void AllocatedObject<Allocator>::free(void* p_Pointer,const char* p_File,uint32 p_Line,bool p_Array,const char* p_Function)
{
	return Allocator::getInstance()->deallocateDebug(p_Pointer,p_File,p_Line,p_Array,p_Function);
}
/*********************************************************************************/
template<class Allocator>
void* AllocatedObject<Allocator>::operator new(size_t p_Size,const char* p_File,uint32 p_Line,const char* p_Function)
{
	return Allocator::getInstance()->allocateDebug(p_Size,p_File,p_Line,false,p_Function);
}
/*********************************************************************************/
template<class Allocator>
void* AllocatedObject<Allocator>::operator new[](size_t p_Size,const char* p_File,uint32 p_Line,const char* p_Function)
{
	return Allocator::getInstance()->allocateDebug(p_Size,p_File,p_Line,true,p_Function);
}
/*********************************************************************************/
template<class Allocator>
void AllocatedObject<Allocator>::operator delete(void* p_Pointer,size_t p_Size)
{
	//return MemoryAllocator->deallocateDebug(p_Pointer,p_File,p_Line,false,p_Function);
	return Allocator::getInstance()->deallocateDebug(p_Pointer,"",0,false,"");
}
/*********************************************************************************/
template<class Allocator>
void AllocatedObject<Allocator>::operator delete[](void* p_Pointer,size_t p_Size)
{
	//return MemoryAllocator->deallocateDebug(p_Pointer,p_File,p_Line,true,p_Function);
	return Allocator::getInstance()->deallocateDebug(p_Pointer,"",0,true,"");
}
/*********************************************************************************/

#else //K15_DEBUG

/*********************************************************************************/
template<class Allocator>
void* AllocatedObject<Allocator>::alloc(size_t p_Size)
{
	return MemoryAllocator->allocate(p_Size);
}
/*********************************************************************************/
template<class Allocator>
void AllocatedObject<Allocator>::free(void* p_Pointer)
{
	MemoryAllocator->deallocate(p_Pointer);
}
/*********************************************************************************/
template<class Allocator>
void* AllocatedObject<Allocator>::new(size_t p_Size)
{
	return alloc(p_Size);
}
/*********************************************************************************/
template<class Allocator>
void AllocatedObject<Allocator>::delete(void* p_Pointer)
{
	MemoryAllocator->deallocate(p_Pointer);
}
/*********************************************************************************/
template<class Allocator>
void* AllocatedObject<Allocator>::new[](size_t p_Size)
{
	return MemoryAllocator->allocate(p_Size);
}
/*********************************************************************************/
template<class Allocator>
void AllocatedObject<Allocator>::delete[](void* p_Pointer)
{
	MemoryAllocator->deallocate(p_Pointer);
}
/*********************************************************************************/

#endif //K15_DEBUG