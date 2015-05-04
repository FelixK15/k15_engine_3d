/**
 * @file K15_GpuResource.h
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
 * 
 */

#ifndef _K15Engine_Rendering_GpuResource_h_
#define _K15Engine_Rendering_GpuResource_h_

#include "K15_Prerequisites.h"

namespace K15_Engine { namespace Rendering {
	class K15_CORE_API GpuResource
	{
	public:
		GpuResource();
		virtual ~GpuResource();

		INLINE bool isBound() const;
		INLINE void setIsBound(bool p_Bound);

		INLINE RendererBase* getRenderer() const;
		INLINE void setRenderer(RendererBase* p_Renderer);

	private:
		RendererBase* m_Renderer;
		bool m_IsBound;
	}; // end of GpuResource class declaration
	#include "K15_GpuResource.inl"
}} // end of K15_Engine::Rendering namespace

#endif //_K15Engine_Rendering_GpuResource_h_