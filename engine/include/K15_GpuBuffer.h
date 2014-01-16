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

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#endif //K15_USE_PRECOMPILED_HEADER

namespace K15_Engine { namespace Rendering { 
	/*********************************************************************************/
	class K15_CORE_API GpuBufferImplBase
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
	class K15_CORE_API GpuBuffer
	{
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
		static const int32 LOCK_WHOLE_BUFFER = -1;
		/*********************************************************************************/

	public:
		GpuBuffer(Enum p_BufferType, Enum p_LockOption = UO_DYNAMIC, Enum p_UsageOption = LO_NORMAL, Enum p_AccessOption = BA_READ_WRITE, bool p_ShadowCopyEnabled = false);
		GpuBuffer(Enum p_BufferType, uint32 p_InitialDataSize, byte* p_InitialData, uint32 p_InitialDataOffset = 0,Enum p_LockOption = UO_DYNAMIC, Enum p_UsageOption = UO_DYNAMIC, Enum p_AccessOption = BA_READ_WRITE, bool p_ShadowCopyEnabled = false);
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

		void lock(uint32 p_StartPos = 0, int32 p_Count = LOCK_WHOLE_BUFFER);
		void unlock();

		bool allocate(uint32 p_Size);
		
		uint32 readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset = 0, bool p_FromShadowCopy = false);
		uint32 writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset = 0);

		INLINE const GpuBufferImplBase* getImpl() const;

	private:
		void writeToShadowCopy(uint32 p_Size, byte* p_Destination, uint32 p_Offset);
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
		bool m_ShadowCopyEnabled;
	};// end of GpuBuffer class declaration
	#include "K15_GpuBuffer.inl"
}}// end of K15_Engine::Renderer namespace

#endif //_K15Engine_Renderer_GpuBuffer_h_