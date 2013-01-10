/**
 * @file K15_Buffer.h
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

#pragma once

#ifndef __K15_GPUBUFFER__
#define __K15_GPUBUFFER__

#include "K15_BufferImpl.h"
#include "K15_Format.h"

using namespace K15_EngineV2;

namespace K15_Renderer
{
	class BufferImpl;

	class Buffer : public Object
	{
		DECLARE_RTTI;

	public:
		Buffer();
		Buffer(BufferImpl::BufferBinding bbBinding,BufferImpl::BufferUsage buUsage,BufferImpl::BufferAccess baAccess,FormatType ftType,U32 iBufferSize,U32 iSizePerElement);
		virtual ~Buffer();

		inline void SetBufferUsage(BufferImpl::BufferUsage buUsage);
		inline void SetBufferBinding(BufferImpl::BufferBinding bbBinding);
		inline void SetBufferAccess(BufferImpl::BufferAccess baAccess);
		inline bool HasShadowCopy() const;

		inline void UpdateFromShadowCopy();

		inline BufferImpl::BufferUsage GetBufferUsage() const;
		inline BufferImpl::BufferBinding GetBufferBinding() const;
		inline BufferImpl::BufferAccess GetBufferAccess() const;

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


	private:
		BufferImpl *m_pBufferImpl;

	};
	typedef Pointer<Buffer> BufferPtr;
	#include "../src/K15_Buffer.inl"
}

#endif //__K15_GPUBUFFER__