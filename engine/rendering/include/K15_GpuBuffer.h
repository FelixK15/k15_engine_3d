/**
 * @file K15_GpuBuffer.h
 * @author  Felix Klinge <f.klinge@k15games.de>
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
 *
 * @section DESCRIPTION
 *
 * 
 */

#ifndef _K15Engine_Renderer_GpuBuffer_h_
#define _K15Engine_Renderer_GpuBuffer_h_

#include "K15_Prerequisites.h"
#include "K15_AllocatedObject.h"
#include "K15_GpuResource.h"
#include "K15_RawData.h"

namespace K15_Engine { namespace Rendering { 
	/*********************************************************************************/
	class K15_CORE_API GpuBufferImplBase : public RenderingAllocatedObject
	{
	protected:
		GpuBufferImplBase();

	public:
		virtual ~GpuBufferImplBase();	

		virtual bool allocate(uint32 p_Size) = 0;

		virtual bool lock(uint32 p_StartPos,int32 p_Count) = 0;
		virtual bool unlock() = 0;

		virtual uint32 readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset) = 0;
		virtual uint32 writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset) = 0;

		void setBuffer(GpuBuffer* p_Buffer);
		GpuBuffer* getBuffer() const;

	protected:
		GpuBuffer* m_Buffer;
	};// end of GpuBufferImplBase class declaration
	/*********************************************************************************/
	class K15_CORE_API GpuBuffer :	public GpuResource,
									public RenderingAllocatedObject
	{
	public:
		/*********************************************************************************/
		struct K15_CORE_API CreationOptions
		{
			Enum BufferType;
			Enum LockOption;
			Enum UsageOption;
			Enum AccessOption;
			Enum IndexType;
			RawData InitialData;
			uint32 Size;

			CreationOptions();
		}; // end of GpuBuffer::CreationOptions class declaration
		/*********************************************************************************/
		struct PendingChange
		{
			uint32 size;
			uint32 offset;

			PendingChange(uint32 p_Size, uint32 p_Offset);
		}; // end of GpuBuffer::PendingChange class declaration
		/*********************************************************************************/
	public:
		/*********************************************************************************/
		enum eUsageOptions
		{
			UO_STATIC = 0,		//<! Content gets set ONLY on creation time.
			UO_DYNAMIC,			//<! Content gets set dynamically throughout the lifetime of the buffer.
			UO_STREAM,			//<! Content gets set dynamically on a per frame basis.
			
			UO_COUNT
		};// UsageOptions
		/*********************************************************************************/
		enum eLockOptions
		{
			LO_NORMAL = 0,	//<! Allows read/wrote 
			LO_DISCARD,		//<! Discard the content of the buffer as soon as it gets locked

			LO_COUNT
		};// LockOptions
		/*********************************************************************************/
		enum eBufferType
		{
			BT_VERTEX_BUFFER = 0,
			BT_INDEX_BUFFER,
			
			BT_COUNT
		};// BufferType
		/*********************************************************************************/
		enum eBufferAccess
		{
			BA_READ_ONLY = 0,
			BA_WRITE_ONLY,
			BA_READ_WRITE,

			BA_COUNT
		};// BufferAccess
		/*********************************************************************************/
	public:
		/*********************************************************************************/
		static const CreationOptions DefaultOptions;
		typedef DynamicArray(PendingChange) PendingChangesArray;
		/*********************************************************************************/

	public:
		GpuBuffer(const CreationOptions& p_Options = DefaultOptions);
		virtual ~GpuBuffer();

		INLINE void setType(Enum p_BufferType);
		INLINE void setLockOption(Enum p_LockOption);
		INLINE void setUsageOption(Enum p_UsageOption);
		INLINE void setAccessOption(Enum p_AccessOption);

		INLINE Enum getType() const;
		INLINE Enum getLockOption() const;
		INLINE Enum getUsageOption() const;
		INLINE Enum getAccessOption() const;

		INLINE uint32 getSize() const;
		INLINE uint32 getUsedSize() const;
		INLINE uint32 getFreeSize() const;

		INLINE bool isLocked() const;

		INLINE bool isDirty() const;
		INLINE void setDirty(bool p_Dirty);

		void* lock(uint32 p_Size = 0, uint32 p_Offset = 0);
		void unlock();

		uint32 readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset = 0);
		uint32 writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset = 0);

		void forceBufferUpdate();
		bool uploadShadowBufferToGpu();

		INLINE GpuBufferImplBase* getImpl();

	private:
		uint32 _writeToShadowCopy(uint32 p_Size, byte* p_Destination, uint32 p_Offset);
		uint32 _readFromShadowCopy(uint32 p_Size, byte* p_Destination, uint32 p_Offset);
		void _addPendingChange(uint32 p_Size, uint32 p_Offset);
		void _createBufferStorage(const CreationOptions& p_Options);

	protected:
		PendingChangesArray m_PendingChanges;
		GpuBufferImplBase* m_Impl;
		byte* m_ShadowCopy;
		uint32 m_Size;
		uint32 m_UsedSize;
		Enum m_LockOption;
		Enum m_UsageOption;
		Enum m_BufferType;
		Enum m_AccessOption;
		bool m_Locked;
		bool m_Dirty;
	};// end of GpuBuffer class declaration
	#include "K15_GpuBuffer.inl"
}}// end of K15_Engine::Renderer namespace

#endif //_K15Engine_Renderer_GpuBuffer_h_