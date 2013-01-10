/**
 * @file K15_BufferImpl.h
 * @author  Felix Klinge <f.klinge@k15games.de>
 * @version 1.0
 * @date 2012/09/22
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

#include "K15_StdInclude.h"

#ifndef __K15_BUFFERIMPL__
#define __K15_BUFFERIMPL__

namespace K15_Renderer
{
	class BufferImpl
	{
	public:
		enum BufferBinding{
			BB_NONE = -1,
			BB_INDEX_BUFFER = 0,	/*!<Buffer will be used as indexbuffer.*/	
			BB_VERTEX_BUFFER,		/*!<Buffer will be used as vertexbuffer.*/	
			BB_DEPTH_STENCIL,		/*!<Buffer will be used as depth stencil buffer.*/	
			BB_CONSTANT_BUFFER		/*!<Buffer will be used as constantbuffer.*/	
		};

		enum BufferUsage{
			BU_NONE = -1,
			BU_DEFAULT = 0,		/*!<Only the GPU can access the buffer (read + write).*/	
			BU_DYNAMIC,			/*!<The Buffer can be access by the GPU and the CPU (e.g. c++ code).*/	
			BU_IMMUTABLE		/*!<Once the buffer has been created, neither the GPU nor the CPU can access the buffer.*/	
		};

		enum BufferAccess{
			BA_NONE = -1,
			BA_WRITE = 0,		/*!<The CPU can write to the buffer (Only possible with BufferUsage BU_DYNAMIC).*/	
			BA_READ,			/*!<Only the GPU can access the buffer (read + write).*/	
			BA_NO_ACCESS		/*!<The CPU (e.g. c++ code) has no access to this buffer.*/	
		};

	public:
		virtual ~BufferImpl();

		inline void SetBufferUsage(BufferUsage buUsage);
		inline void SetBufferBinding(BufferBinding bbBinding);
		inline void SetBufferAccess(BufferAccess baAccess);
		inline bool HasShadowCopy() const;

		virtual void UpdateFromShadowCopy() = 0;

		inline BufferUsage GetBufferUsage() const;
		inline BufferBinding GetBufferBinding() const;
		inline BufferAccess GetBufferAccess() const;

		inline FormatType GetFormat() const;
		inline void SetFormat(FormatType ftType) const;

		inline U32 GetBufferSize() const;
		inline U32 GetStride() const;

		inline void SaveShadowCopy(bool bShadowCopy);

		inline void Lock();
		inline void Unlock();

		inline bool IsLocked() const;

		inline void *GetData() const;
		inline void SetData(void* pData);

	protected:
		BufferImpl();

	protected:
		BufferBinding m_bbBinding;
		BufferUsage m_buUsage;
		BufferAccess m_baAccess;
		FormatType m_ftType;

		U32 m_iBufferSize;
		U32 m_iStride;
		void *m_pData;
	};
}

#endif //__K15_BUFFERIMPL__