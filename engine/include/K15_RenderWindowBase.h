/**
 * @file K15_RenderWindowBase.h
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

#ifndef _K15Engine_System_RenderWindowBase_h_
#define _K15Engine_System_RenderWindowBase_h_

#ifndef K15_USE_PRECOMPILED_HEADER
#	include "K15_Prerequisites.h"
#	include "K15_Application.h"
#endif// K15_USE_PRECOMPILED_HEADER

#include "K15_AllocatedObject.h"

namespace K15_Engine { namespace System { 
	/*********************************************************************************/
	struct Resolution
	{
		uint32 width;
		uint32 height;
	};
	/*********************************************************************************/
	class K15_API_EXPORT RenderWindowBase : public ApplicationAllocatedObject
	{
	public:
		RenderWindowBase();
		virtual ~RenderWindowBase();

		virtual bool initialize() = 0;
		virtual void shutdown() = 0;

		virtual void setWindowTitle(const String& p_WindowTitle);
		const String& getWindowTitle() const;

		virtual void setResolution(const Resolution& p_Resolution);
		const Resolution& getResolution() const;

		virtual void setIsFullscreen(bool p_Fullscreen);
		bool isFullscreen() const;
	
	protected:
		String m_WindowTitle;
		Resolution m_CurrentResolution;
		bool m_IsFullscreen;
	};// end of RenderWindowBase class
	/*********************************************************************************/
}}// end of K15_Engine::System namespace

#endif //_K15Engine_System_RenderWindowBase_h_