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

#ifndef K15_RENDERER_USE_PRECOMPILED_HEADERS
#	include "K15_RendererPrerequisites.h"
#endif //K15_RENDERER_USE_PRECOMPILED_HEADERS

namespace K15_Engine { namespace Rendering { 
	/*********************************************************************************/
	class GpuBufferImplBase
	{
	public:
		virtual void init(Enum p_BufferType,Enum p_UsageOptions,Enum p_LockOptions) = 0;
		virtual void shutdown() = 0;
		virtual void setLockOption(Enum p_LockOption) = 0;
		virtual void setUsageOption(Enum p_UsageOption) = 0;

		virtual bool lock() = 0;
		virtual bool unlock() = 0;

		virtual uint32 readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset) = 0;
		virtual uint32 writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset) = 0;

	protected:
		GpuBuffer *m_Buffer;
	};// end of GpuBufferImplBase class declaration
	/*********************************************************************************/
	class GpuBuffer
	{
	public:
		/*********************************************************************************/
		enum eUsageOptions
		{
			UO_STATIC = 0,		//<! Content gets set ONLY on creation time
			UO_DYNAMIC,			//<! Content gets set dynamically throughout the lifetime of the buffer
			UO_WRITE_ONLY,      //<! The content of the buffer can not be read
			
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
	public:
		GpuBuffer(Enum p_BufferType);
		GpuBuffer(Enum p_BufferType, Enum p_LockOption = UO_DYNAMIC, Enum p_UsageOption = LO_NORMAL, bool p_ShadowCopyEnabled = false);
		GpuBuffer(Enum p_BufferType, Enum p_LockOption, Enum p_UsageOption, uint32 p_InitialDataSize, byte* p_InitialData, uint32 p_InitialDataOffset = 0, bool p_ShadowCopyEnabled = false);
		~GpuBuffer();

		inline void setShadowCopyEnabled(bool p_Enabled);
		inline void setLockOption(Enum p_LockOption);
		inline void setUsageOption(Enum p_UsageOption);

		inline Enum getBufferType() const;
		inline Enum getLockOption() const;
		inline Enum getUsageOption() const;
		inline bool getShadowCopyEnabled() const;

		inline byte* getShadowCopy() const;
		inline uint32 getShadowCopySize() const;

		inline bool isLocked();

		void lock();
		void unlock();

		uint32 readData(uint32 p_Size, byte* p_Destination, uint32 p_Offset = 0, bool p_FromShadowCopy = false);
		uint32 writeData(uint32 p_Size, byte* p_Source, uint32 p_Offset = 0);

	private:
		void writeToShadowCopy(uint32 p_Size, byte* p_Destination, uint32 p_Offset);
		uint32 readFromShadowCopy(uint32 p_Size, byte* p_Destination, uint32 p_Offset);

	private:
		GpuBufferImplBase* m_Impl;
		byte* m_ShadowCopy;
		uint32 m_ShadowCopySize;
		Enum m_LockOption;
		Enum m_UsageOption;
		Enum m_BufferType;
		bool m_Locked;
		bool m_ShadowCopyEnabled;
	};// end of GpuBuffer class declaration
	#include "K15_GpuBuffer.inl"
}}// end of K15_Engine::Renderer namespace

#endif //_K15Engine_Renderer_GpuBuffer_h_