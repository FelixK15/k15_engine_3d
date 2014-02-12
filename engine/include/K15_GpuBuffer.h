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

#ifndef K15_USE_PRECOMPILED_HEADERS
#	include "K15_Prerequisites.h"
#	include "K15_AllocatedObject.h"
#endif //K15_USE_PRECOMPILED_HEADERS

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
		GpuBuffer *m_Buffer;
	};// end of GpuBufferImplBase class declaration
	/*********************************************************************************/
	class K15_CORE_API GpuBuffer : public RenderingAllocatedObject
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
			VertexDeclaration* VertexLayout;
			RawData InitialData;

			CreationOptions();
		}; // end of GpuBuffer::CreationOptions class declaration
		/*********************************************************************************/
	public:
		/*********************************************************************************/
		enum eUsageOptions
		{
			UO_STATIC = 0,		//<! Content gets set ONLY on creation time
			UO_DYNAMIC,			//<! Content gets set dynamically throughout the lifetime of the buffer
			
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
		/*********************************************************************************/

	public:
		GpuBuffer(const CreationOptions& p_Options = DefaultOptions);
		~GpuBuffer();

		INLINE void setShadowCopyEnabled(bool p_Enabled);
		INLINE void setLockOption(Enum p_LockOption);
		INLINE void setUsageOption(Enum p_UsageOption);
		INLINE void setAccessOption(Enum p_AccessOption);

		INLINE Enum getType() const;
		INLINE Enum getLockOption() const;
		INLINE Enum getUsageOption() const;
		INLINE Enum getAccessOption() const;

		INLINE bool getShadowCopyEnabled() const;

		INLINE byte* getShadowCopy() const;
		INLINE uint32 getShadowCopySize() const;

		INLINE uint32 getSize() const;
		INLINE uint32 getUsedSize() const;

		INLINE bool isLocked() const;

		INLINE bool isDirty() const;
		INLINE void setDirty(bool p_Dirty);

		void lock(uint32 p_StartPos = 0, int32 p_Count = 0);
		void unlock();

		bool allocate(uint32 p_Size, bool p_AllocateFromGpuBuffer);

		uint32 readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset = 0, bool p_ReadFromGpuBuffer = false);
		uint32 writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset = 0, bool p_WriteToGpuBuffer = false);

		bool uploadShadowBufferToGpu();

		INLINE const GpuBufferImplBase* getImpl() const;

	private:
		uint32 writeToShadowCopy(uint32 p_Size, byte* p_Destination, uint32 p_Offset);
		uint32 readFromShadowCopy(uint32 p_Size, byte* p_Destination, uint32 p_Offset);

	protected:
		GpuBufferImplBase* m_Impl;
		byte* m_ShadowCopy;
		uint32 m_ShadowCopySize;
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